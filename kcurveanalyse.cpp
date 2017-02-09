#include "kcurveanalyse.h"

#include <QDebug>
#include <QFileDialog>
#include <QDir>
#include <QStringList>
#include <QRegularExpression>
#include <QProcess>

#include "krawdblintimg.h"

KCurveAnalyse::KCurveAnalyse(uint16_t _width, uint16_t _height, double _viewWStep, double _viewHStep, double _oaWidthLow, double _oaWidthHigh, double _oaHeightLow, double _oaHeightHigh, double _viewWidth, double _viewHiehgt)
     :m_iImgWidth(_width),
      m_iImgHeight(_height),
      m_fWStepAngle(_viewWStep),
      m_fHStepAngle(_viewHStep),
      m_iWStep( static_cast<uint16_t>(_viewWStep*_width/_viewWidth) ),
      m_iHStep( static_cast<uint16_t>(_viewHStep*_height/_viewHiehgt) ),
      m_iTotalWidth( static_cast<uint16_t>((std::fabs(_oaWidthLow-_oaWidthHigh)+_viewWidth)*_width/_viewWidth) ),
      m_iTotalHeight( static_cast<uint16_t>((std::fabs(_oaHeightLow-_oaHeightHigh)+_viewHiehgt)*_height/_viewHiehgt) ),
      m_iTotalWCount( static_cast<uint16_t>(std::fabs(_oaWidthLow-_oaWidthHigh)/_viewWStep+1) ),
      m_iTotalHCount( static_cast<uint16_t>(std::fabs(_oaHeightLow-_oaHeightHigh)/_viewHStep+1) ),
      m_fOAWidthLow(_oaWidthLow),
      m_fOAWidthHigh(_oaWidthHigh),
      m_fOAHeightLow(_oaHeightLow),
      m_fOAHeightHigh(_oaHeightHigh),
      m_sCurDirectory("")
{
//    qDebug()<<"m_iStep:"<<m_iStep<<"m_iTotalWidth:"<<m_iTotalWidth
//           <<"m_iTotalHeight:"<<m_iTotalHeight;
//qDebug()<<"m_iWStep:"<<m_iWStep<<" m_iHStep:"<<m_iHStep;
    //unittest_pixel2image();

}

bool KCurveAnalyse::pixel2file(uint16_t pixel_posX, uint16_t pixel_posY, std::vector<std::string>& result)
{
    m_vecXImageIndex.clear();
    m_vecYImageIndex.clear();
    result.clear();
    if(pixel2imagenum(pixel_posX, m_iImgWidth, m_iWStep, m_iTotalWCount, m_vecXImageIndex) < 1 || pixel2imagenum(pixel_posY, m_iImgHeight, m_iHStep, m_iTotalHCount, m_vecYImageIndex)<1){
        return false;
    }

   // for(auto & item : m_vecXImageIndex) std::cout<<item<<" "<<std::endl;

    // translate the image index to the string such as: -10.000_2.500
    // all of these strings are put in the container result
    std::ostringstream os("", std::ios_base::ate);
    os.precision(3);
    os.setf(std::ios::fixed);
    for(uint16_t yIndex = 0;yIndex<m_vecYImageIndex.size();++yIndex){
        for(uint16_t xIndex = 0;xIndex<m_vecXImageIndex.size();++xIndex){
            os.str("_");
            os<<m_fOAWidthLow+(m_vecXImageIndex[xIndex]-1)*m_fWStepAngle<<"_"<<m_fOAHeightLow-(m_vecYImageIndex[yIndex]-1)*m_fHStepAngle;
            result.push_back(os.str());
        }
    }

    return true;
}

uint16_t KCurveAnalyse::pixel2imagenum(uint16_t pixel, uint16_t length, uint16_t step, uint16_t max_image, std::vector<uint16_t> &result)
{
    int16_t min_bound = std::min(std::floor((pixel*1.)/step+1),std::ceil(((pixel*1.)-length+1)/step)+1);
    int16_t max_bound = std::max(std::floor((pixel*1.)/step+1),std::ceil(((pixel*1.)-length+1)/step)+1);
    result.clear();
    min_bound = std::max(min_bound, static_cast<int16_t>(1));
    min_bound = std::min(min_bound, static_cast<int16_t>(max_image));
    max_bound = std::min(max_bound, static_cast<int16_t>(max_image));
    //qDebug()<<"ddd:"<<min_bound<<max_bound<<max_image;
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

    std::cout<<"query position:"<<std::get<0>(position)<<" "<<std::get<1>(position)<<std::endl;
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
                std::cout<<"NULL("<<m_fOAWidthLow+(m_vecXImageIndex[xIndex]-1)*m_fWStepAngle<<"_"<<m_fOAHeightLow-(m_vecYImageIndex[yIndex]-1)*m_fHStepAngle<<") ";
                continue;
            }
            if(!get_pixel_gray(realFileName,std::make_tuple<uint16_t, uint16_t>(std::get<0>(position)-(m_vecXImageIndex[xIndex]-1)*m_iWStep
                                                                                ,std::get<1>(position)-(m_vecYImageIndex[yIndex]-1)*m_iHStep), pixel_value, true)){
                std::cout<<"get pixel value error (@file:"<<realFileName<<")."<<std::endl;
            }else{
                tempVec.push_back(pixel_value);
                std::cout<<pixel_value<<"("<<m_fOAWidthLow+(m_vecXImageIndex[xIndex]-1)*m_fWStepAngle<<"_"<<m_fOAHeightLow-(m_vecYImageIndex[yIndex]-1)*m_fHStepAngle
                        <<"__"<<std::get<0>(position)-(m_vecXImageIndex[xIndex]-1)*m_iWStep
                        <<"_"<<std::get<1>(position)-(m_vecYImageIndex[yIndex]-1)*m_iHStep<<") ";
            }
        }
        vecPixel.push_back(tempVec);
        std::cout << std::endl;
    }

    for(auto &resultitem: vecPixel){
        if(!resultitem.empty()){
            for(auto &item: resultitem){
                std::cout <<item<<" ";
            }
            std::cout<< std::endl;
        }
    }
    return true;
}


bool KCurveAnalyse::get_pixel_point(std::tuple<uint16_t, uint16_t> intialPoint, std::tuple<uint16_t, uint16_t> step, std::ostream &output, outputformat format)
{
    QRegularExpression re;
    std::ostringstream os;
    uint16_t wpos = std::get<0>(intialPoint);
    uint16_t hpos = std::get<1>(intialPoint);

    std::vector<std::string> imagelist;
    std::vector<KPointDetail> pointlist;
    uint16_t xCount = 0;
    uint16_t yCount = 0;
//qDebug()<<m_iTotalWidth<<m_iTotalHeight;
    for(;wpos < m_iTotalWidth && hpos < m_iTotalHeight;){
        //qDebug()<<wpos<<hpos;
        if(!pixel2file(wpos, hpos, imagelist)) return false;
        xCount = m_vecXImageIndex.size();
        yCount = m_vecYImageIndex.size();
        KPointDetail point_detail(std::make_tuple(wpos, hpos), std::make_tuple(xCount, yCount)
                                  , std::make_tuple(m_vecXImageIndex[0], m_vecXImageIndex[xCount-1])
                                  , std::make_tuple(m_vecYImageIndex[0], m_vecYImageIndex[yCount-1]), format);
        pointlist.push_back(point_detail);
        for(uint16_t yIndex = 0; yIndex < yCount; ++yIndex){
            for(uint16_t xIndex = 0; xIndex < xCount; ++xIndex){
                std::string tempFileCue = imagelist[yIndex*xCount+xIndex];
                re.setPattern(tempFileCue.c_str());
                uint16_t pixel_value = 0;
                std::string realFileName("");
                // find the real filename
                for(std::string &item : m_vecFileLists){
                    QRegularExpressionMatch match = re.match(item.c_str());
                    if (match.hasMatch()){
                        //qDebug()<<yIndex<<xIndex<<item.c_str();

                       realFileName = item;
                       break;
                    }
                }

                //get pixel value
                if(!realFileName.empty()){
                    // once some error occurs when reading the image, terminate the program.
                    if(!get_pixel_gray(realFileName,std::make_tuple<uint16_t, uint16_t>(wpos-(m_vecXImageIndex[xIndex]-1)*m_iWStep
                                                                                        ,hpos-(m_vecYImageIndex[yIndex]-1)*m_iHStep), pixel_value, true)){

                        std::cout<<"get pixel value error (@file:"<<realFileName<<")."<<std::endl;
                        std::terminate();
                    }else{
                        pointlist[pointlist.size()-1].put_point_by_coord(pixel_value, std::make_tuple(yIndex, xIndex));
                    }
                }else{
                    // assume the pixel value is 0 when the file doesn't exsit.
                    pointlist[pointlist.size()-1].put_point_by_coord(0, std::make_tuple(yIndex, xIndex));
                }
            }
        }
        if(LIST_VERTICAL == format){
            hpos += std::get<1>(step);
        }else wpos += std::get<0>(step);

    }

    std::ofstream of(m_sCurDirectory+"recorder.txt", std::ofstream::out | std::ios_base::trunc);

    m_vecOutFileLists.clear();
    if(LIST_VERTICAL == format){
        // fill recorder file
        os << xCount - 1 << "\t";
        for(auto & pointdetail : pointlist){
            os<<std::get<1>(pointdetail.get_org_coord());
            os<<"\t";
        }
        of << os.str() << "\r\n";
        os.str("");
        /* xCount must be same for all points */
        for(uint16_t index = 0;index < xCount;++index){
            os<<m_sCurDirectory.c_str()<<"points__x"<<m_vecXImageIndex[0]<<"("<<m_fOAWidthLow+(m_vecXImageIndex[0]-1)*m_fWStepAngle<<")_x"
            <<m_vecXImageIndex[xCount-1]<<"("<<m_fOAWidthLow+(m_vecXImageIndex[xCount-1]-1)*m_fWStepAngle<<")__";
            os<<index<<"("<<xCount<<").txt";
            std::ofstream datafile(os.str(), std::ofstream::out | std::ios_base::trunc);
            m_vecOutFileLists.push_back(std::string( os.str().c_str() ));
            std::cout << os.str().c_str() <<std::endl;
            os.str("");

            os<<"index\t";
            for(auto & pointdetail : pointlist){
                os<<std::get<1>(pointdetail.get_org_coord());
                os<<"\t";
            }
            os<<"column:"<<std::get<0>(intialPoint);
            os<<"\r\n";
            for(uint16_t count = 0;count < m_iTotalHCount;++count){
                os<<count<<"\t";
                for(auto & pointdetail : pointlist){
                    uint16_t temp = 0;
                    if(pointdetail.get_point_by_realcoord(index, count, temp))
                        os<<temp;
                    else{ os<<0; }
                    os<<"\t";
                }
                os<<"\r\n";
            }

            datafile<<os.str();
            datafile.close();
            os.str("");
        }
    }else{
        // fill recorder file
        os << yCount - 1 << "\t";
        for(auto & pointdetail : pointlist){
            os<<std::get<0>(pointdetail.get_org_coord());
            os<<"\t";
        }
        of << os.str() << "\r\n";
        os.str("");
        /* yCount must be same for all points */
        for(uint16_t index = 0;index < yCount;++index){
            os<<m_sCurDirectory.c_str()<<"points__y"<<m_vecYImageIndex[0]<<"("<<m_fOAHeightLow+(m_vecYImageIndex[0]-1)*m_fHStepAngle<<")_y"
            <<m_vecYImageIndex[yCount-1]<<"("<<m_fOAHeightLow+(m_vecYImageIndex[yCount-1]-1)*m_fHStepAngle<<")__";
            os<<index<<"("<<yCount<<").txt";
            std::ofstream datafile(os.str(), std::ofstream::out | std::ios_base::trunc);
            m_vecOutFileLists.push_back(std::string( os.str().c_str() ));
            std::cout << os.str().c_str()<<std::endl;
            os.str("");

            os<<"index\t";
            for(auto & pointdetail : pointlist){
                os<<std::get<0>(pointdetail.get_org_coord());
                os<<"\t";
            }
            os<<"row:"<<std::get<1>(intialPoint);
            os<<"\r\n";
            for(uint16_t count = 0;count < m_iTotalWCount;++count){
                os<<count<<"\t";
                for(auto & pointdetail : pointlist){
                    uint16_t temp = 0;
                    if(pointdetail.get_point_by_realcoord(index, count, temp))
                        os<<temp;
                    else{ os<<0; }
                    os<<"\t";
                }
                os<<"\r\n";
            }

            datafile<<os.str();
            datafile.close();
            os.str("");
        }

    }

    of.close();
    return true;
}

bool KCurveAnalyse::draw_curve(std::string tempfilename)
{
    std::ofstream of(tempfilename, std::ofstream::out | std::ios_base::trunc);

    std::ostringstream os;

    of << "set terminal png small\r\n";

    std::ifstream filerecorder(m_sCurDirectory+"recorder.txt", std::ifstream::in);

    uint16_t num_column = 0;
    filerecorder >> num_column;
    uint16_t * pLineName = new (std::nothrow) uint16_t[num_column];

    if(nullptr == pLineName) return false;

    for(uint16_t index = 0;index < num_column;++index){
        filerecorder >> pLineName[index];
    }
    //std::cout << num_column << std::endl;

    for(auto & str : m_vecOutFileLists){  
        os.str("");
        os << "set output \"" << [&]()->std::string{
                                    std::string temp("");
                                    for(auto & c : str){
                                        /* the ascii code of '\' is 92 */
                                        if(c != 92) temp.push_back(c);
                                        else temp.push_back('/');
                                    }
                                    return temp;
                                }() << ".png\"\r\n";
        os << "plot ";
        for(uint16_t index = 1;index <= num_column;++index){
            os << "\"" << [&]()->std::string{
                   std::string temp("");
                   for(auto & c : str){
                       /* the ascii code of '\' is 92 */
                       if(c != 92) temp.push_back(c);
                       else temp.push_back('/');
                   }
                   return temp;
               }() << "\" " << "u 1:" << index + 1 << " w lp lt " << index%6 + 1
               <<" lw 1.7 pt " << index%13+1 <<"ps 0.6 t \""<< pLineName[index-1] <<"\"";
            if(index != num_column) os << ", ";
        }
        of << os.str() << "\r\n";
    }

    if(nullptr != pLineName){
        delete [] pLineName;
    }
    filerecorder.close();
    of.close();

    QProcess process;
    QStringList arguments;
    arguments << tempfilename.c_str();
    process.start("D://gnuplot//bin//gnuplot.exe", arguments);
    if (!process.waitForStarted())
        return false;

    if (!process.waitForFinished())
        return false;
    process.close();

    return true;
}

bool KCurveAnalyse::get_filelist(){
    QString targetDir = QFileDialog::getExistingDirectory(NULL, QObject::tr("选择目录"), "..", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(targetDir.isEmpty() || targetDir.isNull()){
        return false;
    }
//.replace("/","_")
    QDir thisDir(targetDir);
    QStringList fileLists = thisDir.entryList(QStringList()<<QObject::tr("*.DAT"), QDir::Files | QDir::NoSymLinks | QDir::Readable, QDir::Name);

    if(fileLists.empty()) return false;
    m_vecFileLists.clear();
    m_sCurDirectory = ( QDir::toNativeSeparators(thisDir.absolutePath()) + QDir::separator() ).toLocal8Bit().data();
    //qDebug()<<m_sCurDirectory.c_str();
    for(QString item : fileLists){
        m_vecFileLists.push_back((QDir::toNativeSeparators(thisDir.absolutePath()) + QDir::separator() + item).toLocal8Bit().data());
    }

    return true;
}

