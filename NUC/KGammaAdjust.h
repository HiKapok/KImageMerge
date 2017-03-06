#ifndef __K_GAMMA_ADJUST_H__
#define __K_GAMMA_ADJUST_H__

#include <cstdint>

class KGammaAdjust
{
public:
	KGammaAdjust(uint16_t _bright, uint16_t _patch_size = 3)
		: m_iBright(_bright)
		, m_iKernelSize(_patch_size % 2 == 0 ? _patch_size + 1 : _patch_size)
	{
		[&](float delta){
			float sum = 0.f;
            sum = sum;
			m_aGaussianMat = new (std::nothrow) float[m_iKernelSize * m_iKernelSize];
			if (nullptr == m_aGaussianMat) return;
			for (int iY = -m_iKernelSize / 2; iY <= m_iKernelSize / 2; ++iY)
			{
				for (int iX = -m_iKernelSize / 2; iX <= m_iKernelSize / 2; ++iX)
				{
					m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2] = 1.f / std::pow((2 * 3.1415926 * std::pow(delta, 2.f)),0.5) * std::exp(-1.f * (std::pow(iX, 2.f) + std::pow(iY, 2.f)) / (2 * std::pow(delta, 2.f)));
					//m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2] = std::exp(-1.f * (std::pow(iX, 2.f) + std::pow(iY, 2.f)) / (2 * std::pow(delta, 2.f)));
					//sum += m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2];
				}
			}
			//for (int iY = 0; iY < m_iKernelSize; ++iY)
			//{
				//for (int iX = 0; iX < m_iKernelSize; ++iX) m_aGaussianMat[iY*m_iKernelSize + iX] = m_aGaussianMat[iY*m_iKernelSize + iX] / sum;
			//}
		}(0.6);//0.6
	}
    // this is a non-POD class
	KGammaAdjust(const KGammaAdjust &rhs)
	{
		this->m_iBright = rhs.m_iBright;
		this->m_iKernelSize = rhs.m_iKernelSize;
		// just throw it
		this->m_aGaussianMat = new float[m_iKernelSize * m_iKernelSize];
		memcpy(this->m_aGaussianMat, rhs.m_aGaussianMat, m_iKernelSize * m_iKernelSize * sizeof(float));
	}

	KGammaAdjust & operator=(const KGammaAdjust &rhs){
		this->m_iBright = rhs.m_iBright;
		this->m_iKernelSize = rhs.m_iKernelSize;
		// just throw it
		this->m_aGaussianMat = new float[m_iKernelSize * m_iKernelSize];
		memcpy(this->m_aGaussianMat, rhs.m_aGaussianMat, m_iKernelSize * m_iKernelSize * sizeof(float));
		return *this;
	}

	~KGammaAdjust()
	{
		delete[] m_aGaussianMat;
	}

	void gaussian_filter(uint16_t* _image, uint16_t _width, uint16_t _height) const
	{
		int8_t *x_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == x_delta) return;
		int8_t *y_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == y_delta){ delete[] x_delta;  return; }

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
				_image[iY * _width + iX] = _image[iY * _width + iX] - sum;
			}
		}

		delete[] x_delta;
		delete[] y_delta;
	}
    // for each pixel: result = MAX_VALUE * (pixel/MAX_VALUE)^( 0.5^( (pixel - mean)/mean ) )
    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
	{
		//gaussian_filter(_image, _width, _height);
		
		for (int16_t iRow = 0; iRow < _height; iRow++)
		{
			for (int16_t iColumn = 0; iColumn < _width; iColumn++)
			{
				//6.0
				float tempValue = static_cast<float>(_image[iRow * _width + iColumn]);

                float _gamma = pow(.5f, (tempValue - m_iBright) / m_iBright);
                _image[iRow * _width + iColumn] = static_cast<uint16_t>(_limit * std::pow(tempValue / _limit, _gamma));
                continue;

				if (tempValue < 32768){
					float _gamma = pow(6.f, (tempValue - m_iBright) / m_iBright);
					//float _gamma2 = pow(4.3f, (65535. - tempValue - m_iBright) / m_iBright);
					_image[iRow * _width + iColumn] = static_cast<uint16_t>(65535.f * pow(tempValue / 65535.f, _gamma));
				}
				else
				{
					//tempValue = 65536 - tempValue;
					float _gamma = pow(6.f, (tempValue - m_iBright) / m_iBright);
					//float _gamma2 = pow(4.3f, (65535. - tempValue - m_iBright) / m_iBright);
					_image[iRow * _width + iColumn] = static_cast<uint16_t>(65535.f * pow(tempValue / 65535.f, _gamma));
				}
			}
		}

		return true;
	}
	
private:
	uint16_t m_iBright;
	uint16_t m_iKernelSize;
	float * m_aGaussianMat;
};

#endif /* __K_GAMMA_ADJUST_H__ */

//
//#ifndef __K_GAMMA_ADJUST_H__
//#define __K_GAMMA_ADJUST_H__
//
//#include "KRawDblIntImg.h"
//
//class KGammaAdjust
//{
//public:
//	KGammaAdjust(uint16_t _bright, uint16_t _patch_size = 3)
//		: m_iBright(_bright)
//		, m_iKernelSize(_patch_size % 2 == 0 ? _patch_size + 1 : _patch_size)
//	{
//		[&](float delta){
//			float sum = 0.f;
//			m_aGaussianMat = new (std::nothrow) float[m_iKernelSize * m_iKernelSize];
//			if (nullptr == m_aGaussianMat) return;
//			for (int iY = -m_iKernelSize / 2; iY <= m_iKernelSize / 2; ++iY)
//			{
//				for (int iX = -m_iKernelSize / 2; iX <= m_iKernelSize / 2; ++iX)
//				{
//					m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2] = 1.f / std::pow((2 * 3.1415926 * std::pow(delta, 2.f)), 0.5) * std::exp(-1.f * (std::pow(iX, 2.f) + std::pow(iY, 2.f)) / (2 * std::pow(delta, 2.f)));
//					//m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2] = std::exp(-1.f * (std::pow(iX, 2.f) + std::pow(iY, 2.f)) / (2 * std::pow(delta, 2.f)));
//					//sum += m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2];
//				}
//			}
//			//for (int iY = 0; iY < m_iKernelSize; ++iY)
//			//{
//			//for (int iX = 0; iX < m_iKernelSize; ++iX) m_aGaussianMat[iY*m_iKernelSize + iX] = m_aGaussianMat[iY*m_iKernelSize + iX] / sum;
//			//}
//		}(0.6);//0.6
//	}
//
//	KGammaAdjust(const KGammaAdjust &rhs)
//	{
//		this->m_iBright = rhs.m_iBright;
//		this->m_iKernelSize = rhs.m_iKernelSize;
//		// just throw it
//		this->m_aGaussianMat = new float[m_iKernelSize * m_iKernelSize];
//		memcpy(this->m_aGaussianMat, rhs.m_aGaussianMat, m_iKernelSize * m_iKernelSize * sizeof(float));
//	}
//
//	KGammaAdjust & operator=(const KGammaAdjust &rhs){
//		this->m_iBright = rhs.m_iBright;
//		this->m_iKernelSize = rhs.m_iKernelSize;
//		// just throw it
//		this->m_aGaussianMat = new float[m_iKernelSize * m_iKernelSize];
//		memcpy(this->m_aGaussianMat, rhs.m_aGaussianMat, m_iKernelSize * m_iKernelSize * sizeof(float));
//		return *this;
//	}
//
//	~KGammaAdjust()
//	{
//		delete[] m_aGaussianMat;
//	}
//
//	void gaussian_filter(uint16_t* _image, uint16_t _width, uint16_t _height) const
//	{
//		int8_t *x_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
//		if (nullptr == x_delta) return;
//		int8_t *y_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
//		if (nullptr == y_delta){ delete[] x_delta;  return; }
//
//		int8_t * temp_ptr = x_delta;
//		for (int16_t iX = -m_iKernelSize / 2; iX < m_iKernelSize / 2 + 1; ++iX)
//		{
//			for (int16_t iY = 0; iY < m_iKernelSize; ++iY) *temp_ptr++ = iX;
//		}
//
//		temp_ptr = y_delta;
//		for (int16_t iX = 0; iX < m_iKernelSize; ++iX)
//		{
//			for (int16_t iY = -m_iKernelSize / 2; iY < m_iKernelSize / 2 + 1; ++iY) *temp_ptr++ = iY;
//		}
//
//		for (uint16_t iY = 0; iY < _height; ++iY)
//		{
//			for (uint16_t iX = 0; iX < _width; ++iX)
//			{
//				float sum = 0.f;
//				for (uint16_t index = 0; index < m_iKernelSize * m_iKernelSize; ++index)
//				{
//					int16_t xpos = iX + x_delta[index];
//					int16_t ypos = iY + y_delta[index];
//					if (xpos < 0 || ypos < 0 || xpos > _width - 1 || ypos > _height - 1) continue;
//					sum += m_aGaussianMat[index] * _image[ypos * _width + xpos];
//				}
//				_image[iY * _width + iX] = _image[iY * _width + iX] - sum;
//			}
//		}
//
//		delete[] x_delta;
//		delete[] y_delta;
//	}
//
//	bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, KRawDblIntImg<KGammaAdjust>* _ptr = nullptr) const
//	{
//		gaussian_filter(_image, _width, _height);
//
//		for (int16_t iRow = 0; iRow < _height; iRow++)
//		{
//			for (int16_t iColumn = 0; iColumn < _width; iColumn++)
//			{
//				//6.0
//				float tempValue = static_cast<float>(_image[iRow * _width + iColumn]);
//
//				if (tempValue < 32768){
//					float _gamma = pow(6.f, (tempValue - m_iBright) / m_iBright);
//					//float _gamma2 = pow(4.3f, (65535. - tempValue - m_iBright) / m_iBright);
//					//_image[iRow * _width + iColumn] = static_cast<uint16_t>(65535.f * pow(tempValue / 65535.f, _gamma));
//				}
//				else
//				{
//					tempValue = 65536 - tempValue;
//					float _gamma = pow(6.f, (tempValue - m_iBright) / m_iBright);
//					//float _gamma2 = pow(4.3f, (65535. - tempValue - m_iBright) / m_iBright);
//					//_image[iRow * _width + iColumn] = static_cast<uint16_t>(65535.f * pow(tempValue / 65535.f, _gamma));
//				}
//			}
//		}
//		if (_ptr) _ptr->save_image(_image, true);
//
//		return true;
//	}
//
//private:
//	uint16_t m_iBright;
//	uint16_t m_iKernelSize;
//	float * m_aGaussianMat;
//};
//
//#endif /* __K_GAMMA_ADJUST_H__ */
