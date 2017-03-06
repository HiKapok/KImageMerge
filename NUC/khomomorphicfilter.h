#ifndef __K_HOMOMORPHIC_FILTER_H__
#define __K_HOMOMORPHIC_FILTER_H__

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <cstdint>
#include <complex>
#include <bitset>
#include <cmath>
#include "fftpf-impl.h"

class KHomomorphicFilter
{
public:
    KHomomorphicFilter(uint16_t _bright, double _gammaHigh = 3., double _gammaLow = .8, double _cutoff = 70., uint16_t _patch_size = 3)
        : m_iBright(_bright)
        , m_fGammaHigh(_gammaHigh)
        , m_fGammaLow(_gammaLow)
        , m_fCutOff(_cutoff)
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
                    //m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2] = 1.f / std::pow((2 * 3.1415926 * std::pow(delta, 2.f)),0.5) * std::exp(-1.f * (std::pow(iX, 2.f) + std::pow(iY, 2.f)) / (2 * std::pow(delta, 2.f)));
                    m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2] = std::exp(-1.f * (std::pow(iX, 2.f) + std::pow(iY, 2.f)) / (2 * std::pow(delta, 2.f)));
                    sum += m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2];
                }
            }
            for (int iY = 0; iY < m_iKernelSize; ++iY)
            {
                for (int iX = 0; iX < m_iKernelSize; ++iX) m_aGaussianMat[iY*m_iKernelSize + iX] = m_aGaussianMat[iY*m_iKernelSize + iX] / sum;
            }
        }(0.6);//0.6
    }

    KHomomorphicFilter(const KHomomorphicFilter &rhs)
    {
        this->m_iBright = rhs.m_iBright;
        this->m_iKernelSize = rhs.m_iKernelSize;
        // just throw it
        this->m_aGaussianMat = new float[m_iKernelSize * m_iKernelSize];
        memcpy(this->m_aGaussianMat, rhs.m_aGaussianMat, m_iKernelSize * m_iKernelSize * sizeof(float));
    }

    KHomomorphicFilter & operator=(const KHomomorphicFilter &rhs){
        this->m_iBright = rhs.m_iBright;
        this->m_iKernelSize = rhs.m_iKernelSize;
        // just throw it
        this->m_aGaussianMat = new float[m_iKernelSize * m_iKernelSize];
        memcpy(this->m_aGaussianMat, rhs.m_aGaussianMat, m_iKernelSize * m_iKernelSize * sizeof(float));
        return *this;
    }

    ~KHomomorphicFilter()
    {
        delete[] m_aGaussianMat;
    }

    void FFT(std::complex<double> * TD, std::complex<double> * FD, int r)
    {
        // 付立叶变换点数
        long	count;

        // 循环变量
        int		i, j, k;

        // 中间变量
        int		bfsize, p;

        // 角度
        double	angle;

        std::complex<double> *W, *X1, *X2, *X;

        // 计算付立叶变换点数
        count = 1 << r;

        // 分配运算所需存储器
        W = new std::complex<double>[count / 2];
        X1 = new std::complex<double>[count];
        X2 = new std::complex<double>[count];

        // 计算加权系数
        for (i = 0; i < count / 2; i++)
        {
            angle = -i * M_PI * 2 / count;
            W[i] = std::complex<double>(cos(angle), sin(angle));
        }

        // 将时域点写入X1
        memcpy(X1, TD, sizeof(std::complex<double>) * count);

        // 采用蝶形算法进行快速付立叶变换
        for (k = 0; k < r; k++)
        {
            for (j = 0; j < 1 << k; j++)
            {
                bfsize = 1 << (r - k);
                for (i = 0; i < bfsize / 2; i++)
                {
                    p = j * bfsize;
                    X2[i + p] = X1[i + p] + X1[i + p + bfsize / 2];
                    X2[i + p + bfsize / 2] = (X1[i + p] - X1[i + p + bfsize / 2]) * W[i * (1 << k)];
                }
            }
            X = X1;
            X1 = X2;
            X2 = X;
        }

        // 重新排序
        for (j = 0; j < count; j++)
        {
            p = 0;
            for (i = 0; i < r; i++)
            {
                if (j&(1 << i))
                {
                    p += 1 << (r - i - 1);
                }
            }
            FD[j] = X1[p];
        }

        // 释放内存
        delete[] W;
        delete[] X1;
        delete[] X2;
    }

    bool do_fft_one_dim(std::complex<double> *spatial_data, std::complex<double> *frequency_data, uint32_t level)
    {
        uint32_t num_points = static_cast<uint32_t>(std::pow(2.f, level));

        std::complex<double> * w_array = new (std::nothrow) std::complex<double>[num_points / 2];
        if (nullptr == w_array) return false;

        std::complex<double> * left_array = new (std::nothrow) std::complex<double>[num_points];
        if (nullptr == left_array){
            delete[] w_array;
            return false;
        }

        std::complex<double> * right_array = new (std::nothrow) std::complex<double>[num_points];
        if (nullptr == right_array){
            delete[] w_array;
            delete[] left_array;
            return false;
        }

        for (uint32_t index = 0; index < num_points / 2; ++index)
        {
            double theta = -2. * M_PI * index / num_points;
            w_array[index] = std::complex<double>(std::cos(theta), std::sin(theta));
            right_array[index] = spatial_data[index];
        }
        for (uint32_t index = num_points / 2; index < num_points; ++index)
        {
            right_array[index] = spatial_data[index];
        }
        // total level times iteration
        for (uint32_t _level = 0; _level < level; ++_level)
        {
            // delta is the pair num and the next level group num
            //std::cout << "level: " << _level << std::endl;
            uint32_t delta = static_cast<uint32_t>(std::pow(2., (level - _level - 1)));
            uint32_t num_each_group = static_cast<uint32_t>(std::pow(2., _level));

            std::swap(left_array, right_array);
            for (uint32_t pair_index = 0; pair_index < delta; ++pair_index)
            {
                for (uint32_t group_index = 0; group_index < num_each_group; ++group_index){
                    uint32_t this_index = pair_index + delta * 2 * group_index;
                    uint32_t inverse_group_index = [=]() -> uint32_t{
                        std::bitset<32> org_num(group_index);
                        std::bitset<32> real_num(0);
                        for (uint32_t bit_index = 0; bit_index < _level; ++bit_index)
                        {
                            real_num[_level - bit_index - 1] = org_num[bit_index];
                        }
                        return static_cast<uint32_t>(real_num.to_ulong());
                    }();
                    right_array[this_index] = left_array[this_index] + left_array[this_index + delta] * w_array[inverse_group_index * delta];
                    right_array[this_index + delta] = left_array[this_index] - left_array[this_index + delta] * w_array[inverse_group_index * delta];
                    //std::cout << "right[" << this_index << "] = " << "left[" << this_index << "] + " << "left[" << this_index + delta << "] * w[" << inverse_group_index * delta << "]" << std::endl;
                    //std::cout << "right[" << this_index + delta << "] = " << "left[" << this_index << "] - " << "left[" << this_index + delta << "] * w[" << inverse_group_index * delta << "]" << std::endl;
                }
            }
        }

        for (uint32_t index = 0; index < num_points; ++index)
        {
            std::bitset<32> org_num(index);
            std::bitset<32> real_num(0);
            for (uint32_t bit_index = 0; bit_index < level; ++bit_index)
            {
                real_num[level - bit_index - 1] = org_num[bit_index];
            }
            frequency_data[real_num.to_ulong()] = right_array[index];
        }

        delete[] w_array;
        delete[] left_array;
        delete[] right_array;
        return true;
    }

    bool do_dit_one_dim(std::complex<double> *spatial_data, std::complex<double> *frequency_data, uint32_t level)
    {
        uint32_t num_points = static_cast<uint32_t>(std::pow(2.f, level));

        std::complex<double> * w_array = new (std::nothrow) std::complex<double>[num_points / 2];
        if (nullptr == w_array) return false;

        std::complex<double> * left_array = new (std::nothrow) std::complex<double>[num_points];
        if (nullptr == left_array){
            delete[] w_array;
            return false;
        }

        std::complex<double> * right_array = new (std::nothrow) std::complex<double>[num_points];
        if (nullptr == right_array){
            delete[] w_array;
            delete[] left_array;
            return false;
        }

        for (uint32_t index = 0; index < num_points / 2; ++index)
        {
            double theta = -2. * M_PI * index / num_points;
            w_array[index] = std::complex<double>(std::cos(theta), std::sin(theta));
        }
        for (uint32_t index = 0; index < num_points; ++index)
        {
            std::bitset<32> org_num(index);
            std::bitset<32> real_num(0);
            for (uint32_t bit_index = 0; bit_index < level; ++bit_index)
            {
                real_num[level - bit_index - 1] = org_num[bit_index];
            }
            right_array[real_num.to_ulong()] = spatial_data[index];
        }

        // total level times iteration
        for (uint32_t _level = 0; _level < level; ++_level)
        {
            // delta is the pair num and the next level group num
            //std::cout << "level: " << _level << std::endl;
            uint32_t w_groups = static_cast<uint32_t>(std::pow(2., (level - _level - 1)));
            uint32_t num_each_group = static_cast<uint32_t>(std::pow(2., _level));

            std::swap(left_array, right_array);
            for (uint32_t pair_index = 0; pair_index < w_groups; ++pair_index)
            {
                for (uint32_t group_index = 0; group_index < num_each_group; ++group_index){
                    uint32_t this_index = 2 * num_each_group * pair_index + group_index;
                    right_array[this_index] = left_array[this_index] + left_array[this_index + num_each_group] * w_array[group_index * w_groups];
                    right_array[this_index + num_each_group] = left_array[this_index] - left_array[this_index + num_each_group] * w_array[group_index * w_groups];
                    //std::cout << "right[" << this_index << "] = " << "left[" << this_index << "] + " << "left[" << this_index + num_each_group << "] * w[" << inverse_group_index * w_groups << "]" << std::endl;
                    //std::cout << "right[" << this_index + num_each_group << "] = " << "left[" << this_index << "] - " << "left[" << this_index + num_each_group << "] * w[" << inverse_group_index * w_groups << "]" << std::endl;
                }
            }
        }

        for (uint32_t index = 0; index < num_points; ++index)
        {
            frequency_data[index] = right_array[index];
        }

        delete[] w_array;
        delete[] left_array;
        delete[] right_array;
        return true;
    }

    bool do_dif_one_dim(std::complex<double> *spatial_data, std::complex<double> *frequency_data, uint32_t level)
    {
        uint32_t num_points = static_cast<uint32_t>(std::pow(2.f, level));

        std::complex<double> * w_array = new (std::nothrow) std::complex<double>[num_points / 2];
        if (nullptr == w_array) return false;

        std::complex<double> * left_array = new (std::nothrow) std::complex<double>[num_points];
        if (nullptr == left_array){
            delete[] w_array;
            return false;
        }

        std::complex<double> * right_array = new (std::nothrow) std::complex<double>[num_points];
        if (nullptr == right_array){
            delete[] w_array;
            delete[] left_array;
            return false;
        }

        for (uint32_t index = 0; index < num_points / 2; ++index)
        {
            double theta = -2. * M_PI * index / num_points;
            w_array[index] = std::complex<double>(std::cos(theta), std::sin(theta));
            right_array[index] = spatial_data[index];
        }
        for (uint32_t index = num_points / 2; index < num_points; ++index)
        {
            right_array[index] = spatial_data[index];
        }

        // total level times iteration
        for (uint32_t _level = 0; _level < level; ++_level)
        {
            // delta is the pair num and the next level group num
            //std::cout << "level: " << _level << std::endl;
            uint32_t w_groups = static_cast<uint32_t>(std::pow(2., _level));
            uint32_t num_each_group = static_cast<uint32_t>(std::pow(2., (level - _level - 1)));

            std::swap(left_array, right_array);
            for (uint32_t pair_index = 0; pair_index < w_groups; ++pair_index)
            {
                for (uint32_t group_index = 0; group_index < num_each_group; ++group_index){
                    uint32_t this_index = 2 * num_each_group * pair_index + group_index;
                    right_array[this_index] = left_array[this_index] + left_array[this_index + num_each_group];// *w_array[group_index * w_groups];
                    right_array[this_index + num_each_group] = (left_array[this_index] - left_array[this_index + num_each_group]) * w_array[group_index * w_groups];
                    //std::cout << "right[" << this_index << "] = " << "left[" << this_index << "] + " << "left[" << this_index + num_each_group << "] * w[" << inverse_group_index * w_groups << "]" << std::endl;
                    //std::cout << "right[" << this_index + num_each_group << "] = " << "left[" << this_index << "] - " << "left[" << this_index + num_each_group << "] * w[" << inverse_group_index * w_groups << "]" << std::endl;
                }
            }
        }

        for (uint32_t index = 0; index < num_points; ++index)
        {
            std::bitset<32> org_num(index);
            std::bitset<32> real_num(0);
            for (uint32_t bit_index = 0; bit_index < level; ++bit_index)
            {
                real_num[level - bit_index - 1] = org_num[bit_index];
            }
            frequency_data[real_num.to_ulong()] = right_array[index];
        }

        delete[] w_array;
        delete[] left_array;
        delete[] right_array;
        return true;
    }

    bool do_ifft_one_dim(std::complex<double> *frequency_data, std::complex<double> *spatial_data, uint32_t level)
    {
        uint32_t num_points = static_cast<uint32_t>(std::pow(2.f, level));

        std::complex<double> * inv_frequency_data = new (std::nothrow) std::complex<double>[num_points];
        if (nullptr == inv_frequency_data) return false;

        for (uint32_t index = 0; index < num_points; ++index)
        {
            inv_frequency_data[index] = std::complex<double>(frequency_data[index].real(), -frequency_data[index].imag());
        }

        do_fft_one_dim(inv_frequency_data, spatial_data, level);

        for (uint32_t index = 0; index < num_points; ++index)
        {
            spatial_data[index] = std::complex<double>(spatial_data[index].real() * 1. / num_points, -spatial_data[index].imag() * 1. / num_points);
        }

        delete[] inv_frequency_data;

        return true;
    }
    // don't use this implement
    bool do_fourier_2d(double* _image, uint16_t _width, uint16_t _height)
    {
        uint16_t _level_width = static_cast<uint16_t>(std::log(_width) / log(2.));
        uint16_t _level_height = static_cast<uint16_t>(std::log(_height) / log(2.));

        uint16_t _round_width = static_cast<uint16_t>(std::pow(2., _level_width));
        uint16_t _round_height = static_cast<uint16_t>(std::pow(2., _level_height));

        std::complex<double> * _image2convert = new (std::nothrow) std::complex<double>[_round_width * _round_height];
        if (nullptr == _image2convert) return false;
        std::complex<double> * _image_in_freq = new (std::nothrow) std::complex<double>[_round_width * _round_height];
        if (nullptr == _image_in_freq)
        {
            delete[] _image2convert;
            return false;
        }

        uint32_t _length = static_cast<uint32_t>(_width * _height);
        for (uint32_t index = 0; index < _length; ++index)
        {
            _image2convert[index] = std::complex<double>(_image[index], 0.);
        }



        delete[] _image2convert;
        delete[] _image_in_freq;
        return true;
    }
    // log->fft->H(u,v)->ifft->exp
    // H(u,v) = E_h - E_l / ( 1 + 2.415 * (D(u,v)/D_0)^4 )
    // where the D(u,v) is the distance to the center of filter and D_0 is the cut-off frequency
    bool do_fourier_any(double* _image, uint16_t _width, uint16_t _height)
    {
        std::vector<std::vector<std::complex<double>>> complex_image;
        std::vector<std::vector<std::complex<double>>> frequency_image;
        FFTPF<double> Fourier;

        frequency_image.resize(_height);

        for (uint16_t iY = 0; iY < _height; ++iY)
        {
            std::vector <std::complex<double>> temp_vec;
            for (uint16_t iX = 0; iX < _width; ++iX)
            {
                temp_vec.push_back(std::complex<double>(static_cast<double>(_image[iY * _width + iX] * std::pow(-1., (iX + iY))), 0.));
            }
            complex_image.push_back(temp_vec);
        }

        for (uint16_t iY = 0; iY < _height; ++iY)
        {
            frequency_image[iY].resize(_width);
            Fourier.fft(complex_image[iY], frequency_image[iY]);
        }

        complex_image.resize(_width);
        for (uint16_t iY = 0; iY < _width; ++iY)
        {
            complex_image[iY].resize(_height);
        }
        // make a matrix transpose
        for (uint16_t iY = 0; iY < _height; ++iY)
        {
            for (uint16_t iX = 0; iX < _width; ++iX)
            {
                complex_image[iX][iY] = frequency_image[iY][iX];
            }
        }

        frequency_image.resize(_width);
        for (uint16_t iY = 0; iY < _width; ++iY)
        {
            frequency_image[iY].resize(_height);
            Fourier.fft(complex_image[iY], frequency_image[iY]);
        }

        // the amplitude image
        float *amp_image = new (std::nothrow) float[_width * _height];
        if (nullptr == amp_image) return false;

        for (uint16_t iY = 0; iY < _width; ++iY)
        {
            for (uint16_t iX = 0; iX < _height; ++iX)
            {
                double amplitude = std::pow(std::pow(frequency_image[iY][iX].imag(), 2.) + std::pow(frequency_image[iY][iX].real(), 2.), .5);
                //if (amplitude < 0.) amplitude = 0.;
                //if (amplitude > _limit) amplitude = _limit;
                amp_image[iY * _height + iX] = amplitude;
            }
        }

        double *filter_image = new (std::nothrow) double[_width * _height];
        if (nullptr == filter_image){
            delete[] amp_image;
            return false;
        }

        double *distance_image = new (std::nothrow) double[_width * _height];
        if (nullptr == distance_image){
            delete[] filter_image;
            delete[] amp_image;
            return false;
        }

        for (uint16_t iY = 0; iY < _width; ++iY)
        {
            for (uint16_t iX = 0; iX < _height; ++iX)
            {
                distance_image[iY * _height + iX] = std::pow(std::pow(iY - _width / 2, 2.) + std::pow(iX - _height / 2, 2.), .5);
            }
        }

//		const double gamma1 = 3.;
//		const double gamma2 = 0.8;
//		const double rou_c = 70.;
        for (uint16_t iY = 0; iY < _width; ++iY)
        {
            for (uint16_t iX = 0; iX < _height; ++iX)
            {
                filter_image[iY * _height + iX] = m_fGammaHigh - m_fGammaLow / (1 + 2.415 * std::pow((distance_image[iY * _height + iX] / m_fCutOff), 4.));
            }
        }

        // do filter in frequency domain
        for (uint16_t iY = 0; iY < _width; ++iY)
        {
            for (uint16_t iX = 0; iX < _height; ++iX)
            {
                frequency_image[iY][iX] = frequency_image[iY][iX] * std::complex<double>(filter_image[iY * _height + iX], 0.);
            }
        }

        complex_image.resize(_width);
        for (uint16_t iY = 0; iY < _width; ++iY)
        {
            complex_image[iY].resize(_height);
            Fourier.ifft(frequency_image[iY], complex_image[iY]);
        }
        // make a matrix transpose
        frequency_image.resize(_height);
        for (uint16_t iY = 0; iY < _height; ++iY)
        {
            frequency_image[iY].resize(_width);
            for (uint16_t iX = 0; iX < _width; ++iX)
            {
                frequency_image[iY][iX] = complex_image[iX][iY];
            }
        }

        complex_image.resize(_height);
        for (uint16_t iY = 0; iY < _height; ++iY)
        {
            complex_image[iY].resize(_width);
            Fourier.ifft(frequency_image[iY], complex_image[iY]);
        }

        for (uint32_t iY = 0; iY < _height; ++iY)
        {
            for (uint16_t iX = 0; iX < _width; ++iX)
            {
                _image[iY * _width + iX] = complex_image[iY][iX].real() * std::pow(-1., iX + iY);
            }
        }

        delete[] distance_image;
        delete[] filter_image;
        delete[] amp_image;
        return true;
    }

//	bool Fourier(LPSTR lpDIBBits, long lWidth, long lHeight)
//	{
//
//		// 指向源图像的指针
//		unsigned char*	lpSrc;
//
//		// 中间变量
//		double	dTemp;
//
//		// 循环变量
//		long	i;
//		long	j;
//
//		// 进行付立叶变换的宽度和高度（2的整数次方）
//		long	w;
//		long	h;
//
//		int		wp;
//		int		hp;
//
//		// 图像每行的字节数
//		long	lLineBytes;
//
//		// 计算图像每行的字节数
//		lLineBytes = WIDTHBYTES(lWidth * 8);
//
//		// 赋初值
//		w = 1;
//		h = 1;
//		wp = 0;
//		hp = 0;
//
//		// 计算进行付立叶变换的宽度和高度（2的整数次方）
//		while (w * 2 <= lWidth)
//		{
//			w *= 2;
//			wp++;
//		}
//
//		while (h * 2 <= lHeight)
//		{
//			h *= 2;
//			hp++;
//		}
//
//		// 分配内存
//		std::complex<double> *TD = new std::complex<double>[w * h];
//		std::complex<double> *FD = new std::complex<double>[w * h];
//
//		// 行
//		for (i = 0; i < h; i++)
//		{
//			// 列
//			for (j = 0; j < w; j++)
//			{
//				// 指向DIB第i行，第j个象素的指针
//				lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
//
//				// 给时域赋值
//				TD[j + w * i] = std::complex<double>(*(lpSrc), 0);
//			}
//		}
//
//		for (i = 0; i < h; i++)
//		{
//			// 对y方向进行快速付立叶变换
//			FFT(&TD[w * i], &FD[w * i], wp);
//		}
//
//		// 保存变换结果
//		for (i = 0; i < h; i++)
//		{
//			for (j = 0; j < w; j++)
//			{
//				TD[i + h * j] = FD[j + w * i];
//			}
//		}
//
//		for (i = 0; i < w; i++)
//		{
//			// 对x方向进行快速付立叶变换
//			FFT(&TD[i * h], &FD[i * h], hp);
//		}
//
//		// 行
//		for (i = 0; i < h; i++)
//		{
//			// 列
//			for (j = 0; j < w; j++)
//			{
//				// 计算频谱
//				dTemp = sqrt(FD[j * h + i].real() * FD[j * h + i].real() +
//					FD[j * h + i].imag() * FD[j * h + i].imag()) / 100;
//
//				// 判断是否超过255
//				if (dTemp > 255)
//				{
//					// 对于超过的，直接设置为255
//					dTemp = 255;
//				}
//
//				// 指向DIB第(i<h/2 ? i+h/2 : i-h/2)行，第(j<w/2 ? j+w/2 : j-w/2)个象素的指针
//				// 此处不直接取i和j，是为了将变换后的原点移到中心
//				//lpSrc = (unsigned char*)lpDIBBits + lLineBytes * (lHeight - 1 - i) + j;
//				lpSrc = (unsigned char*)lpDIBBits + lLineBytes *
//					(lHeight - 1 - (i<h / 2 ? i + h / 2 : i - h / 2)) + (j<w / 2 ? j + w / 2 : j - w / 2);
//
//				// 更新源图像
//				*(lpSrc) = (BYTE)(dTemp);
//			}
//		}
//
//		// 删除临时变量
//		delete TD;
//		delete FD;
//
//		// 返回
//		return true;
//	}

    void gaussian_filter(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
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
                    uint16_t image_temp = _image[ypos * _width + xpos];
                    if (xpos < 0 || ypos < 0 || xpos > _width - 1 || ypos > _height - 1)
                    {
                        // reduce the edge effect(const reflect boundary extend)
                        image_temp = _image[iY * _width + iX];
                        //continue;
                    }
                    sum += m_aGaussianMat[index] * image_temp;
                }
                // there is no scale front 'sum' in the paper
                sum = (_image[iY * _width + iX] - 5 * sum / 3) + m_iBright;//
                if (sum < 0.f) sum = 0.f;
                if (sum > _limit) sum = _limit;
                _image[iY * _width + iX] = sum;
            }
        }

        delete[] x_delta;
        delete[] y_delta;
    }

    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit)
    {
        //gaussian_filter(_image, _width, _height, _limit);

        double *dbl_image = new (std::nothrow) double[_width * _height];
        if (nullptr == dbl_image) return false;

        uint32_t _length = static_cast<uint32_t>(_width * _height);
        for (uint32_t index = 0; index < _length; ++index)
        {
            dbl_image[index] = std::log(static_cast<double>(_image[index]) + 1.);
        }

        do_fourier_any(dbl_image, _width, _height);

        double global_min = (std::numeric_limits<double>::max)();
        double global_max = -(std::numeric_limits<double>::max)();
        [&]()
        {
            for (uint16_t iY = 0; iY < _height; ++iY)
            {
                for (uint16_t iX = 0; iX < _width; ++iX)
                {
                    dbl_image[iY * _width + iX] = std::exp(dbl_image[iY * _width + iX]);
                    if (dbl_image[iY * _width + iX] > global_max) global_max = dbl_image[iY * _width + iX];
                    if (dbl_image[iY * _width + iX] < global_min) global_min = dbl_image[iY * _width + iX];
                }
            }
        }();

        for (uint16_t iY = 0; iY < _height; ++iY)
        {
            for (uint16_t iX = 0; iX < _width; ++iX)
            {
                _image[iY * _width + iX] = static_cast<uint16_t>(_limit * (dbl_image[iY * _width + iX] - global_min) / (global_max - global_min));
            }
        }

        delete[] dbl_image;
        return true;
    }

private:
    uint16_t m_iBright;
    double m_fGammaHigh;
    double m_fGammaLow;
    double m_fCutOff;
    uint16_t m_iKernelSize;
    float * m_aGaussianMat;
};

#endif /* __K_HOMOMORPHIC_FILTER_H__ */

