#ifndef KBKGFIT
#define KBKGFIT

#include <cstdint>
#include <cfloat>

#include <QDebug>

class KBkgFit
{
public:
    KBkgFit()
    {
    }
    // create matrix A_6*6
    inline void matrixA_prepare(double *pArray, uint16_t width, uint16_t height) const{
        memset(pArray, 0, sizeof(double)*6*6);
        for(uint16_t iY = 0;iY<height;++iY){
            for(uint16_t iX=0;iX < width;++iX){
                std::vector<int32_t> temp={1, iX, iY, iX*iX, iX*iY, iY*iY};
                for(uint16_t index = 0;index<36;++index){
                    pArray[index] += temp[index/6]*temp[index%6];
                }
            }
        }
    }
    // create matrix B_6*1
    void matrixB_prepare(uint16_t* _image, double *pArray, uint16_t width, uint16_t height) const{
        memset(pArray, 0, sizeof(double)*6*1);
        for(uint16_t iY = 0;iY<height;++iY){
            for(uint16_t iX=0;iX < width;++iX){
                uint16_t _pixel = _image[iY*width+iX];
                std::vector<int32_t> temp={1*_pixel, iX*_pixel, iY*_pixel, iX*iX*_pixel, iX*iY*_pixel, iY*iY*_pixel};
                for(uint16_t index = 0;index<6;++index){
                    pArray[index] += temp[index];
                }
            }
        }
    }
    // use gaussian elimination method to get a up ladder type matrix which the first element of each line is 1
    inline void add_line_from(double *_pArray, double *_pInvArray, uint32_t _length, uint32_t _firstPlace) const{
        uint32_t _startLine = _firstPlace;
        double _scaleRatio = _pArray[_firstPlace * _length + _startLine];
        uint32_t _curWidth = 0;
        for(; _curWidth < _startLine; ++_curWidth){
            _pInvArray[_firstPlace * _length + _curWidth] /= _scaleRatio;
        }
        for(; _curWidth < _length; ++_curWidth){
            _pArray[_firstPlace * _length + _curWidth] /= _scaleRatio;
            _pInvArray[_firstPlace * _length + _curWidth] /= _scaleRatio;
        }

        for(uint32_t _line = _startLine+1; _line < _length; ++_line){
            double _ampRatio = _pArray[_line * _length + _startLine];
            _curWidth = 0;
            for(; _curWidth < _startLine; ++_curWidth){
                _pInvArray[_line * _length + _curWidth] -= _ampRatio*_pInvArray[_firstPlace * _length + _curWidth];
            }
            for(;_curWidth < _length;++_curWidth){
                _pArray[_line * _length + _curWidth] -= _ampRatio*_pArray[_firstPlace * _length + _curWidth];
                _pInvArray[_line * _length + _curWidth] -= _ampRatio*_pInvArray[_firstPlace * _length + _curWidth];
            }
        }
    }
    // use gaussian elimination method to get a unit matrix
    // and now, the _pInvArray is the inverse matrix we need
    inline void add_line_backward(double *_pArray, double *_pInvArray, uint32_t _length, uint32_t _firstPlace) const{
        uint32_t _curWidth = 0;
        for(uint32_t _line = 0; _line < _firstPlace; ++_line){
            double _ampRatio = _pArray[_line * _length + _firstPlace];
            _curWidth = 0;
            for(; _curWidth < _firstPlace; ++_curWidth){
                _pInvArray[_line * _length + _curWidth] -= _ampRatio*_pInvArray[_firstPlace * _length + _curWidth];
            }
            for(;_curWidth < _length;++_curWidth){
                _pArray[_line * _length + _curWidth] -= _ampRatio*_pArray[_firstPlace * _length + _curWidth];
                _pInvArray[_line * _length + _curWidth] -= _ampRatio*_pInvArray[_firstPlace * _length + _curWidth];
            }
        }
    }
    // calculate the multiply of two square matrix which has the same dimension of _length
    void matrix_mul(double *lpArray, double *rpArray, double *result_array, uint32_t _length) const{
        memset(result_array, 0, sizeof(double) * _length * _length);

        for(uint32_t iY = 0;iY<_length;++iY){
            for(uint32_t iX = 0;iX<_length;++iX){
                result_array[iY*_length+iX]=[=]()->double{
                    double ret = 0;
                    for(uint32_t index = 0;index<_length;++index){
                        ret+=lpArray[iY*_length+index]*rpArray[index*_length+iX];
                    }
                    return ret;
                }();
            }
        }
    }
    // calculate the multiply of two square matrix
    // the left operand is one _lHeight*_mid matrix
    // the right operand is one _mid*_rWidth matrix
    void matrix_mul(double *lpArray, double *rpArray, double *result_array, uint32_t _lHeight, uint32_t _mid, uint32_t _rWidth) const{
        memset(result_array, 0, sizeof(double) * _lHeight * _rWidth);
        for(uint32_t iY = 0;iY<_lHeight;++iY){
            for(uint32_t iX = 0;iX<_rWidth;++iX){
                result_array[iY*_rWidth+iX]=[=]()->double{
                    double ret = 0;
                    for(uint32_t index = 0;index<_mid;++index){
                        ret+=lpArray[iY*_mid+index]*rpArray[index*_rWidth+iX];
                    }
                    return ret;
                }();
            }
        }
    }
    // the routine of calculate the inverse of a matrix
    void matrix_cal_inv(double *pArray, double *inv_array, uint32_t length) const{
        memset(inv_array, 0, sizeof(double) * length * length);
        for(uint32_t index = 0;index < length;++index){
            inv_array[index * length + index] = 1.;
        }
        for(uint32_t _curHeight = 0;_curHeight < length;++_curHeight){
            // equal to zero
            if(pArray[_curHeight * length + _curHeight] < DBL_EPSILON){
                pArray[_curHeight * length + _curHeight] += DBL_EPSILON;
                add_line_from(pArray, inv_array, length, _curHeight);
                //return;
            }else{
               add_line_from(pArray, inv_array, length, _curHeight);
            }
        }
        for(uint32_t _curHeight = length - 1;_curHeight > 0;--_curHeight){
            add_line_backward(pArray, inv_array, length, _curHeight);
       }
    }
    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
    {
        double *arrayA = new double[6*6];
        double *arrayInvA = new double[6*6];
        double *arrayB = new double[6];
        double *arrayX = new double[6];
        matrixA_prepare(arrayA, _width, _height);
        matrixB_prepare(_image, arrayB, _width, _height);

        matrix_cal_inv(arrayA, arrayInvA, 6);
//        double *arrayTest = new double[6*6];
//        matrixA_prepare(arrayA, _width, _height);
//         matrix_mul(arrayInvA, arrayA, arrayTest, 6, 6, 6);
//         for(int index=0;index<36;++index)qDebug()<<arrayTest[index];
//        delete[] arrayTest;
        matrix_mul(arrayInvA, arrayB, arrayX, 6, 6, 1);

        double _mean = 0.;
        for(int iY = 0;iY<_height;++iY){
            for(int iX=0;iX < _width;++iX){
                _mean += (1*arrayX[0] + iX*arrayX[1] + iY*arrayX[2] + iX*iX*arrayX[3] + iX*iY*arrayX[4] + iY*iY*arrayX[5]);
            }
        }
        _mean /= (_height*_width);
        for(int iY = 0;iY<_height;++iY){
            for(int iX=0;iX < _width;++iX){
                //double temp_pixel = (_mean + _image[iY*_width+iX] -0)*0+ (1*arrayX[0] + iX*arrayX[1] + iY*arrayX[2] + iX*iX*arrayX[3] + iX*iY*arrayX[4] + iY*iY*arrayX[5]);
                double temp_pixel = _mean + _image[iY*_width+iX] - (1*arrayX[0] + iX*arrayX[1] + iY*arrayX[2] + iX*iX*arrayX[3] + iX*iY*arrayX[4] + iY*iY*arrayX[5]);
                if(temp_pixel<0.) temp_pixel=0.;
                if(temp_pixel>_limit) temp_pixel=_limit;
                _image[iY*_width+iX] = static_cast<uint16_t>(temp_pixel);
            }
        }

        delete[] arrayA;
        delete[] arrayInvA;
        delete[] arrayB;
        delete[] arrayX;
        return true;
    }

private:

};

#endif // KBKGFIT

