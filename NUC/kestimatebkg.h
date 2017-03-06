#ifndef KESTIMATEBKG
#define KESTIMATEBKG

#include <cstdint>
#include <cfloat>
#include <memory>
#include <cmath>

#include <QDebug>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class KEstimateBkg
{
public:
    KEstimateBkg(uint16_t _width, uint16_t _height, uint16_t _patch_size = 3)
        : m_iWidth(_width)
        , m_iHeight(_height)
        , m_iKernelSize(_patch_size % 2 == 0 ? _patch_size + 1 : _patch_size)
    {
    }

    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
    {
        int xBlocks = _width / m_iWidth;
        int yBlocks = _height / m_iHeight;

        if (_width % m_iWidth) xBlocks += 1;
        if (_height % m_iHeight) yBlocks += 1;
        // no gaussian filter
        //gaussian_filter(_image, _width, _height);

        int8_t *x_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
        if (nullptr == x_delta) return false;
        int8_t *y_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
        if (nullptr == y_delta){ delete[] x_delta;  return false; }
        int8_t * temp_ptr = y_delta;
        for (int16_t iX = -m_iKernelSize / 2; iX < m_iKernelSize / 2 + 1; ++iX)
        {
            for (int16_t iY = 0; iY < m_iKernelSize; ++iY) *temp_ptr++ = iX;
        }

        temp_ptr = x_delta;
        for (int16_t iX = 0; iX < m_iKernelSize; ++iX)
        {
            for (int16_t iY = -m_iKernelSize / 2; iY < m_iKernelSize / 2 + 1; ++iY) *temp_ptr++ = iY;
        }

        double * pAdjustTabel = new (std::nothrow) double[xBlocks * yBlocks];
        if (nullptr == pAdjustTabel){
            delete[] x_delta;
            delete[] y_delta;
            return false;
        }

        double * temp_image = new (std::nothrow) double[_width * _height];
        if (nullptr == temp_image){
            delete[] x_delta;
            delete[] y_delta;
            delete[] pAdjustTabel;
            return false;
        }

        for (int iXB = 0; iXB < xBlocks; ++iXB)
        {
            for (int iYB = 0; iYB < yBlocks; ++iYB)
            {
                int xStart = iXB * m_iWidth;
                int yStart = iYB * m_iHeight;

                int xWidth = m_iWidth;
                int yWidth = m_iHeight;

                double _power = 0.;
                double _sigma = 0.;
                double _min_value = _limit;

                if (xStart + m_iWidth > _width) xWidth = _width - xStart;
                if (yStart + m_iHeight > _height) yWidth = _height - yStart;

                for (int iY = yStart; iY < yStart + yWidth; ++iY)
                {
                    for (int iX = xStart; iX < xStart + xWidth; ++iX)
                    {
                        _power += _image[iY * _width + iX];
                        if(_min_value > _image[iY * _width + iX]) _min_value = _image[iY * _width + iX];
                    }
                }
                _power = _power / (yWidth * xWidth);

                for (int iY = yStart; iY < yStart + yWidth; ++iY)
                {
                    for (int iX = xStart; iX < xStart + xWidth; ++iX)
                    {
                        _sigma += std::pow((_image[iY * _width + iX] - _power), 2.);
                    }
                }

                _sigma /= (yWidth * xWidth);
                _sigma = std::pow(_sigma, .5f);

                pAdjustTabel[iYB * xBlocks + iXB] = static_cast<double>(std::max(_min_value, _power - 3 * _sigma));
            }
        }
        // make bilinear interpolation
        for (int16_t iRow = 0; iRow < _height; iRow++)
        {
            for (int16_t iColumn = 0; iColumn < _width; iColumn++)
            {
                float fVerBlock = static_cast<float>(iRow) / m_iHeight - .5f;
                float fHorBlock = static_cast<float>(iColumn) / m_iWidth - .5f;
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
                    temp_image[iRow * _width + iColumn] = static_cast<double>(pAdjustTabel[iVerBlock*xBlocks + iHorBlock] * (1 - fBeta) * (1 - fAlpha)
                        + pAdjustTabel[iVerBlock*xBlocks + iHorBlock + 1] * (1 - fBeta) * fAlpha
                        + pAdjustTabel[(iVerBlock + 1) * xBlocks + iHorBlock] * fBeta * (1 - fAlpha)
                        + pAdjustTabel[(iVerBlock + 1) * xBlocks + iHorBlock + 1] * fBeta * fAlpha);
                    //_image[iRow * _width + iColumn] = static_cast<uint16_t>(get_bilinear_value(pAdjustTabel[iVerBlock*xBlocks + iHorBlock], iIndex));
                }
                //   else{
            }
        }

        for (int16_t iRow = 0; iRow < _height; iRow++)
        {
            for (int16_t iColumn = 0; iColumn < _width; iColumn++)
            {
                temp_image[iRow * _width + iColumn] = _image[iRow * _width + iColumn] - temp_image[iRow * _width + iColumn];
            }
        }

        double * midfilter_image = new (std::nothrow) double[_width * _height];
        if (nullptr == midfilter_image){
            delete[] x_delta;
            delete[] y_delta;
            delete[] pAdjustTabel;
            delete[] temp_image;
            return false;
        }
        for (int16_t iY = 0; iY < _height; iY++)
        {
            for (int16_t iX = 0; iX < _width; iX++)
            {
                std::vector<double> pixel_buff;
                for (uint16_t index = 0; index < m_iKernelSize * m_iKernelSize; ++index)
                {
                    int16_t xpos = iX + x_delta[index];
                    int16_t ypos = iY + y_delta[index];
                    if (xpos < 0 || ypos < 0 || xpos > _width - 1 || ypos > _height - 1) continue;
                    pixel_buff.push_back(temp_image[ypos * _width + xpos]);
                }
                std::sort(pixel_buff.begin(), pixel_buff.end());
                midfilter_image[iY * _width + iX] = pixel_buff[pixel_buff.size()/2];
            }
        }

        double global_min = (std::numeric_limits<double>::max)();
        double global_max = -(std::numeric_limits<double>::max)();
        [&]()
        {
            for (uint16_t iY = 0; iY < _height; ++iY)
            {
                for (uint16_t iX = 0; iX < _width; ++iX)
                {
                    if (midfilter_image[iY * _width + iX] > global_max) global_max = midfilter_image[iY * _width + iX];
                    if (midfilter_image[iY * _width + iX] < global_min) global_min = midfilter_image[iY * _width + iX];
                }
            }
        }();
        if(std::fabs(global_max - global_min) < DBL_EPSILON) global_max = global_min + FLT_EPSILON;

        for (uint16_t iY = 0; iY < _height; ++iY)
        {
            for (uint16_t iX = 0; iX < _width; ++iX)
            {
                _image[iY * _width + iX] = static_cast<uint16_t>(_limit * (midfilter_image[iY * _width + iX] - global_min) / (global_max - global_min));
            }
        }

        delete[] pAdjustTabel;
        delete[] temp_image;
        delete[] midfilter_image;
        delete[] x_delta;
        delete[] y_delta;
        return true;
    }

private:
    uint16_t m_iWidth;
    uint16_t m_iHeight;
    uint16_t m_iKernelSize;
};

#endif // KESTIMATEBKG

