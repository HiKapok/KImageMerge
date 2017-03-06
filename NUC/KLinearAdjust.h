#ifndef __KLINEAR_ADJUST_H__
#define __KLINEAR_ADJUST_H__

#include <cstdint>

class KLinearAdjust
{
public:
    KLinearAdjust(uint16_t _bright, uint16_t _patch_size = 3)
		: m_iBright(_bright)
		, m_iKernelSize(_patch_size % 2 == 0 ? _patch_size + 1 : _patch_size)
	{
	}

	~KLinearAdjust()
	{
	}
    // make the local block's variance to 1
    // for each block: result = ( pixel - mean )/( sigma + 0.1 ) * bright + bright
    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
	{
        _limit = _limit;

		int8_t *x_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == x_delta) return false;
		int8_t *y_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == y_delta){ delete[] x_delta;  return false; }

        int32_t* cpy_image = new (std::nothrow) int32_t[_width * _height];
		if (nullptr == cpy_image){ delete[] x_delta; delete[] y_delta; return false; }
		//memcpy(cpy_image, _image, sizeof(uint16_t) *_width * _height);

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
		for (uint16_t iY = 0; iY < _height; ++iY)
		{
			for (uint16_t iX = 0; iX < _width; ++iX)
			{
				float _mean = 0.f;
				float _sigma = 0.f;
				uint32_t _count = 0;
				for (uint16_t index = 0; index < m_iKernelSize * m_iKernelSize; ++index)
				{
					int16_t xpos = iX + x_delta[index];
					int16_t ypos = iY + y_delta[index];
					if (xpos < 0 || ypos < 0 || xpos > _width - 1 || ypos > _height - 1) continue;
					_mean += _image[ypos * _width + xpos];
					++_count;
				}
				if (_count) _mean /= _count;

				for (uint16_t index = 0; index < m_iKernelSize * m_iKernelSize; ++index)
				{
					int16_t xpos = iX + x_delta[index];
					int16_t ypos = iY + y_delta[index];
					if (xpos < 0 || ypos < 0 || xpos > _width - 1 || ypos > _height - 1) continue;
					_sigma += std::pow((_image[ypos * _width + xpos] - _mean), 2.);
				}
				if (_count) _sigma /= _count;
				
                if (_sigma < 0.618){
                    cpy_image[iY * _width + iX] = m_iBright;
                    continue;
				}
                //float temp = ((_image[iY * _width + iX] - _mean) / (.1f + _sigma) * m_iBright + m_iBright);

                cpy_image[iY * _width + iX] = static_cast<int32_t>((_image[iY * _width + iX] - _mean) / (.1f + _sigma) * m_iBright + m_iBright);
				continue;
				_sigma = std::pow(_sigma, .2f);
				cpy_image[iY * _width + iX] = static_cast<uint16_t>(1 * (_image[iY * _width + iX] - _mean) / _sigma  + m_iBright);
			}
		}
        //memcpy(_image, cpy_image, sizeof(uint16_t) *_width * _height);
//        double global_min = (std::numeric_limits<double>::max)();
//        double global_max = -(std::numeric_limits<double>::max)();
//        [&]()
//        {
//            for (uint16_t iY = 0; iY < _height; ++iY)
//            {
//                for (uint16_t iX = 0; iX < _width; ++iX)
//                {
//                    if (cpy_image[iY * _width + iX] > global_max) global_max = cpy_image[iY * _width + iX];
//                    if (cpy_image[iY * _width + iX] < global_min) global_min = cpy_image[iY * _width + iX];
//                }
//            }
//        }();
        for (uint16_t iY = 0; iY < _height; ++iY)
        {
            for (uint16_t iX = 0; iX < _width; ++iX)
            {
                double temp_value = cpy_image[iY * _width + iX];//_limit * (cpy_image[iY * _width + iX] - global_min) / (global_max - global_min);
                if(temp_value<0.) temp_value=0.;
                if(temp_value>_limit) temp_value=_limit;
                _image[iY * _width + iX] = static_cast<uint16_t>(temp_value);
            }
        }
		delete[] x_delta;
		delete[] y_delta;
		delete[] cpy_image;
		return true;
	}

private:
	uint16_t m_iBright;
	uint16_t m_iKernelSize;
};

#endif /* __KLINEAR_ADJUST_H__ */


//
//int xBlocks = _width / m_iWidth;
//int yBlocks = _height / m_iHeight;
//
//if (_width % m_iWidth) xBlocks += 1;
//if (_height % m_iHeight) yBlocks += 1;
//// no gaussian filter
////gaussian_filter(_image, _width, _height);
//
//float * pMeanTabel = new (std::nothrow) float[xBlocks * yBlocks];
//if (nullptr == pMeanTabel) return false;
//
//float * pSigmaTabel = new (std::nothrow) float[xBlocks * yBlocks];
//if (nullptr == pSigmaTabel){
//	delete[] pMeanTabel;
//	return false;
//}
//for (int iXB = 0; iXB < xBlocks; ++iXB)
//{
//	for (int iYB = 0; iYB < yBlocks; ++iYB)
//	{
//		int xStart = iXB * m_iWidth;
//		int yStart = iYB * m_iHeight;
//
//		int xWidth = m_iWidth;
//		int yWidth = m_iHeight;
//
//		float _mean = 0.f;
//		float _sigma = 0.f;
//
//		if (xStart + m_iWidth > _width) xWidth = _width - xStart;
//		if (yStart + m_iHeight > _height) yWidth = _height - yStart;
//
//		for (int iY = yStart; iY < yStart + yWidth; ++iY)
//		{
//			for (int iX = xStart; iX < xStart + xWidth; ++iX)
//			{
//				_mean += _image[iY * _width + iX];
//			}
//		}
//		_mean = _mean / (yWidth * xWidth);
//
//		for (int iY = yStart; iY < yStart + yWidth; ++iY)
//		{
//			for (int iX = xStart; iX < xStart + xWidth; ++iX)
//			{
//				_sigma += std::pow((_image[iY * _width + iX] - _mean), 2.);
//			}
//		}
//		_sigma = _sigma / (yWidth * xWidth);
//
//		pMeanTabel[iYB * xBlocks + iXB] = _mean;
//		pSigmaTabel[iYB * xBlocks + iXB] = _sigma;
//	}
//}
//for (int16_t iRow = 0; iRow < _height; iRow++)
//{
//	for (int16_t iColumn = 0; iColumn < _width; iColumn++)
//	{
//		float fVerBlock = static_cast<float>(iRow) / m_iHeight - .5f;
//		float fHorBlock = static_cast<float>(iColumn) / m_iWidth - .5f;
//		uint16_t iVerBlock = static_cast<uint16_t>(fVerBlock);
//		uint16_t iHorBlock = static_cast<uint16_t>(fHorBlock);
//		float fAlpha = fHorBlock - iHorBlock;
//		float fBeta = fVerBlock - iVerBlock;
//
//		if (iVerBlock == yBlocks - 1)
//		{
//			iVerBlock--;
//			fBeta += 1;
//		}
//		if (iHorBlock == xBlocks - 1)
//		{
//			iHorBlock--;
//			fAlpha += 1;
//		}
//
//		uint16_t iIndex = _image[iRow * _width + iColumn];
//
//		if (iVerBlock >= 0 && iVerBlock < yBlocks - 1 && iHorBlock >= 0 && iHorBlock < xBlocks - 1)
//		{
//			_image[iRow * _width + iColumn] = static_cast<uint16_t>((_image[iRow * _width + iColumn] - pMeanTabel[iVerBlock*xBlocks + iHorBlock]) / (.1f + pSigmaTabel[iVerBlock*xBlocks + iHorBlock]) * m_iBright + m_iBright);
//
//		}
//		//   else{
//	}
//}
//if (_ptr) _ptr->save_image(_image, true);
//delete[] pSigmaTabel;
//delete[] pMeanTabel;
//
//return true;
