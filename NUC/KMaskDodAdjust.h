#ifndef __K_MASK_DOD_ADJUST_H__
#define __K_MASK_DOD_ADJUST_H__

#include <cstdint>

class KMaskDodAdjust
{
public:
    KMaskDodAdjust(uint16_t _bright, float _scale_param, double _sigma, uint16_t _patch_size = 3)
		: m_iBright(_bright)
		, m_fScaleParam(_scale_param)
        , m_fSigma(_sigma)
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
					//m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2] = 1.f / std::pow((2 * M_PI * std::pow(delta, 2.f)),0.5) * std::exp(-1.f * (std::pow(iX, 2.f) + std::pow(iY, 2.f)) / (2 * std::pow(delta, 2.f)));
					m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2] = std::exp(-1.f * (std::pow(iX, 2.f) + std::pow(iY, 2.f)) / (2 * std::pow(delta, 2.f)));
					sum += m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2];
				}
			}
			for (int iY = 0; iY < m_iKernelSize; ++iY)
			{
				for (int iX = 0; iX < m_iKernelSize; ++iX) m_aGaussianMat[iY*m_iKernelSize + iX] = m_aGaussianMat[iY*m_iKernelSize + iX] / sum;
			}
        }(m_fSigma);//0.6
	}

	KMaskDodAdjust(const KMaskDodAdjust &rhs)
	{
        this->m_fScaleParam = rhs.m_fScaleParam;
        this->m_fSigma = rhs.m_fSigma;
		this->m_iBright = rhs.m_iBright;
		this->m_iKernelSize = rhs.m_iKernelSize;
		// just throw it
		this->m_aGaussianMat = new float[m_iKernelSize * m_iKernelSize];
		memcpy(this->m_aGaussianMat, rhs.m_aGaussianMat, m_iKernelSize * m_iKernelSize * sizeof(float));
	}

	KMaskDodAdjust & operator=(const KMaskDodAdjust &rhs){
        this->m_fScaleParam = rhs.m_fScaleParam;
        this->m_fSigma = rhs.m_fSigma;
		this->m_iBright = rhs.m_iBright;
		this->m_iKernelSize = rhs.m_iKernelSize;
		// just throw it
		this->m_aGaussianMat = new float[m_iKernelSize * m_iKernelSize];
		memcpy(this->m_aGaussianMat, rhs.m_aGaussianMat, m_iKernelSize * m_iKernelSize * sizeof(float));
		return *this;
	}

	~KMaskDodAdjust()
	{
		delete[] m_aGaussianMat;
	}

    void gaussian_filter(int32_t* _image, uint16_t _width, uint16_t _height, double _limit) const
	{
        _limit = _limit;
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
                    uint16_t image_temp = 0;
					if (xpos < 0 || ypos < 0 || xpos > _width - 1 || ypos > _height - 1)
					{
						// reduce the edge effect(const reflect boundary extend)
						image_temp = _image[iY * _width + iX];
						//continue;
                    }else image_temp = _image[ypos * _width + xpos];
					sum += m_aGaussianMat[index] * image_temp;
				}
				// there is no scale front 'sum' in the paper
                sum = (_image[iY * _width + iX] - 25*sum/25) + m_iBright;//
                //if (sum < 0.f) sum = 0.f;
                //if (sum > _limit) sum = _limit;
				_image[iY * _width + iX] = sum;
			}
		}

		delete[] x_delta;
		delete[] y_delta;
	}

    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
	{
        int32_t * temp_image = new (std::nothrow) int32_t[_width * _height];
        if(nullptr == temp_image) return false;

        for (int16_t iRow = 0; iRow < _height; iRow++)
        {
            for (int16_t iColumn = 0; iColumn < _width; iColumn++)
            {
                temp_image[iRow * _width + iColumn] = static_cast<int32_t>(_image[iRow * _width + iColumn]);
            }
        }
        gaussian_filter(temp_image, _width, _height, _limit);
		
		// no enhancement here but in the paper (subsection linear stretching method)
        if (m_fScaleParam > 0){
            for (int16_t iRow = 0; iRow < _height; iRow++)
            {
                for (int16_t iColumn = 0; iColumn < _width; iColumn++)
                {
                    int32_t temp_value = temp_image[iRow * _width + iColumn];

                    temp_value = _limit * (temp_value - m_fScaleParam) / (_limit - 2 * m_fScaleParam);
                    if (temp_value < 0.f) temp_value = 0.f;
                    if (temp_value > _limit)  temp_value = _limit;
                    _image[iRow * _width + iColumn] = static_cast<uint16_t>(temp_value);
                }
            }
        }
        else
        {
            for (int16_t iRow = 0; iRow < _height; iRow++)
            {
                for (int16_t iColumn = 0; iColumn < _width; iColumn++)
                {
                    int32_t temp_value = temp_image[iRow * _width + iColumn];
                    temp_value = temp_value * (_limit + 2 * m_fScaleParam) / _limit - m_fScaleParam;
                    if (temp_value < 0.f) temp_value = 0.f;
                    if (temp_value > _limit)  temp_value = _limit;
                    _image[iRow * _width + iColumn] = static_cast<uint16_t>(temp_value);
                }
            }
        }

        delete [] temp_image;
		return true;
	}
	
private:
	uint16_t m_iBright;
    float m_fScaleParam;
    double m_fSigma;
	uint16_t m_iKernelSize;
	float * m_aGaussianMat;
};

#endif /* __K_MASK_DOD_ADJUST_H__ */


