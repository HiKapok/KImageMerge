#ifndef __KNAKA_ADJUST_H__
#define __KNAKA_ADJUST_H__

#include <cstdint>

class KNakaAdjust
{
public:
	KNakaAdjust(uint16_t _bright, uint16_t _patch_size = 3)
		: m_iBright(_bright)
		, m_iKernelSize(_patch_size % 2 == 0 ? _patch_size + 1 : _patch_size)
	{
	}

	~KNakaAdjust()
	{
	}

    // for each pixel: result = pixel / (pixel + mean)
    // where mean is the mean of the window which the pixel center in it
    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
	{
        _limit = _limit;
		int8_t *x_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == x_delta) return false;
		int8_t *y_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == y_delta){ delete[] x_delta;  return false; }

		double* mean_map = new (std::nothrow) double[_width * _height];
		if (nullptr == mean_map){ delete[] x_delta; delete[] y_delta; return false; }

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
				
				mean_map[iY * _width + iX] = _mean;
			}
		}

		for (uint16_t iY = 0; iY < _height; ++iY)
		{
			for (uint16_t iX = 0; iX < _width; ++iX)
			{
				double _mean = mean_map[iY * _width + iX];
                _image[iY * _width + iX] = static_cast<uint16_t>(_image[iY * _width + iX] * m_iBright / (_mean + _image[iY * _width + iX]));
			}
		}
		delete[] x_delta;
		delete[] y_delta;
		delete[] mean_map;
        return true;
	}

private:
	uint16_t m_iBright;
	uint16_t m_iKernelSize;
};

#endif /* __KNAKA_ADJUST_H__ */
