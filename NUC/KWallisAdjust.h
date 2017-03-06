#ifndef __KWALLIS_ADJUST_H__
#define __KWALLIS_ADJUST_H__

#include <cstdint>
#include <cfloat>

class KWallisAdjust
{
public:
    KWallisAdjust(double _mean, double _sigma, uint16_t _patch_size = 3)
        : m_iKernelSize(_patch_size % 2 == 0 ? _patch_size + 1 : _patch_size),
          m_fTargetMean(_mean),
          m_fTargetSigma(_sigma)
	{
	}

	~KWallisAdjust()
	{
	}

    // for each pixel: result = (pixel - mean)*_sigma/sigma + _mean
    // where the _mean and _sigma is the mean and sigma of the block which the _mean is max
    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
	{
        double _sigma_f = 0.;
        double _mean_f = -(std::numeric_limits<double>::max)();

		int8_t *x_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == x_delta) return false;
		int8_t *y_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == y_delta){ delete[] x_delta;  return false; }

        double* image2scale = new (std::nothrow) double[_width * _height];
        if (nullptr == image2scale){ delete[] x_delta; delete[] y_delta; return false; }
        memset(image2scale, 0, sizeof(double) * _width * _height);

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

        double* mean_map = new (std::nothrow) double[_width * _height];
        if (nullptr == mean_map){ delete[] x_delta; delete[] y_delta; delete[] image2scale; return false; }

        double* sigma_map = new (std::nothrow) double[_width * _height];
        if (nullptr == sigma_map){ delete[] x_delta; delete[] y_delta; delete[] image2scale; delete[] mean_map; return false; }
        //memcpy(cpy_image, _image, sizeof(T) *_width * _height);


        // calculate the global mean and sigma map
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
                _sigma = std::pow(_sigma, .5f);

                mean_map[iY * _width + iX] = _mean;
                sigma_map[iY * _width + iX] = _sigma;
            }
        }

//		uint32_t max_pos = 0;
//        uint32_t _length = static_cast<uint32_t>(_width * _height);
//        for (uint32_t index = 0; index < _length; ++index)
//		{
//            if (_mean_f < mean_map[index]){
//				_mean_f = mean_map[index];
//				max_pos = index;
//			}
//		}
		
////        _mean_f = 0;
////        for (uint16_t iY = 80; iY < 170; ++iY)
////        {
////            for (uint16_t iX = 100; iX < 250; ++iX)
////            {
////                uint16_t index = iY*_width + iX;
////                if (_mean_f < mean_map[index]){
////                    _mean_f = mean_map[index];
////                    max_pos = index;
////                }
////            }
////        }

//		uint16_t num_valid_count = 0;
//		for (uint16_t index = 0; index < m_iKernelSize * m_iKernelSize; ++index)
//		{
//			int16_t xpos = max_pos % _width + x_delta[index];
//			int16_t ypos = max_pos / _width + y_delta[index];
//			if (xpos < 0 || ypos < 0 || xpos > _width - 1 || ypos > _height - 1) continue;
//			_sigma_f += std::pow((_image[ypos * _width + xpos] - _mean_f), 2.);
//			++num_valid_count;
//		}
//		if (num_valid_count) _sigma_f /= num_valid_count;
//		_sigma_f = std::pow(_sigma_f, .5f);

//        qDebug()<<_sigma_f<<_mean_f<<max_pos;

        //_sigma_f = 2605.92; _mean_f=60432.7;
        _sigma_f = m_fTargetSigma;
        _mean_f=m_fTargetMean;
        qDebug()<<_sigma_f<<_mean_f;
		for (uint16_t iY = 0; iY < _height; ++iY)
		{
			for (uint16_t iX = 0; iX < _width; ++iX)
			{
				double _mean_g = mean_map[iY * _width + iX];
				double _sigma_g = sigma_map[iY * _width + iX];
                //qDebug()<<_mean_f;
               // qDebug()<<_sigma_f;
                if (_sigma_g < DBL_EPSILON) _sigma_g = DBL_EPSILON;
                //if (_sigma_f < DBL_EPSILON) _sigma_f = DBL_EPSILON;
                double temp_value = (_image[iY * _width + iX] - _mean_g) * (_sigma_f / _sigma_g) + _mean_f;
                //qDebug()<<temp_value;
                image2scale[iY * _width + iX] = temp_value;
//                if (temp_value < 0.) temp_value = 0.;
//				if (temp_value > _limit) temp_value = _limit;
//                _image[iY * _width + iX] = static_cast<uint16_t>(temp_value);
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
                    if (image2scale[iY * _width + iX] > global_max) global_max = image2scale[iY * _width + iX];
                    if (image2scale[iY * _width + iX] < global_min) global_min = image2scale[iY * _width + iX];
                }
            }
        }();
        if(std::fabs(global_max - global_min) < FLT_EPSILON) global_max = global_min + FLT_EPSILON;

        for (uint16_t iY = 0; iY < _height; ++iY)
        {
            for (uint16_t iX = 0; iX < _width; ++iX)
            {
                double temp_value = image2scale[iY * _width + iX];
                if (temp_value < 0.) temp_value = 0.;
                if (temp_value > _limit) temp_value = _limit;
                _image[iY * _width + iX] = static_cast<uint16_t>(temp_value);
                //_image[iY * _width + iX] = _limit * (image2scale[iY * _width + iX] - global_min) / (global_max - global_min);
                //_image[iY * _width + iX] = image2scale[iY * _width + iX];
            }
        }

		delete[] x_delta;
		delete[] y_delta;
        //delete[] mean_map;
        //delete[] sigma_map;
        delete[] image2scale;
        return true;
	}

private:
	uint16_t m_iKernelSize;
    double m_fTargetMean;
    double m_fTargetSigma;
};

#endif /* __KWALLIS_ADJUST_H__ */
