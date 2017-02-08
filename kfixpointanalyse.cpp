#include "kfixpointanalyse.h"

#include <QFileDialog>
#include <QDir>
#include <QStringList>
#include <QRegularExpression>
#include <QProcess>

#include "krawdblintimg.h"

KFixPointAnalyse::KFixPointAnalyse(uint16_t _width, uint16_t _height, double _viewWStep, double _viewHStep, double _oaWidthLow, double _oaWidthHigh, double _oaHeightLow, double _oaHeightHigh)
    :m_iImgWidth(_width),
     m_iImgHeight(_height),
     m_fWStepAngle(_viewWStep),
     m_fHStepAngle(_viewHStep),
     m_iTotalWCount( static_cast<uint16_t>(std::fabs(_oaWidthLow-_oaWidthHigh)/_viewWStep+1) ),
     m_iTotalHCount( static_cast<uint16_t>(std::fabs(_oaHeightLow-_oaHeightHigh)/_viewHStep+1) ),
     m_fOAWidthLow(_oaWidthLow),
     m_fOAWidthHigh(_oaWidthHigh),
     m_fOAHeightLow(_oaHeightLow),
     m_fOAHeightHigh(_oaHeightHigh)
{

}

bool KFixPointAnalyse::BuildFileList()
{
    QString targetDir = QFileDialog::getExistingDirectory(NULL, QObject::tr("选择目录"), "..", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(targetDir.isEmpty() || targetDir.isNull()){
        return false;
    }
    QDir thisDir(targetDir);
    QStringList fileLists = thisDir.entryList(QStringList()<<QObject::tr("*.DAT"), QDir::Files | QDir::NoSymLinks | QDir::Readable, QDir::Name);

    if(fileLists.empty()) return false;


    std::vector<std::string> vecFileLists;
    //m_sCurDirectory = ( QDir::toNativeSeparators(thisDir.absolutePath()) + QDir::separator() ).toLocal8Bit().data();
    //qDebug()<<m_sCurDirectory.c_str();
    for(QString item : fileLists){
        vecFileLists.push_back((QDir::toNativeSeparators(thisDir.absolutePath()) + QDir::separator() + item).toLocal8Bit().data());
    }

    // traverse all the position
    QRegularExpression re;
    m_mFileBuff.clear();
    uint32_t hashtemp = 0;
    for(uint16_t yIndex = 0; yIndex < m_iTotalHCount; ++yIndex){
        for(uint16_t xIndex = 0; xIndex < m_iTotalWCount; ++xIndex){
            hashtemp = yIndex * static_cast<uint16_t>(65535) + xIndex;
            std::ostringstream os;
            os.precision(3);
            os.setf(std::ios::fixed);
            os.str("");
            os<<m_fOAWidthLow+xIndex*m_fWStepAngle<<"_"<<m_fOAHeightLow-yIndex*m_fHStepAngle;

            re.setPattern(os.str().c_str());
            // find the real filename
            for(std::string &item : vecFileLists){
                QRegularExpressionMatch match = re.match(item.c_str());
                if (match.hasMatch()){
                   m_mFileBuff.insert(std::make_pair(hashtemp, item));
                   break;
                }
            }
        }
    }

    return true;
}

bool KFixPointAnalyse::DrawCurve(KFixPointAnalyse::outputformat type, std::string filename, uint16_t index, std::tuple<uint16_t, uint16_t> querypos)
{
    if(DumpPoint2File(type, filename, index, querypos)){
        std::ofstream of(filename+".plot", std::ofstream::out | std::ios_base::trunc);

        std::ostringstream os;

        of << "set terminal png small\r\n";

        std::ifstream filerecorder(filename, std::ifstream::in);

        os << "set output \"" << [&]()->std::string{
                                    std::string temp("");
                                    for(auto & c : filename){
                                        /* the ascii code of '\' is 92 */
                                        if(c != 92) temp.push_back(c);
                                        else temp.push_back('/');
                                    }
                                    return temp;
                                }() << ".png\"\r\n";
        os << "plot " << "\"" << [&]()->std::string{
                   std::string temp("");
                   for(auto & c : filename){
                       /* the ascii code of '\' is 92 */
                       if(c != 92) temp.push_back(c);
                       else temp.push_back('/');
                   }
                   return temp;
               }() << "\" " << "u 1:2" << " w lp lt 1"
               <<" lw 1.7 pt 1 " << "ps 0.6 t \""<< index
              << "_" << std::get<0>(querypos) << "_" << std::get<1>(querypos) <<"\"\r\n";


        of << os.str();

        filerecorder.close();
        of.close();

        QProcess process;
        QStringList arguments;
        arguments << (filename+".plot").c_str();
        process.start("D://gnuplot//bin//gnuplot.exe", arguments);
        if (!process.waitForStarted())
            return false;

        if (!process.waitForFinished())
            return false;
        process.close();

        return true;
    }
    return false;
}

bool KFixPointAnalyse::DumpPoint2File(KFixPointAnalyse::outputformat type, std::string filename, uint16_t index, std::tuple<uint16_t, uint16_t> querypos)
{
    std::ofstream of(filename.c_str(), std::ios_base::out | std::ios_base::trunc);
    uint32_t hashtemp = 0;
    uint16_t pixel_value = 0;
    if(type == LIST_HORIZONTAL){
        if(index > m_iTotalHCount - 1) return false;
        else{
            for(uint16_t xIndex = 0; xIndex < m_iTotalWCount; ++xIndex){
                hashtemp = index * static_cast<uint16_t>(65535) + xIndex;
                std::unordered_map<uint32_t,std::string>::iterator it = m_mFileBuff.find(hashtemp);
                if(it == m_mFileBuff.end()) of << xIndex << "\t" << 0 << std::endl;
                else{
                    get_point_value( it->second , querypos, pixel_value);
                    of << xIndex << "\t" << pixel_value << std::endl;
                }
            }

        }
    }else{
        if(index > m_iTotalWCount - 1) return false;
        else{
            for(uint16_t yIndex = 0; yIndex < m_iTotalHCount; ++yIndex){
                hashtemp = yIndex * static_cast<uint16_t>(65535) + index;

                std::unordered_map<uint32_t,std::string>::iterator it = m_mFileBuff.find(hashtemp);
                if(it == m_mFileBuff.end()) of << yIndex << "\t" << 0 << std::endl;
                else{
                    get_point_value( it->second , querypos, pixel_value);
                    of << yIndex << "\t" << pixel_value << std::endl;
                }
            }
        }
    }

    of.close();
    return true;
}

bool KFixPointAnalyse::get_point_value(std::string filename, std::tuple<uint16_t, uint16_t> querypos, uint16_t &value, bool bAverage)
{
    uint16_t *image = new uint16_t[m_iImgWidth * m_iImgHeight];

    KRawDblIntImg imageContainer(filename, m_iImgWidth, m_iImgHeight);
    if(imageContainer.isvalid()){
        imageContainer.image_buff_reset();

        if(bAverage){
            double pixel_sum = 0.;
            uint16_t imagenum = imageContainer.get_image_count();
            for(uint16_t index = 0;index < imagenum;++index){
                if(imageContainer.get_next_image(image)){
                    pixel_sum += image[std::get<1>(querypos)*m_iImgWidth+std::get<0>(querypos)];
                }else return false;
            }
            pixel_sum /= imagenum;
            value = static_cast<uint16_t>(pixel_sum);
        }else{
            if(imageContainer.get_next_image(image)){
                value = image[std::get<1>(querypos)*m_iImgWidth+std::get<0>(querypos)];
            }else return false;
        }

    }else return false;

    delete[] image;
    return true;
}
