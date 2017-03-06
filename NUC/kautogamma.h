#ifndef KAUTOGAMMA
#define KAUTOGAMMA

#include <cstdint>
#include <memory>
#include <cmath>

#include <QDebug>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

class KAutoGamma
{
public:
    KAutoGamma(double _rou, double _scale, double _a, double _b, double _c)
        : m_fRou(_rou)
        , m_fScaleParam(_scale)
        , m_fParamA(_a)
        , m_fParamB(_b)
        , m_fParamC(_c)
    {

    }

    inline double getF1(uint16_t _pixel, double _param_a, double _limit) const{
        return _param_a * (std::cos(M_PI * _pixel / _limit));
    }

    inline double getF2(uint16_t _pixel, double _param_b, double _limit) const{
        double _alpha = std::atan(-2.*_param_b/_limit);
        double _rou = m_fRou;
        return ( (_rou*std::sin(4.*M_PI*_pixel/_limit) + _param_b) * std::cos(_alpha) + _pixel * std::sin(_alpha) );
    }
    inline double getF3(uint16_t _pixel, double _param_c, double _limit) const{
        return ( _param_c * std::fabs(2. * _pixel / _limit - 1) * std::cos(3. * M_PI * _pixel / _limit) );
    }

    inline getHx(uint16_t _pixel, double _param_a, double _param_b, double _param_c, double _limit) const{
        return (1. + getF1(_pixel, _param_a, _limit) + getF2(_pixel, _param_b, _limit) + getF3(_pixel, _param_c, _limit) + _param_a + _param_b + _param_c);
    }

    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
    {
        //std::unique_ptr<double[]> fArrayF1(new double[static_cast<uint32_t>(_limit) + 1]);
        //std::unique_ptr<double[]> fArrayF2(new double[static_cast<uint32_t>(_limit) + 1]);
        //std::unique_ptr<double[]> fArrayF3(new double[static_cast<uint32_t>(_limit) + 1]);

        double global_min = (std::numeric_limits<double>::max)();
        double global_max = -(std::numeric_limits<double>::max)();
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
       // if(std::fabs(global_max - global_min) < FLT_EPSILON) global_max = global_min + FLT_EPSILON;


        double _gray_delta = (global_max - global_min + 1.) / 5.;
        std::unique_ptr<uint32_t[]> histgram(new uint32_t[5]);
        for(uint16_t index = 0;index < 5;++index){
            histgram[index] = 0;
        }
        for (int16_t iRow = 0; iRow < _height; iRow++)
        {
            for (int16_t iColumn = 0; iColumn < _width; iColumn++)
            {
                uint16_t tempValue = static_cast<uint16_t>(_image[iRow * _width + iColumn]);
                histgram[(tempValue-global_min)/_gray_delta]++;
            }
        }
        double _b = histgram[0] * 1. / (_width * _height);
        double _a = _b - 1;
        double _c = _a + .05;
        _a=0.8;
        _b=0.6;
        _c = 0.3;
        _a = m_fParamA;
        _b = m_fParamB;
        _c = m_fParamC;
        qDebug()<<"scale"<<m_fScaleParam;
        qDebug()<<"m_fRou"<<m_fRou;
        qDebug()<<"a"<<_a<<"b"<<_b<<"c"<<_c;

        for (int16_t iRow = 0; iRow < _height; iRow++)
        {
            for (int16_t iColumn = 0; iColumn < _width; iColumn++)
            {
                double tempValue = static_cast<double>(_image[iRow * _width + iColumn]);

                tempValue = m_fScaleParam * _limit * std::pow(tempValue/_limit, 1./getHx(tempValue,_a,_b,_c,_limit));
                //tempValue = m_fScaleParam * _limit * 1./(1+std::exp(-(tempValue-m_fRou)));
                if(tempValue < 0.) tempValue = 0.;
                if(tempValue > _limit) tempValue = _limit;
                _image[iRow * _width + iColumn] = static_cast<uint16_t>(tempValue);
            }
        }

        return true;
    }

private:
    double m_fRou;
    double m_fScaleParam;
    double m_fParamA;
    double m_fParamB;
    double m_fParamC;
};

#endif // KAUTOGAMMA

