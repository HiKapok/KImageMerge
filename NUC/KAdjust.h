#ifndef __KADJUST_H__
#define __KADJUST_H__

#include <cstdint>

class KAdjust
{
public:
	KAdjust(uint16_t _bright, uint16_t _width, uint16_t _height, uint16_t _patch_size = 3)
		: m_iBright(_bright)
		, m_iWidth(_width)
		, m_iHeight(_height)
		, m_iKernelSize(_patch_size % 2 == 0 ? _patch_size + 1 : _patch_size)
	{
		[&](float delta){
			float sum = 0.f;
			m_aGaussianMat = new (std::nothrow) float[m_iKernelSize * m_iKernelSize];
			if (nullptr == m_aGaussianMat) return;
			for (int iY = -m_iKernelSize / 2; iY <= m_iKernelSize / 2; ++iY)
			{
				for (int iX = -m_iKernelSize / 2; iX <= m_iKernelSize / 2; ++iX)
				{
					m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2] = std::exp(-1.f * (std::pow(iX, 2.f) + std::pow(iY, 2.f)) / (2 * std::pow(delta, 2.f)));
					sum += m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2];
				}
			}
			for (int iY = 0; iY < m_iKernelSize; ++iY)
			{
				for (int iX = 0; iX < m_iKernelSize; ++iX) m_aGaussianMat[iY*m_iKernelSize + iX] = m_aGaussianMat[iY*m_iKernelSize + iX] / sum;
			}
		}(0.6);
	}
    // this is a non-POD class
	KAdjust(const KAdjust &rhs)
	{
		this->m_iBright = rhs.m_iBright;
		this->m_iWidth = rhs.m_iWidth;
		this->m_iHeight = rhs.m_iHeight;
		this->m_iKernelSize = rhs.m_iKernelSize;
		// just throw it
		this->m_aGaussianMat = new float[m_iKernelSize * m_iKernelSize];
		memcpy(this->m_aGaussianMat, rhs.m_aGaussianMat, m_iKernelSize * m_iKernelSize * sizeof(float));
	}

	KAdjust & operator=(const KAdjust &rhs){
		this->m_iBright = rhs.m_iBright;
		this->m_iWidth = rhs.m_iWidth;
		this->m_iHeight = rhs.m_iHeight;
		this->m_iKernelSize = rhs.m_iKernelSize;
		// just throw it
		this->m_aGaussianMat = new float[m_iKernelSize * m_iKernelSize];
		memcpy(this->m_aGaussianMat, rhs.m_aGaussianMat, m_iKernelSize * m_iKernelSize * sizeof(float));
		return *this;
	}

	~KAdjust()
	{
		delete[] m_aGaussianMat;
	}
    // no help here
	void gaussian_filter(uint16_t* _image, uint16_t _width, uint16_t _height) const
	{
		int8_t *x_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == x_delta) return;
		int8_t *y_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == y_delta){ delete [] x_delta;  return; }

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
				float sum = 0.f;
				for (uint16_t index = 0; index < m_iKernelSize * m_iKernelSize; ++index)
				{
					int16_t xpos = iX + x_delta[index];
					int16_t ypos = iY + y_delta[index];
					if (xpos < 0 || ypos < 0 || xpos > _width - 1 || ypos > _height - 1) continue;
					sum += m_aGaussianMat[index] * _image[ypos * _width + xpos];
				}
                _image[iY * _width + iX] = static_cast<uint16_t>(sum);
			}
		}
		
		delete[] x_delta;
		delete[] y_delta;
	}
    // for each block: result = exp( (log(bright/MAX_VALUE) )/( log(average/MAX_VALUE) ) * log(pixel / MAX_VALUE) ) * MAX_VALUE
    // then bilinear interpolation is made to reduce the block influence
    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
	{
		int xBlocks = _width / m_iWidth;
		int yBlocks = _height / m_iHeight;

		if (_width % m_iWidth) xBlocks += 1;
		if (_height % m_iHeight) yBlocks += 1;
		// no gaussian filter
		//gaussian_filter(_image, _width, _height);

		float * pAdjustTabel = new (std::nothrow) float[xBlocks * yBlocks];
		if (nullptr == pAdjustTabel) return false;

		for (int iXB = 0; iXB < xBlocks; ++iXB)
		{
			for (int iYB = 0; iYB < yBlocks; ++iYB)
			{
				int xStart = iXB * m_iWidth;
				int yStart = iYB * m_iHeight;

				int xWidth = m_iWidth;
				int yWidth = m_iHeight;

				float _power = 0.f;

				if (xStart + m_iWidth > _width) xWidth = _width - xStart;
				if (yStart + m_iHeight > _height) yWidth = _height - yStart;

				for (int iY = yStart; iY < yStart + yWidth; ++iY)
				{
					for (int iX = xStart; iX < xStart + xWidth; ++iX)
					{
						_power += _image[iY * _width + iX];
					}
				}
				_power = _power / (yWidth * xWidth);
                pAdjustTabel[iYB * xBlocks + iXB] = static_cast<float>(log(m_iBright / _limit) / log(_power / _limit));
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

				uint16_t iIndex = _image[iRow * _width + iColumn];

				if (iVerBlock >= 0 && iVerBlock < yBlocks - 1 && iHorBlock >= 0 && iHorBlock < xBlocks - 1)
				{
                    _image[iRow * _width + iColumn] = static_cast<uint16_t>(get_bilinear_value(pAdjustTabel[iVerBlock*xBlocks + iHorBlock], iIndex, _limit) * (1 - fBeta) * (1 - fAlpha)
                        + get_bilinear_value(pAdjustTabel[iVerBlock*xBlocks + iHorBlock + 1], iIndex, _limit) * (1 - fBeta) * fAlpha
                        + get_bilinear_value(pAdjustTabel[(iVerBlock + 1) * xBlocks + iHorBlock], iIndex, _limit) * fBeta * (1 - fAlpha)
                        + get_bilinear_value(pAdjustTabel[(iVerBlock + 1) * xBlocks + iHorBlock + 1], iIndex, _limit) * fBeta * fAlpha);
					//_image[iRow * _width + iColumn] = static_cast<uint16_t>(get_bilinear_value(pAdjustTabel[iVerBlock*xBlocks + iHorBlock], iIndex));
				}
				//   else{
			}
		}
        delete[] pAdjustTabel;
		return true;
	}
	
private:
	uint16_t m_iBright;
	uint16_t m_iWidth;
	uint16_t m_iHeight;
	uint16_t m_iKernelSize;
	float * m_aGaussianMat;
    inline double get_bilinear_value(uint32_t _value, uint16_t _index, double _limit) const
	{
		if (_index == 0) return 0.f;
		//return (exp(static_cast<double>(_value)* pow(2.7, static_cast<double>(_index) / 65535.)) * 65535.);
        return (exp(static_cast<double>(_value)* log(static_cast<double>(_index) / _limit)) * _limit);
	}
};

#endif /* __KADJUST_H__ */
