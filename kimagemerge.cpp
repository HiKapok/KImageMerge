#include "kimagemerge.h"

#include "kimagecollector.h"

#include <memory>
#include <sstream>
#include <functional>

#include <QString>
#include <QDebug>

KImageMerge::KImageMerge(uint16_t _width, uint16_t _height, uint16_t _hCounts, uint16_t _vCounts, uint16_t _hOverlap, uint16_t _vOverlap, bool bShowLine)
    : m_iImageWidth(_width)
    , m_iImageHeight(_height)
    , m_iHImageCounts(_hCounts)
    , m_iVImageCounts(_vCounts)
    , m_iHOverLap(_hOverlap < m_iImageWidth ? _hOverlap : (m_iImageWidth-1))
    , m_iVOverLap(_vOverlap < m_iImageHeight ? _vOverlap : (m_iImageHeight-1))
    , m_iHCurCounts(0)
    , m_iVCurCounts(0)
    , m_ifShowLine(bShowLine)
    , m_sOutputFile("")
    , m_iOutputWidth(0)
    , m_iOutputHeight(0)
    , m_iFileIndex(0)
    //, m_iLastLine(0)
    //, m_iLastColumn(0)
    , m_fileHolder(std::make_shared<KImageCollector>())
{
    //qDebug()<<(m_iHImageCounts * (m_iImageWidth - m_iHOverLap) + m_iHOverLap) << (m_iVImageCounts * (m_iImageHeight - m_iVOverLap) + m_iVOverLap);
    if(m_iHImageCounts < 2) m_iHOverLap = 0;
    if(m_iVImageCounts < 2) m_iVOverLap = 0;
    m_pTempImage = new uint16_t[(m_iHImageCounts * (m_iImageWidth - m_iHOverLap) + m_iHOverLap) * m_iImageHeight];
}

//bool KImageMerge::set_widget(QWidget * _widget){

//    std::shared_ptr<KImageCollector> tempPtr(new KImageCollector(_widget));
//    std::swap(m_fileHolder, tempPtr);
//}

bool KImageMerge::merge_image(KImageMerge::MergeType mergeType, std::string saveFile)
{
    if(m_fileHolder->total_num() != static_cast<uint32_t>(m_iHImageCounts) * static_cast<uint32_t>(m_iVImageCounts)) return false;
    // clear output file

    m_iOutputWidth = m_iHImageCounts * (m_iImageWidth - m_iHOverLap) + m_iHOverLap;
    m_iOutputHeight = m_iVImageCounts * (m_iImageHeight - m_iVOverLap) + m_iVOverLap;

    std::ostringstream os;
    os << m_iOutputWidth << "X" << m_iOutputHeight;
    std::size_t found = saveFile.find_last_of("./\\");
    saveFile = saveFile.substr(0,found) + "_" + os.str() + ".DAT";

    m_sOutputFile = saveFile;
    //m_sOutputSuffix = os.str();

    std::ios_base::openmode mode = std::ios_base::binary | std::ios_base::out | std::ios_base::trunc;
    std::ofstream fs(saveFile.c_str(), mode);
    //if(!fs) return false;
    fs.close();
    if(SymmetryPrefer == mergeType){
        std::ofstream fs((saveFile+std::string(".tmp")).c_str(), mode);
        //if(!fs) return false;
        fs.close();
    }

    ++m_iFileIndex;
    if(FormerPrefer == mergeType)
        m_fileHolder->each_file(m_iImageWidth, m_iImageHeight, std::bind(&KImageMerge::doFormerMerge, this, saveFile, std::placeholders::_1));
    else if(LatterPrefer == mergeType) m_fileHolder->each_file(m_iImageWidth, m_iImageHeight, std::bind(&KImageMerge::doLatterMerge, this, saveFile, std::placeholders::_1));
    else if(SymmetryPrefer == mergeType) m_fileHolder->each_file(m_iImageWidth, m_iImageHeight, std::bind(&KImageMerge::doLatterMerge, this, saveFile, std::placeholders::_1));
    else m_fileHolder->each_file(m_iImageWidth, m_iImageHeight, std::bind(&KImageMerge::doCenterReflectMerge, this, saveFile, std::placeholders::_1));

    return true;
}

bool KImageMerge::prepare_files(QString targetDir)
{
    bool _sts = m_fileHolder->prepare_lists(targetDir);
    if(_sts){
        // never clear these vars unless a true result
        m_iFileIndex = 0;
        m_iHCurCounts = 0;
        m_iVCurCounts = 0;
        //m_iLastColumn = 0;
        //m_iLastLine = 0;
    }
    return _sts;
}

bool KImageMerge::doFormerMerge(std::string saveFile, uint16_t *image)
{
    uint32_t totalWidth = m_iHImageCounts * (m_iImageWidth - m_iHOverLap) + m_iHOverLap;
    uint32_t totalHeight = m_iImageHeight;

    uint32_t indexHeight = (0 == m_iVCurCounts ? 0 : m_iVOverLap);
    int32_t delta = (0 == m_iHCurCounts ? 0 : m_iHOverLap);
//    qDebug()<<"image height from:"<<indexHeight<<"to"<<m_iImageHeight-1<<";total height"<<m_iImageHeight-indexHeight;
//    qDebug()<<"image width from:"<<(0 == m_iHCurCounts ? 0 : m_iHOverLap)<<"to"<<m_iImageWidth-1<<";total width"<<(m_iImageWidth-(0 == m_iHCurCounts ? 0 : m_iHOverLap));
//    qDebug()<<"destination width from:"<<m_iHCurCounts*(m_iImageWidth - m_iHOverLap)+delta<<"to"<<m_iHCurCounts*(m_iImageWidth - m_iHOverLap)+delta+(m_iImageWidth-(0 == m_iHCurCounts ? 0 : m_iHOverLap))-1<<";total width:"<<(m_iImageWidth-(0 == m_iHCurCounts ? 0 : m_iHOverLap));

    for(;indexHeight < m_iImageHeight;++indexHeight){
        uint16_t * ptrLine = &m_pTempImage[indexHeight*totalWidth + m_iHCurCounts*(m_iImageWidth - m_iHOverLap)+delta];
       //qDebug()<<indexHeight<<m_iHCurCounts*(m_iImageWidth - m_iHOverLap)+delta;
        uint32_t indexWidth = (0 == m_iHCurCounts ? 0 : m_iHOverLap);
        if(m_ifShowLine){
            for(;indexWidth < m_iImageWidth;++indexWidth){
                if(indexWidth+1 == m_iImageWidth)
                    *ptrLine = 0;
                else if(indexWidth+m_iHOverLap+1 == m_iImageWidth)
                        *ptrLine = 65535;
                    else
                        *ptrLine = image[indexHeight*m_iImageWidth+indexWidth];
                ptrLine++;
            }
        }else{
            for(;indexWidth < m_iImageWidth;++indexWidth){
                *ptrLine = image[indexHeight*m_iImageWidth+indexWidth];
                ptrLine++;
            }
        }
    }
    ++m_iHCurCounts;
    if(m_iHCurCounts == m_iHImageCounts){
        uint32_t heightStart = (0 == m_iVCurCounts ? 0:m_iVOverLap);
        // flush the tempImage here
        if(!write2file(FormerPrefer, saveFile, totalWidth, totalHeight, heightStart, m_pTempImage)){
            return false;
        }
        ++m_iVCurCounts;
        m_iHCurCounts = 0;
    }

    return true;
}

bool KImageMerge::doLatterMerge(std::string saveFile, uint16_t *image)
{
    uint32_t totalWidth = m_iHImageCounts * (m_iImageWidth - m_iHOverLap) + m_iHOverLap;
    uint32_t totalHeight = m_iImageHeight;

    uint32_t maxHeight = (m_iVImageCounts == (m_iVCurCounts + 1) ? m_iImageHeight : (m_iImageHeight - m_iVOverLap));
//    qDebug()<<"image height from:"<<0<<"to"<<maxHeight<<";total height"<<maxHeight;
//    qDebug()<<"image width from:"<<0<<"to"<<(m_iHImageCounts == (m_iHCurCounts + 1) ? m_iImageWidth : (m_iImageWidth - m_iHOverLap))<<";total width"<<(m_iHImageCounts == (m_iHCurCounts + 1) ? m_iImageWidth : (m_iImageWidth - m_iHOverLap));
//    qDebug()<<"destination width from:"<<m_iHCurCounts*(m_iImageWidth - m_iHOverLap)<<"to"<<m_iHCurCounts*(m_iImageWidth - m_iHOverLap)+(m_iHImageCounts == (m_iHCurCounts + 1) ? m_iImageWidth : (m_iImageWidth - m_iHOverLap))<<";total width:"<<(m_iHImageCounts == (m_iHCurCounts + 1) ? m_iImageWidth : (m_iImageWidth - m_iHOverLap));


    for(uint32_t indexHeight = 0;indexHeight < maxHeight;++indexHeight){
        uint16_t * ptrLine = &m_pTempImage[indexHeight*totalWidth + m_iHCurCounts*(m_iImageWidth - m_iHOverLap)];
       //qDebug()<<indexHeight<<m_iHCurCounts*(m_iImageWidth - m_iHOverLap)+delta;
        uint32_t maxWidth = (m_iHImageCounts == (m_iHCurCounts + 1) ? m_iImageWidth : (m_iImageWidth - m_iHOverLap));
        if(m_ifShowLine){
            for(uint32_t indexWidth = 0;indexWidth < maxWidth;++indexWidth){
                if(indexWidth+1 == maxWidth)
                    *ptrLine = 0;
                else if(indexWidth+1 == m_iHOverLap)
                        *ptrLine = 65535;
                    else
                        *ptrLine = image[indexHeight*m_iImageWidth+indexWidth];
                ptrLine++;
            }
        }else{
            for(uint32_t indexWidth = 0;indexWidth < maxWidth;++indexWidth){
                *ptrLine = image[indexHeight*m_iImageWidth+indexWidth];
                ptrLine++;
            }
        }
    }
    ++m_iHCurCounts;
    if(m_iHCurCounts == m_iHImageCounts){
        uint32_t heightEnd = (m_iVImageCounts == (m_iVCurCounts+1) ? totalHeight:(totalHeight-m_iVOverLap));
        // flush the tempImage here
        if(!write2file(LatterPrefer, saveFile, totalWidth, heightEnd, 0, m_pTempImage)){
            return false;
        }
        ++m_iVCurCounts;
        m_iHCurCounts = 0;
    }

    return true;
}

bool KImageMerge::doSymmetryMerge(std::string saveFile, uint16_t *image)
{
    uint32_t hBlockSize = m_iImageWidth - m_iHOverLap;
    uint32_t vBlockSize = m_iImageHeight - m_iVOverLap;

    uint32_t halfHSlices = (m_iOutputWidth/2) / hBlockSize + 1;
    uint32_t eachHDelta  = m_iOutputWidth/2 - (halfHSlices-1) * hBlockSize;

    uint32_t halfVSlices = (m_iOutputHeight/2) / vBlockSize + 1;
    uint32_t eachVDelta  = m_iOutputHeight/2 - (halfVSlices-1) * vBlockSize;

    uint32_t indexHeight = 0;

    uint32_t firstCopyVSize = ((m_iVCurCounts+1) == halfVSlices) ? eachVDelta:vBlockSize;
    uint32_t secondCopyVSize = ((m_iVCurCounts+halfVSlices) == m_iVImageCounts) ? eachVDelta:vBlockSize;
    //qDebug()<<"halfVSlices"<<halfVSlices<<"halfHSlices"<<halfHSlices;
    //qDebug()<<"eachHDelta"<<eachHDelta<<"eachVDelta"<<eachVDelta;
    if(m_iVCurCounts < halfVSlices){
        for(;indexHeight < firstCopyVSize;++indexHeight){
            uint32_t indexWidth = 0;
            uint32_t copyHSize = ((m_iHCurCounts+1) == halfHSlices) ? eachHDelta:hBlockSize;
            uint16_t *ptrLine = &m_pTempImage[indexHeight*m_iOutputWidth + hBlockSize*m_iHCurCounts];

            if(m_iHCurCounts < halfHSlices){
                for(;indexWidth < copyHSize;++indexWidth){
                    *ptrLine++ = image[indexHeight*m_iImageWidth+indexWidth];
                }
            }
            if(m_iHCurCounts + halfHSlices >= m_iHImageCounts){
                //for(;indexWidth < m_iHOverLap;++indexWidth);
                copyHSize = ((m_iHCurCounts + halfHSlices) == m_iHImageCounts) ? eachHDelta:hBlockSize;
                //while(copyHSize-->0) ++indexWidth;
                if((m_iHCurCounts + halfHSlices) == m_iHImageCounts)
                    ptrLine = &m_pTempImage[indexHeight*m_iOutputWidth + m_iOutputWidth/2];
                else
                    ptrLine = &m_pTempImage[indexHeight*m_iOutputWidth + m_iOutputWidth/2 + eachHDelta + hBlockSize*(m_iHCurCounts + halfHSlices - 1 - m_iHImageCounts)];
                for(indexWidth = m_iImageWidth-copyHSize;indexWidth < m_iImageWidth;++indexWidth){
                    *ptrLine++ = image[indexHeight*m_iImageWidth+indexWidth];
                }
            }
        }
    }
    if(m_iVCurCounts + halfVSlices >= m_iVImageCounts){
        for(indexHeight = m_iImageHeight-secondCopyVSize;indexHeight < m_iImageHeight;++indexHeight){
            uint32_t indexWidth = 0;
            uint32_t copyHSize = ((m_iHCurCounts+1) == halfHSlices) ? eachHDelta:hBlockSize;
            uint16_t *ptrLine = &m_pTempImage[indexHeight*m_iOutputWidth + hBlockSize*m_iHCurCounts];

            if(m_iHCurCounts < halfHSlices){
                for(;indexWidth < copyHSize;++indexWidth){
                    *ptrLine++ = image[indexHeight*m_iImageWidth+indexWidth];
                }
            }
            if(m_iHCurCounts + halfHSlices >= m_iHImageCounts){
                //for(;indexWidth < m_iHOverLap;++indexWidth);
                copyHSize = ((m_iHCurCounts + halfHSlices) == m_iHImageCounts) ? eachHDelta:hBlockSize;
                //while(copyHSize-->0) ++indexWidth;
                if((m_iHCurCounts + halfHSlices) == m_iHImageCounts)
                    ptrLine = &m_pTempImage[indexHeight*m_iOutputWidth + m_iOutputWidth/2];
                else
                    ptrLine = &m_pTempImage[indexHeight*m_iOutputWidth + m_iOutputWidth/2 + eachHDelta + hBlockSize*(m_iHCurCounts + halfHSlices - 1 - m_iHImageCounts)];
                for(indexWidth = m_iImageWidth-copyHSize;indexWidth < m_iImageWidth;++indexWidth){
                    *ptrLine++ = image[indexHeight*m_iImageWidth+indexWidth];
                }
            }
        }
    }
    ++m_iHCurCounts;
    if(m_iHCurCounts == m_iHImageCounts){
        if(m_iVCurCounts < halfVSlices){
            if(!write2file(saveFile, m_iOutputWidth, firstCopyVSize, 0, m_pTempImage)){
                return false;
            }
        }
        if(m_iVCurCounts + halfVSlices >= m_iVImageCounts){
            if(!write2file(saveFile + std::string(".tmp"), m_iOutputWidth, m_iImageHeight, m_iImageHeight-secondCopyVSize, m_pTempImage)){
                return false;
            }
        }
        ++m_iVCurCounts;
        if(m_iVCurCounts == m_iVImageCounts){
            if(!write2file(saveFile, saveFile + std::string(".tmp"))){
                return false;
            }
        }
        m_iHCurCounts = 0;
    }

    return true;

/*
    uint32_t totalWidth = m_iHImageCounts * (m_iImageWidth - m_iHOverLap) + m_iHOverLap;
    //uint32_t totalHeight = m_iImageHeight;

    // no matter the column and row nums is odd or even
    // copy (column(row)'s length - 2*overlap) when curcount equals column(row)/2
    // otherwise copy (column(row)'s length - overlap)
    // besides column(row)'s length at the beginning and the end
    uint32_t columnToCopy = ( (0 == m_iHCurCounts || (m_iHImageCounts-1) == m_iHCurCounts) ? m_iImageWidth : (m_iImageWidth - m_iHOverLap));
    uint32_t rowToCopy = ( (0 == m_iVCurCounts || (m_iVImageCounts-1) == m_iVCurCounts) ? m_iImageHeight : (m_iImageHeight - m_iVOverLap));

    if(m_iHCurCounts == (m_iHImageCounts/2)) columnToCopy -= m_iHOverLap;
    if(m_iVCurCounts == (m_iVImageCounts/2)) rowToCopy -= m_iVOverLap;

//    qDebug()<<"image height from:"<<indexHeight<<"to"<<m_iImageHeight-1<<";total height"<<m_iImageHeight-indexHeight;
//    qDebug()<<"image width from:"<<(0 == m_iHCurCounts ? 0 : m_iHOverLap)<<"to"<<m_iImageWidth-1<<";total width"<<(m_iImageWidth-(0 == m_iHCurCounts ? 0 : m_iHOverLap));
//    qDebug()<<"destination width from:"<<m_iHCurCounts*(m_iImageWidth - m_iHOverLap)+delta<<"to"<<m_iHCurCounts*(m_iImageWidth - m_iHOverLap)+delta+(m_iImageWidth-(0 == m_iHCurCounts ? 0 : m_iHOverLap))-1<<";total width:"<<(m_iImageWidth-(0 == m_iHCurCounts ? 0 : m_iHOverLap));
    uint32_t indexStart = m_iHOverLap;
    if((m_iHCurCounts > m_iHImageCounts/2) || (0 == m_iHCurCounts)){
        indexStart = 0;
    }

    for(uint32_t indexHeight = 0;indexHeight < rowToCopy;++indexHeight){
        uint16_t * ptrLine = &m_pTempImage[indexHeight*totalWidth + m_iLastColumn];
       //qDebug()<<indexHeight<<m_iHCurCounts*(m_iImageWidth - m_iHOverLap)+delta;
        if(m_ifShowLine){
            for(uint32_t indexWidth = 0;indexWidth < columnToCopy;++indexWidth){
                if(indexWidth+1 == columnToCopy)
                    *ptrLine = 0;
                else if(indexWidth+m_iHOverLap+1 == columnToCopy)
                        *ptrLine = 65535;
                    else
                        *ptrLine = image[indexHeight*m_iImageWidth+indexWidth+indexStart];
                ptrLine++;
            }
        }else{
            for(uint32_t indexWidth = 0;indexWidth < columnToCopy;++indexWidth){
                *ptrLine = image[indexHeight*m_iImageWidth+indexWidth+indexStart];
                ptrLine++;
            }
        }
    }

    ++m_iHCurCounts;
    m_iLastColumn += columnToCopy;
    if(m_iHCurCounts == m_iHImageCounts){
        // reuse write2file just change the FormerPrefer to LatterPrefer when m_iVCurCounts > m_iVImageCounts/2
        if(!write2file(FormerPrefer, saveFile, totalWidth, rowToCopy, 0, m_pTempImage)){
            return false;
        }
        ++m_iVCurCounts;
        m_iHCurCounts = 0;
        m_iLastColumn = 0;
        m_iLastLine += rowToCopy;
    }

    return true;
*/
}

bool KImageMerge::doCenterReflectMerge(std::string saveFile, uint16_t *image)
{
    uint32_t hStartColumn = 0;
    uint32_t vStartRow = 0;
    uint32_t hCopyColumn = 0;
    uint32_t vCopyRow = 0;
    uint32_t hCopyStart = 0;

    if(m_iHCurCounts <= m_iHImageCounts/2){
        hCopyStart = m_iHCurCounts * (m_iImageWidth - m_iHOverLap);
    }else{
        hCopyStart = m_iHCurCounts * (m_iImageWidth - m_iHOverLap) + m_iHOverLap;
    }
    if(m_iHCurCounts < m_iHImageCounts/2){
        hStartColumn = 0;
        hCopyColumn = m_iImageWidth - m_iHOverLap;
    }else{
        if(m_iHCurCounts > m_iHImageCounts/2){
            hStartColumn = m_iHOverLap;
            hCopyColumn = m_iImageWidth - m_iHOverLap;
        }else{
            hStartColumn = 0;
            hCopyColumn = m_iImageWidth;
        }
    }
    if(m_iVCurCounts < m_iVImageCounts/2){
        vStartRow = 0;
        vCopyRow = m_iImageHeight - m_iVOverLap;
    }else{
        if(m_iVCurCounts > m_iVImageCounts/2){
            vStartRow = m_iVOverLap;
            vCopyRow = m_iImageHeight - m_iVOverLap;
        }else{
            vStartRow = 0;
            vCopyRow = m_iImageHeight;
        }
    }


    for(uint32_t indexHeight = vStartRow;indexHeight < vStartRow + vCopyRow;++indexHeight){
        uint16_t *ptrLine = &m_pTempImage[indexHeight*m_iOutputWidth + hCopyStart];

        for(uint32_t indexWidth = hStartColumn;indexWidth < hStartColumn + hCopyColumn;++indexWidth){
            *ptrLine++ = image[indexHeight*m_iImageWidth+indexWidth];
        }
    }

    ++m_iHCurCounts;
    if(m_iHCurCounts == m_iHImageCounts){
        if(!write2file(saveFile, m_iOutputWidth, vStartRow + vCopyRow, vStartRow, m_pTempImage)){
            return false;
        }
        ++m_iVCurCounts;

        m_iHCurCounts = 0;
    }

    return true;
}

bool KImageMerge::write2file(KImageMerge::MergeType mergeType, std::string saveFile, uint32_t width, uint32_t height, uint32_t startHeight, uint16_t *image)
{
    std::ios_base::openmode mode = std::ios_base::binary | std::ios_base::out | std::ios_base::app;

    std::ofstream fs(saveFile.c_str(), mode);
    for(uint32_t indexHeight=startHeight;indexHeight < height;++indexHeight){
        if(m_ifShowLine){
            if(FormerPrefer == mergeType){
                for(uint32_t indexWidth=0;indexWidth < width;++indexWidth){
                    if(indexHeight+1 == height)
                        m_iTempElem.split_into_raw(0);
                    else if(indexHeight+m_iVOverLap+1 == height)
                            m_iTempElem.split_into_raw(65535);
                        else
                            m_iTempElem.split_into_raw(image[indexHeight*width+indexWidth]);
                    fs.write(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
                }
            }else{
                for(uint32_t indexWidth=0;indexWidth < width;++indexWidth){
                    if(indexHeight + 1 == height)
                        m_iTempElem.split_into_raw(0);
                    else if(indexHeight + 1 == m_iVOverLap)
                            m_iTempElem.split_into_raw(65535);
                        else
                            m_iTempElem.split_into_raw(image[indexHeight*width+indexWidth]);
                    fs.write(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
                }
            }
        }else{
            for(uint32_t indexWidth=0;indexWidth < width;++indexWidth){
                m_iTempElem.split_into_raw(image[indexHeight*width+indexWidth]);
                fs.write(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
            }
        }
    }
    fs.close();

    return true;
}

bool KImageMerge::write2file(std::string saveFile, uint32_t width, uint32_t height, uint32_t startHeight, uint16_t *image)
{
    std::ios_base::openmode mode = std::ios_base::binary | std::ios_base::out | std::ios_base::app;

    std::ofstream fs(saveFile.c_str(), mode);
    for(uint32_t indexHeight=startHeight;indexHeight < height;++indexHeight){
        for(uint32_t indexWidth=0;indexWidth < width;++indexWidth){
            m_iTempElem.split_into_raw(image[indexHeight*width+indexWidth]);
            fs.write(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
        }
    }
    fs.close();

    return true;
}

bool KImageMerge::write2file(std::string saveFile, std::string mergeSource)
{
    std::ios_base::openmode mode = std::ios_base::binary | std::ios_base::out | std::ios_base::app;

    std::ofstream fsdes(saveFile.c_str(), mode);
    std::ifstream fssrc(mergeSource.c_str(), std::ios_base::binary | std::ios_base::in);
    fssrc.read(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
    while(fssrc){
        fsdes.write(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
        fssrc.read(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
    }
    fsdes.close();
    fssrc.close();

    return true;
}

KImageMerge::~KImageMerge(){
    delete[] m_pTempImage;
}
