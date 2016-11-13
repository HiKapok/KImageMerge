#include "kimagecollector.h"

#include <functional>

#include <QString>
#include <QFileDialog>
#include <QObject>
#include <QWidget>
#include <QDir>
#include <QDebug>
#include <QStringList>
#include <QRegularExpression>

#include "krawdblintimg.h"

//class KStringCompare : public std::binary_function<std::string, std::string, bool> {
//public:
//    KStringCompare()
//        : re((QObject::tr("_([0-9.]{4,6})_"))){}
//    result_type operator()(first_argument_type lhs, second_argument_type rhs){
//        QRegularExpressionMatch lmatch = re.match(lhs.c_str());
//        QRegularExpressionMatch rmatch = re.match(rhs.c_str());

//        if (lmatch.hasMatch() && rmatch.hasMatch()) {
//            return lmatch.captured(1).toInt() < rmatch.captured(1).toInt();
//        }
//        return false;
//    }
//private:
//    QRegularExpression re;
//};

//QRegularExpression re((QObject::tr("_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})\\.DAT")));
//QRegularExpressionMatch lmatch = re.match("AutoLvdsImage_02_-10.000_2.500.DAT");

//if (lmatch.hasMatch()) {
//    qDebug()<< lmatch.captured(2);
//}
class KStringCompare : public std::binary_function<std::string, std::string, bool> {
public:
    KStringCompare()
        : re((QObject::tr("_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})\\.DAT"))){}
    result_type operator()(first_argument_type lhs, second_argument_type rhs){
        QRegularExpressionMatch lmatch = re.match(lhs.c_str());
        QRegularExpressionMatch rmatch = re.match(rhs.c_str());

        if (lmatch.hasMatch() && rmatch.hasMatch()) {
            if(lmatch.captured(2).toDouble() > rmatch.captured(2).toDouble()) return true;
            else{
                if(lmatch.captured(2).toDouble() == rmatch.captured(2).toDouble()){
                    if(lmatch.captured(1).toDouble() <= rmatch.captured(1).toDouble()) return true;
                    else return false;
                }else return false;

            }
            //return (lmatch.captured(2).toDouble() > rmatch.captured(2).toDouble() ) && (lmatch.captured(1).toInt() < rmatch.captured(1).toInt());
            //return lmatch.captured(1).toInt() < rmatch.captured(1).toInt();
        }
        return false;
    }
private:
    QRegularExpression re;
};
KImageCollector::KImageCollector(/*QWidget *pWidget*/)
    //: m_pMainWidget(pWidget)
{

}


bool KImageCollector::prepare_lists(QString targetDir){
    QDir thisDir(targetDir);
    QStringList fileLists = thisDir.entryList(QStringList()<<QObject::tr("*.DAT"), QDir::Files | QDir::NoSymLinks | QDir::Readable, QDir::Name);


    if(fileLists.empty()) return false;
    m_vecFileLists.clear();
//    for(uint16_t index = 0;index < fileLists.size();++index){
//        m_vecFileLists.push_back((QDir::toNativeSeparators(thisDir.absolutePath()) + QDir::separator() + fileLists.at(index)).toLocal8Bit().data());
//    }
    for(QString item : fileLists){
        m_vecFileLists.push_back((QDir::toNativeSeparators(thisDir.absolutePath()) + QDir::separator() + item).toLocal8Bit().data());
    }
    std::sort(m_vecFileLists.begin(), m_vecFileLists.end(), KStringCompare());
//    for(auto item:m_vecFileLists)
//    {
//        qDebug()<<item.c_str();
//    }
    return true;
}

void KImageCollector::each_file(uint16_t width, uint16_t height, std::function<bool(uint16_t*)> mergeCore)
{
    uint16_t *image = new uint16_t[width * height];

    for(auto file : m_vecFileLists){
        KRawDblIntImg imageContainer(file, width, height);
        if(imageContainer.isvalid()){
            imageContainer.image_buff_reset();
            if(imageContainer.get_next_image(image)){
                if(!mergeCore(image)) return;
            }else return;
        }else return;
    }
    delete[] image;
}

KImageCollector::~KImageCollector(){

}

