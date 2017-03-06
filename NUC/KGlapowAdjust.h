#ifndef __KGLAPOW_ADJUST_H__
#define __KGLAPOW_ADJUST_H__

#include <cstdint>
#include <cfloat>

class KGlapowAdjust
{
public:
    KGlapowAdjust(uint16_t _bright, uint16_t _patch_size = 3)
		: m_iBright(_bright)
		, m_iKernelSize(_patch_size % 2 == 0 ? _patch_size + 1 : _patch_size)
	{
	}

	~KGlapowAdjust()
	{
	}

    // for each pixel: result = MAX_VALUE * ( fi(pixel) - fi(global_min) ) / ( fi(global_max) - fi(global_min) )
    // where fi(x) = 1 / ( 1 + exp( (mean - x)/sigma ) )
    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
	{
		int8_t *x_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == x_delta) return false;
		int8_t *y_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
		if (nullptr == y_delta){ delete[] x_delta;  return false; }

		float* image2scale = new (std::nothrow) float[_width * _height];
		if (nullptr == image2scale){ delete[] x_delta; delete[] y_delta; return false; }
		memset(image2scale, 0, sizeof(float) * _width * _height);

		uint32_t* add_count = new (std::nothrow) uint32_t[_width * _height];
		if (nullptr == add_count){ delete[] x_delta; delete[] y_delta; delete[] image2scale; return false; }
		memset(add_count, 0, sizeof(uint32_t) * _width * _height);

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

		float global_min = (std::numeric_limits<float>::max)();
		float global_max = -(std::numeric_limits<float>::max)();
		[&]()
		{
			for (uint16_t iY = 0; iY < _height; ++iY)
			{
				for (uint16_t iX = 0; iX < _width; ++iX)
				{
					if (_image[iY * _width + iX] > global_max) global_max = _image[iY * _width + iX];
					if (_image[iY * _width + iX] < global_min) global_min = _image[iY * _width + iX];
				}
			}
		}();
        // this is the function BIG fi
		auto get_fi = [&](float _value, float _mean, float _sigma) -> float
		{
			if (_sigma < FLT_EPSILON)
			{
				if (_value >= _mean) return 1.f;
				return 0.f;
			}
            return static_cast<float>(1. / (1. + std::exp((_mean - _value) / _sigma)));
		};

        uint16_t iYStart = (_height % m_iKernelSize) / 2;
        uint16_t iXStart = (_width % m_iKernelSize) / 2;
        for (uint16_t iY = iYStart; iY < _height; iY += m_iKernelSize)
		{
            for (uint16_t iX = iXStart; iX < _width; iX+=m_iKernelSize)
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

				float fi_xmin = get_fi(global_min, _mean, _sigma);
				float fi_xmax = get_fi(global_max, _mean, _sigma);
				//float fi_curpoint = get_fi(_image[iY * _width + iX], _mean, _sigma);
                if(std::fabs(fi_xmax - fi_xmin) < FLT_EPSILON) fi_xmax = fi_xmin + FLT_EPSILON;
				for (uint16_t index = 0; index < m_iKernelSize * m_iKernelSize; ++index)
				{
					int16_t xpos = iX + x_delta[index];
					int16_t ypos = iY + y_delta[index];
					if (xpos < 0 || ypos < 0 || xpos > _width - 1 || ypos > _height - 1) continue;
					float fi_curpoint = get_fi(_image[ypos * _width + xpos], _mean, _sigma);
					float temp_value = _limit * (fi_curpoint - fi_xmin) / (fi_xmax - fi_xmin);
					//if (temp_value < 0.f) temp_value = 0.f;
					//if (temp_value > _limit)  temp_value = _limit;
					image2scale[ypos * _width + xpos] += temp_value;
					add_count[ypos * _width + xpos]++;
				}
			}
		}

		for (uint16_t iY = 0; iY < _height; ++iY)
		{
			for (uint16_t iX = 0; iX < _width; ++iX)
			{
				if (add_count[iY * _width + iX]) image2scale[iY * _width + iX] /= add_count[iY * _width + iX];
			}
		}

		global_min = (std::numeric_limits<float>::max)();
		global_max = -(std::numeric_limits<float>::max)();
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
                _image[iY * _width + iX] = _limit * (image2scale[iY * _width + iX] - global_min) / (global_max - global_min);
                //_image[iY * _width + iX] = image2scale[iY * _width + iX];
			}
		}

		delete[] x_delta;
		delete[] y_delta;
		delete[] image2scale;
		delete[] add_count;
        return true;
	}

private:
	uint16_t m_iBright;
	uint16_t m_iKernelSize;
};

#endif /* __KGLAPOW_ADJUST_H__ */


/*template <typename T = uint16_t>
bool operator()(T* _image, uint16_t _width, uint16_t _height, KRawDblIntImg<>* _ptr = nullptr) const
{
int8_t *x_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
if (nullptr == x_delta) return false;
int8_t *y_delta = new (std::nothrow) int8_t[m_iKernelSize * m_iKernelSize];
if (nullptr == y_delta){ delete[] x_delta;  return false; }

float* image2scale = new (std::nothrow) float[_width * _height];
if (nullptr == image2scale){ delete[] x_delta; delete[] y_delta; return false; }

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

float global_min = (std::numeric_limits<float>::max)();
float global_max = -(std::numeric_limits<float>::max)();
[&]()
{
for (uint16_t iY = 0; iY < _height; ++iY)
{
for (uint16_t iX = 0; iX < _width; ++iX)
{
if (_image[iY * _width + iX] > global_max) global_max = _image[iY * _width + iX];
if (_image[iY * _width + iX] < global_min) global_min = _image[iY * _width + iX];
}
}
}();

auto get_fi = [&](float _value, float _mean, float _sigma) -> float
{
return static_cast<float>(1. / (1. - std::exp((_mean - _value) / _sigma)));
};

float _limit = 255.f;
if (typeid(T) == typeid(uint16_t)){
_limit = 65535.f;
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
_sigma = std::pow(_sigma, .5f);

float fi_xmin = get_fi(global_min, _mean, _sigma);
float fi_xmax = get_fi(global_max, _mean, _sigma);
float fi_curpoint = get_fi(_image[iY * _width + iX], _mean, _sigma);
float temp_value = _limit * (fi_curpoint - fi_xmin) / (fi_xmax - fi_xmin);
if (temp_value < 0.f) temp_value = 0.f;
if (temp_value > _limit)  temp_value = _limit;
image2scale[iY * _width + iX] = static_cast<double>(temp_value);
}
}

global_min = (std::numeric_limits<float>::max)();
global_max = -(std::numeric_limits<float>::max)();
[&]()
{
for (uint16_t iY = 0; iY < _height; ++iY)
{
for (uint16_t iX = 0; iX < _width; ++iX)
{
if (image2scale[iY * _width + iX] > global_max) global_max = _image[iY * _width + iX];
if (image2scale[iY * _width + iX] < global_min) global_min = _image[iY * _width + iX];
}
}
}();

for (uint16_t iY = 0; iY < _height; ++iY)
{
for (uint16_t iX = 0; iX < _width; ++iX)
{
_image[iY * _width + iX] = _limit * (image2scale[iY * _width + iX] - global_min) / (global_max - global_min);
}
}

delete[] x_delta;
delete[] y_delta;
delete[] image2scale;
if (_ptr && typeid(T) == typeid(uint16_t)) _ptr->save_image(reinterpret_cast<uint16_t*>(_image), true);
return true;
}
*/
