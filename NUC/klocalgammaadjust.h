#ifndef KLOCALGAMMAADJUST
#define KLOCALGAMMAADJUST

#include <cstdint>
#include <cfloat>

class KLocalGammaAdjust
{
public:
    KLocalGammaAdjust(uint16_t _bright, uint16_t _width, uint16_t _height, uint16_t _woverlap, uint16_t _hoverlap, double _gamma, double _meanThres, double _sigmaThres)
        : m_iBright(_bright)
        , m_iWidth(_width)
        , m_iHeight(_height)
        , m_iWOverlap(_woverlap)
        , m_iHOverlap(_hoverlap)
        , m_fThresMean(_meanThres)
        , m_fThresSigma(_sigmaThres)
        , m_fGammaRatio(_gamma)
    {
    }
    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
    {
        uint16_t lwidth = m_iWidth -  m_iWOverlap;
        uint16_t rwidth = m_iWOverlap;
        uint16_t xBlocks = (2 * _width - lwidth) / m_iWidth + 1;
        uint16_t yBlocks = (2 * _height - rwidth) / m_iHeight + 1;

        double * pAdjustTabel = new (std::nothrow) double[xBlocks * yBlocks];
        if (nullptr == pAdjustTabel) return false;

        for (uint16_t iXB = 0; iXB < xBlocks; ++iXB)
        {
            for (uint16_t iYB = 0; iYB < yBlocks; ++iYB)
            {
                uint16_t xStart = (iXB+1) / 2 * lwidth + iXB / 2 * rwidth;
                uint16_t yStart = (iYB+1) / 2 * lwidth + iYB / 2 * rwidth;

                uint16_t xWidth = m_iWidth;
                uint16_t yWidth = m_iHeight;

                if (xStart + m_iWidth > _width) xWidth = _width - xStart;
                if (yStart + m_iHeight > _height) yWidth = _height - yStart;

                double local_min = (std::numeric_limits<double>::max)();
                double local_max = -(std::numeric_limits<double>::max)();
                double _mean = 0.;
                double _sigma = 0.;

                for (uint16_t iY = yStart; iY < yStart + yWidth; ++iY)
                {
                    for (uint16_t iX = xStart; iX < xStart + xWidth; ++iX)
                    {
                        _mean += _image[iY * _width + iX];
                        if(local_min > _image[iY * _width + iX]) local_min = _image[iY * _width + iX];
                        if(local_max < _image[iY * _width + iX]) local_max = _image[iY * _width + iX];
                    }
                }
                _mean /= (xWidth*yWidth);
                for (uint16_t iY = yStart; iY < yStart + yWidth; ++iY)
                {
                    for (uint16_t iX = xStart; iX < xStart + xWidth; ++iX)
                    {
                        _sigma += std::pow((_image[iY * _width + iX] - _mean), 2.);
                    }
                }
                _sigma /= (xWidth*yWidth);
                double adjust_result = 0.;
                if(_mean < m_fThresMean && _sigma < m_fThresSigma){
                    adjust_result = local_max/_limit;
                }else{
                    adjust_result = m_iBright * std::pow((_image[(yStart+yWidth/2) * _width + xStart+xWidth/2]-local_min)/(local_max-local_min), m_fGammaRatio);
                    //adjust_result = m_iBright * std::pow((_mean-local_min)/(local_max-local_min),m_fGammaRatio);
                }//adjust_result = m_iBright * std::pow((_image[(yStart+yWidth/2) * _width + xStart+xWidth/2]-local_min)/(local_max-local_min),m_fGammaRatio);

//qDebug()<<_mean<<_mean;
                //qDebug()<<adjust_result;
                pAdjustTabel[iYB * xBlocks + iXB] = adjust_result;
            }

        }

        // make bilinear interpolation
        for (int16_t iRow = 0; iRow < _height; iRow++)
        {
            for (int16_t iColumn = 0; iColumn < _width; iColumn++)
            {
                float fHorBlock = (2. * iColumn - lwidth) / m_iWidth + .5f;
                float fVerBlock = (2. * iRow - rwidth) / m_iHeight + .5f;
                int16_t iVerBlock = static_cast<int16_t>(fVerBlock);
                int16_t iHorBlock = static_cast<int16_t>(fHorBlock);
                float fAlpha = fHorBlock - iHorBlock;
                float fBeta = fVerBlock - iVerBlock;

                if (iVerBlock == yBlocks - 1)
                {
                    iVerBlock--;
                    fBeta += 1;
                }
                if (iHorBlock == xBlocks - 1)
                {
                    iHorBlock--;
                    fAlpha += 1;
                }

                if (iVerBlock >= 0 && iVerBlock < yBlocks - 1 && iHorBlock >= 0 && iHorBlock < xBlocks - 1)
                {
                    _image[iRow * _width + iColumn] = static_cast<uint16_t>(pAdjustTabel[iVerBlock*xBlocks + iHorBlock] * (1 - fBeta) * (1 - fAlpha)
                        + pAdjustTabel[iVerBlock*xBlocks + iHorBlock + 1] * (1 - fBeta) * fAlpha
                        + pAdjustTabel[(iVerBlock + 1) * xBlocks + iHorBlock] * fBeta * (1 - fAlpha)
                        + pAdjustTabel[(iVerBlock + 1) * xBlocks + iHorBlock + 1] * fBeta * fAlpha);
                }
            }
        }
        delete[] pAdjustTabel;
        return false;
    }

private:
    uint16_t m_iBright;
    uint16_t m_iWidth;
    uint16_t m_iHeight;
    uint16_t m_iWOverlap;
    uint16_t m_iHOverlap;
    double m_fThresMean;
    double m_fThresSigma;
    double m_fGammaRatio;
};

#endif // KLOCALGAMMAADJUST

