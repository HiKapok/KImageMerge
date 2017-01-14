#include "kcurveanalyse.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QStringList>
#include <QRegularExpression>

#include "krawdblintimg.h"

KCurveAnalyse::KCurveAnalyse(uint16_t _width, uint16_t _height, double _viewWidth, double _viewHiehgt, double _oaWidthLow, double _oaWidthHigh, double _oaHeightLow, double _oaHeightHigh)
     :m_iImgWidth(_width),
      m_iImgHeight(_height),
      m_iStep(static_cast<uint16_t>( (_width/_viewWidth + _height/_viewHiehgt)/2.) ),
      m_itotalWidth( static_cast<uint16_t>((std::fabs(_oaWidthLow-_oaWidthHigh)+_viewWidth)*_width/_viewWidth) ),
      m_itotalHeight( static_cast<uint16_t>((std::fabs(_oaHeightLow-_oaHeightHigh)+_viewHiehgt)*_height/_viewHiehgt) ),
      m_fOAWidthLow(_oaWidthLow),
      m_fOAWidthHigh(_oaWidthHigh),
      m_fOAHeightLow(_oaHeightLow),
      m_fOAHeightHigh(_oaHeightHigh)
{
//    qDebug()<<"m_iStep:"<<m_iStep<<"m_itotalWidth:"<<m_itotalWidth
//           <<"m_itotalHeight:"<<m_itotalHeight;

    //unittest_pixel2image();

}

bool KCurveAnalyse::pixel2file(uint16_t pixel_posX, uint16_t pixel_posY, std::vector<std::string>& result)
{
    m_vecXImageIndex.clear();
    m_vecYImageIndex.clear();
    result.clear();
    if(pixel2imagenum(pixel_posX, m_iImgWidth, 21, m_vecXImageIndex) < 1 || pixel2imagenum(pixel_posY, m_iImgHeight, 23, m_vecYImageIndex)<1){
        return false;
    }

    std::ostringstream os;
    os.precision(3);
    os.setf(std::ios::fixed);
    for(uint16_t yIndex = 0;yIndex<m_vecYImageIndex.size();++yIndex){
        for(uint16_t xIndex = 0;xIndex<m_vecXImageIndex.size();++xIndex){
            os.str("");
            os<<m_fOAWidthLow+(m_vecXImageIndex[xIndex]-1)<<"_"<<m_fOAHeightLow-(m_vecYImageIndex[yIndex]-1);
            result.push_back(os.str());
        }
    }

    return true;
}

uint16_t KCurveAnalyse::pixel2imagenum(uint16_t pixel, uint16_t length, uint16_t max_image, std::vector<uint16_t> &result)
{
    int16_t min_bound = std::min(std::floor((pixel*1.)/m_iStep+1),std::ceil(((pixel*1.)-length+1)/m_iStep)+1);
    int16_t max_bound = std::max(std::floor((pixel*1.)/m_iStep+1),std::ceil(((pixel*1.)-length+1)/m_iStep)+1);
    result.clear();
    min_bound = std::max(min_bound, static_cast<int16_t>(1));
    max_bound = std::min(max_bound, static_cast<int16_t>(max_image));
    for(int16_t index = min_bound;index <= max_bound;++index){
        result.push_back(static_cast<uint16_t>(index));
    }
    return result.size();
}

bool KCurveAnalyse::get_pixel_gray(std::string filename, std::tuple<uint16_t, uint16_t> position, uint16_t& pixel_gray, bool bAverage){

    uint16_t *image = new uint16_t[m_iImgWidth * m_iImgHeight];

    KRawDblIntImg imageContainer(filename, m_iImgWidth, m_iImgHeight);
    if(imageContainer.isvalid()){
        imageContainer.image_buff_reset();

        if(bAverage){
            double pixel_sum = 0.;
            uint16_t imagenum = imageContainer.get_image_count();
            for(uint16_t index = 0;index < imagenum;++index){
                if(imageContainer.get_next_image(image)){
                    pixel_sum += image[std::get<1>(position)*m_iImgWidth+std::get<0>(position)];
                }else return false;
            }
            pixel_sum /= imagenum;
            pixel_gray = static_cast<uint16_t>(pixel_sum);
        }else{
            if(imageContainer.get_next_image(image)){
                pixel_gray = image[std::get<1>(position)*m_iImgWidth+std::get<0>(position)];
            }else return false;
        }

    }else return false;

    delete[] image;
    return true;
}

bool KCurveAnalyse::get_pixel_list(std::tuple<uint16_t, uint16_t> position, std::vector<std::vector<uint16_t>> &vecPixel)
{
    std::vector<std::string> imagelist;

    if(!pixel2file(std::get<0>(position), std::get<1>(position), imagelist)) return false;

    uint16_t xCount = m_vecXImageIndex.size();
    uint16_t yCount = m_vecYImageIndex.size();

    QRegularExpression re;
    qDebug()<<"xCount:"<<xCount<<" yCount:"<<yCount;
    for(uint16_t yIndex = 0; yIndex < yCount; ++yIndex){
        std::vector<uint16_t> tempVec;
        for(uint16_t xIndex = 0; xIndex < xCount; ++xIndex){
            std::string tempFileCue = imagelist[yIndex*xCount+xIndex];
            re.setPattern(tempFileCue.c_str());
            uint16_t pixel_value = 0;
            std::string realFileName("");
            // find the real filename
            for(std::string &item : m_vecFileLists){
                QRegularExpressionMatch match = re.match(item.c_str());
                if (match.hasMatch()){
                   realFileName = item;
                   break;
                }
            }
            //qDebug()<<realFileName.c_str();
            //get pixel value
            if(realFileName.empty()){
                std::cout<<"NULL("<<m_fOAWidthLow+(m_vecXImageIndex[xIndex]-1)<<"_"<<m_fOAHeightLow-(m_vecYImageIndex[yIndex]-1)<<") ";
                continue;
            }
            if(!get_pixel_gray(realFileName,std::make_tuple<uint16_t, uint16_t>(std::get<0>(position)-(m_vecXImageIndex[xIndex]-1)*m_iStep
                                                                                ,std::get<1>(position)-(m_vecYImageIndex[yIndex]-1)*m_iStep), pixel_value, true)){
                std::cout<<"get pixel value error (@file:"<<realFileName<<")."<<std::endl;
            }else{
                tempVec.push_back(pixel_value);
                std::cout<<pixel_value<<"("<<m_fOAWidthLow+(m_vecXImageIndex[xIndex]-1)<<"_"<<m_fOAHeightLow-(m_vecYImageIndex[yIndex]-1)
                        <<"__"<<std::get<0>(position)-(m_vecXImageIndex[xIndex]-1)*m_iStep
                        <<"_"<<std::get<1>(position)-(m_vecYImageIndex[yIndex]-1)*m_iStep<<") ";
            }
        }
        vecPixel.push_back(tempVec);
        std::cout << std::endl;
    }

    return true;
}

bool KCurveAnalyse::get_filelist(){
    QString targetDir = QFileDialog::getExistingDirectory(NULL, QObject::tr("选择目录"), "..", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(targetDir.isEmpty() || targetDir.isNull()){
        return false;
    }
    QDir thisDir(targetDir);
    QStringList fileLists = thisDir.entryList(QStringList()<<QObject::tr("*.DAT"), QDir::Files | QDir::NoSymLinks | QDir::Readable, QDir::Name);

    if(fileLists.empty()) return false;
    m_vecFileLists.clear();

    for(QString item : fileLists){
        m_vecFileLists.push_back((QDir::toNativeSeparators(thisDir.absolutePath()) + QDir::separator() + item).toLocal8Bit().data());
    }

    return true;
}

