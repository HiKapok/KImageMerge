#ifndef KHOMOFILTER3
#define KHOMOFILTER3

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#include <cstdint>
#include <cfloat>
#include <complex>
#include <bitset>
#include <cmath>

#include <QDebug>

#include "fftpf-impl.h"

class KHomoFilter3
{
public:
    KHomoFilter3(double _gammaHigh = 2.5, double _gammaLow = .2, double _sharpen = 0.5, double _cutoff = 1.)
        : m_fGammaHigh(_gammaHigh)
        , m_fGammaLow(_gammaLow)
        , m_fSharpen(_sharpen)
        , m_fCutOff(_cutoff)
    {
    }

    ~KHomoFilter3()
    {
    }

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

        double _average_distance = 0.;
        for (uint16_t iY = 0; iY < _width; ++iY)
        {
            for (uint16_t iX = 0; iX < _height; ++iX)
            {
                distance_image[iY * _height + iX] = std::pow(std::pow(iY - _width / 2, 2.) + std::pow(iX - _height / 2, 2.), .5);
                _average_distance += distance_image[iY * _height + iX];
                if(distance_image[iY * _height + iX] < DBL_EPSILON) distance_image[iY * _height + iX] = DBL_EPSILON;
            }
        }
        _average_distance /= (_width * _height);
        //m_fCutOff = 0.5 * _average_distance;
//		const double gamma1 = 3.;
//		const double gamma2 = 0.8;
//		const double rou_c = 70.;
        qDebug()<<"GammaHigh"<<m_fGammaHigh<<"GammaLow"<<m_fGammaLow<<"Sharpen"<<m_fSharpen<<"m_fCutOff"<<m_fCutOff;
        for (uint16_t iY = 0; iY < _width; ++iY)
        {
            for (uint16_t iX = 0; iX < _height; ++iX)
            {
                filter_image[iY * _height + iX] = m_fGammaLow + (m_fGammaHigh - m_fGammaLow) * (1. - std::exp( -m_fSharpen * std::pow( (distance_image[iY * _height + iX] / m_fCutOff), 2. ) ));
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

    // log->fft->H(u,v)->ifft->exp
    // H(u,v) = E_l + (E_h - E_l) * ( 1. - e^( -c * (D(u,v)/D_0)^2 )
    // where D_0 is the cut-off freauency and c is sharpen ratio, D(u,v) is the distance to the center of the filter
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
    double m_fGammaHigh;
    double m_fGammaLow;
    double m_fSharpen;
    double m_fCutOff;
};


#endif // KHOMOFILTER3

