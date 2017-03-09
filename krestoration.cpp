#include "krestoration.h"


#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QStringList>
#include <QRegularExpression>

#include <thread>
#include <mutex>
#include <functional>
#include <cmath>

KRestoration::KRestoration()
{
    if(loadFileFromDir()) fixAngleDistortion();
}

bool KRestoration::loadFileFromDir()
{
    QString targetDir = QFileDialog::getExistingDirectory(NULL, QObject::tr("选择目录"), "D://HustProjects", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(targetDir.isEmpty() || targetDir.isNull()){
        return false;
    }
    QDir thisDir(targetDir);
    QStringList fileLists = thisDir.entryList(QStringList()<<QObject::tr("*.DAT"), QDir::Files | QDir::NoSymLinks | QDir::Readable, QDir::Name);

    if(fileLists.empty()) return false;
    m_vecFileLists.clear();
    m_sCurDirectory = ( QDir::toNativeSeparators(thisDir.absolutePath()) + QDir::separator() ).toLocal8Bit().data();

    m_sDesDirectory = ( QDir::toNativeSeparators(thisDir.absolutePath()) + QObject::tr("_转换文件") + QDir::separator() ).toLocal8Bit().data();
    QDir newDir;
    newDir.mkpath(QDir::toNativeSeparators(thisDir.absolutePath())+QObject::tr("_转换文件"));
    //qDebug()<<m_sDesDirectory.c_str();
    for(QString item : fileLists){
        m_vecFileLists.push_back((QDir::toNativeSeparators(thisDir.absolutePath()) + QDir::separator() + item).toLocal8Bit().data());
    }

    return true;
}

bool KRestoration::fixAngleDistortion()
{
    QRegularExpression re(QObject::tr("_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})"));
    bool sucess1 = false;
    bool sucess2 = false;
    for(auto & str : m_vecFileLists){
        QRegularExpressionMatch match = re.match(str.c_str());
        if (match.hasMatch()){
            double horizon = match.captured(1).toDouble(&sucess1);
            double vertical = match.captured(2).toDouble(&sucess2);
            //qDebug()<<horizon << vertical;
            if(sucess1 && sucess2){
                m_vecTask.push_back(std::packaged_task<bool()>(std::bind(KRestoration::runAngleCorrection, this, str, horizon, vertical)));
            }else{
                std::cerr << "failed to process file: " << str << std::endl;
            }
        }else{
            std::cerr << "failed to process file: " << str << std::endl;
        }

    }
    //std::cout << std::thread::hardware_concurrency() << std::endl;
    std::vector<std::thread> vecThreads;
    for(uint16_t index = 0;index < std::thread::hardware_concurrency();index++){
        vecThreads.push_back(std::move(std::thread(std::bind(runTask,this))));
    }
    for(auto & thread : vecThreads){
        auto id = thread.get_id();
        thread.join();
        std::cout << "thread " << id << " joined." << std::endl;
    }
    return true;
}

bool KRestoration::runAngleCorrection(std::string file, double horizon, double vertical)
{
    horizon = std::cos(horizon * 0.0055555555555556 * 3.14159265358979323846);
    vertical = std::cos((vertical+8.5) * 0.0055555555555556 * 3.14159265358979323846);

    if(!transformCopy(file, m_sDesDirectory+file.substr(file.find_last_of("\\/")+1), std::bind(fixPixelAngleDistor, this, horizon, vertical, std::placeholders::_1))){
        //std::call_once(m_oneFlag, std::terminate);
        std::cerr << "thread failed: id=" << std::this_thread::get_id() << "." << std::endl;
    }
    return true;
}

void KRestoration::runTask()
{
    std::packaged_task<bool()> task;
    for(;;){
        {
            std::lock_guard<std::mutex> locker(m_mutex);
            if(!m_vecTask.empty()) task = std::move(m_vecTask.front());
            else break;
            m_vecTask.pop_front();
        }
        std::future<bool> result = task.get_future();
        task();
        if(!result.get()){
            std::lock_guard<std::mutex> locker(m_mutex);
            std::cerr << "one task failed." << std::endl;
        }
    }
}

void KRestoration::fixPixelAngleDistor(double horizon, double vertical, uint16_t& pixel)
{
    //std::cout << pixel << std::endl;
    uint32_t temp = pixel / horizon / vertical;
    //std::cout << temp << std::endl;
    if(temp > 65535) {
        temp = 65535;
        //std::cout << "overflow." << std::endl;
    }
    pixel = temp;

}


template<typename _callable>
bool KRestoration::transformCopy(std::string input, std::string output, _callable fun)
{
    std::ifstream fs(input.c_str(), std::ifstream::binary);
    std::ofstream ofs(output.c_str(), std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);

    if (!fs || !ofs) return false;
    fs.seekg(0, fs.end);
    uint32_t _length = static_cast<int>(fs.tellg()) / 2;
    fs.seekg(0, fs.beg);
    KStructMem tempBuff;
    uint16_t temp_pixel = 0;
    for (uint32_t index(0); index < _length; ++index){
        fs.read(reinterpret_cast<char *>(&tempBuff), sizeof(KStructMem));
        temp_pixel = static_cast<uint16_t>(tempBuff.get_real_value());
        fun(std::ref(temp_pixel));
        tempBuff.split_into_raw(temp_pixel);
        ofs.write(reinterpret_cast<char *>(&tempBuff), sizeof(KStructMem));

    }
    fs.close();
    ofs.close();

    return true;
}
