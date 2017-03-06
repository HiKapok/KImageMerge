#ifndef KGAMMAFIT
#define KGAMMAFIT

#include <cstdint>
#include <cfloat>

#include <QDebug>

class KGammaFit
{
public:
    KGammaFit(uint16_t _maxDegree, double _ratio, double _sigma = 0.6, uint16_t _patch_size = 3)
        : m_iMaxDegree(_maxDegree>1?_maxDegree:1),
          m_fRatio(_ratio),
          m_iKernelSize(_patch_size % 2 == 0 ? _patch_size + 1 : _patch_size)
    {
        [&](){
            float sum = 0.f;
            m_aGaussianMat = new (std::nothrow) float[m_iKernelSize * m_iKernelSize];
            if (nullptr == m_aGaussianMat) return;
            for (int iY = -m_iKernelSize / 2; iY <= m_iKernelSize / 2; ++iY)
            {
                for (int iX = -m_iKernelSize / 2; iX <= m_iKernelSize / 2; ++iX)
                {
                    //m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2] = 1.f / std::pow((2 * M_PI * std::pow(_sigma, 2.f)),0.5) * std::exp(-1.f * (std::pow(iX, 2.f) + std::pow(iY, 2.f)) / (2 * std::pow(_sigma, 2.f)));
                    m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2] = std::exp(-1.f * (std::pow(iX, 2.f) + std::pow(iY, 2.f)) / (2 * std::pow(_sigma, 2.f)));
                    sum += m_aGaussianMat[(iY + m_iKernelSize / 2)*m_iKernelSize + iX + m_iKernelSize / 2];
                }
            }
            for (int iY = 0; iY < m_iKernelSize; ++iY)
            {
                for (int iX = 0; iX < m_iKernelSize; ++iX) m_aGaussianMat[iY*m_iKernelSize + iX] = m_aGaussianMat[iY*m_iKernelSize + iX] / sum;
            }
        }();
    }
    KGammaFit(const KGammaFit &rhs)
    {
        this->m_iMaxDegree = rhs.m_iMaxDegree;
        this->m_fRatio = rhs.m_fRatio;
        this->m_iKernelSize = rhs.m_iKernelSize;
        // just throw it
        this->m_aGaussianMat = new float[m_iKernelSize * m_iKernelSize];
        memcpy(this->m_aGaussianMat, rhs.m_aGaussianMat, m_iKernelSize * m_iKernelSize * sizeof(float));
    }

    void this_swap(KGammaFit &rhs){
        using std::swap;
        swap(this->m_iMaxDegree, rhs.m_iMaxDegree);
        swap(this->m_fRatio, rhs.m_fRatio);
        swap(this->m_iKernelSize, rhs.m_iKernelSize);
        swap(this->m_aGaussianMat, rhs.m_aGaussianMat);
    }

    KGammaFit & operator=(KGammaFit rhs /* copy construct here */){
        this_swap(rhs);
        return *this;
    }

    ~KGammaFit()
    {
        delete[] m_aGaussianMat;
    }

    void gaussian_filter(double* _image, uint16_t _width, uint16_t _height, double _limit) const
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
                double sum = 0.f;
                for (uint16_t index = 0; index < m_iKernelSize * m_iKernelSize; ++index)
                {
                    int16_t xpos = iX + x_delta[index];
                    int16_t ypos = iY + y_delta[index];
                    double image_temp = 0.;
                    if (xpos < 0 || ypos < 0 || xpos > _width - 1 || ypos > _height - 1)
                    {
                        // reduce the edge effect(const reflect boundary extend)
                        image_temp = _image[iY * _width + iX];
                        //continue;
                    }else image_temp = _image[ypos * _width + xpos];
                    sum += m_aGaussianMat[index] * image_temp;
                }
                // there is no scale front 'sum' in the paper
                //sum = (_image[iY * _width + iX] - 25*sum/25) + m_iBright;//
                //if (sum < 0.f) sum = 0.f;
                //if (sum > _limit) sum = _limit;
                _image[iY * _width + iX] = sum;
            }
        }

        delete[] x_delta;
        delete[] y_delta;
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

    inline void cal_matrix_trans(double *pArray, uint32_t _length){
        for(uint32_t iY = 0;iY<_length;++iY){
            for(uint32_t iX = iY;iX<_length;++iX){
                std::swap(pArray[iY*_length+iX], pArray[iX*_length+iY]);
            }
        }
    }
    inline void cal_matrix_trans(double *pArray, double *pDesArray, uint32_t _length){
        for(uint32_t iY = 0;iY<_length;++iY){
            for(uint32_t iX = 0;iX<_length;++iX){
                pDesArray[iX*_length+iY] = pArray[iY*_length+iX];
            }
        }
    }
    inline void cal_matrix_trans(double *pArray, double *pDesArray, uint32_t _width, uint32_t _height) const{
            for(uint32_t iY = 0;iY<_height;++iY){
                for(uint32_t iX = 0;iX<_width;++iX){
                    pDesArray[iX*_height+iY] = pArray[iY*_width+iX];
                }
            }
        }
    inline void matrixQ_prepare(double *result, uint16_t _MAX_N, uint16_t iY, uint16_t iX) const{
        for(uint16_t out_index = 1; out_index <= _MAX_N; ++out_index){
            for(uint16_t in_index = 0; in_index < out_index; ++in_index){
                *result++ = (out_index-in_index)*std::pow(iX, out_index-in_index-1)*std::pow(iY, in_index);
                //qDebug()<<(out_index-in_index)<<"x^"<<out_index-in_index-1<<"y^"<<in_index<<" ";
            }
            *result++ = 0.;
        }
    }
    inline void matrixR_prepare(double *result, uint16_t _MAX_N, uint16_t iY, uint16_t iX) const{
        for(uint16_t out_index = 1; out_index <= _MAX_N; ++out_index){
            *result++ = 0.;
            for(uint16_t in_index = 1; in_index <= out_index; ++in_index){
                *result++ = in_index*std::pow(iX, out_index-in_index)*std::pow(iY, in_index-1);
                //qDebug()<<in_index<<"x^"<<out_index-in_index<<"y^"<<in_index-1<<" ";
            }
        }
    }
    inline void matrixGamma_prepare(uint16_t _MAX_N){
        for(uint16_t out_index = 1; out_index <= _MAX_N; ++out_index){
            for(uint16_t in_index = 0; in_index <= out_index; ++in_index){
                qDebug()<<"r^"<<out_index-in_index<<in_index<<" ";
            }
        }
    }
    // create matrix M_2P*( (N+3)*N/2 ), where P is the total number of pixels
    // !manage your pArray memory outside yourself!
    inline void matrixM_prepare(double *pArray, uint16_t _MAX_POW, uint16_t width, uint16_t height) const{
        uint32_t _total_pixels = width * height;
        uint32_t _num_perline = _MAX_POW*(_MAX_POW+3)/2;
        memset(pArray, 0, sizeof(double)*2*_total_pixels*_num_perline);
        // prepare first P lines
        for(uint16_t iY = 0; iY < height; ++iY){
            for(uint16_t iX = 0; iX < width; ++iX){
                matrixQ_prepare(pArray+(iY*width+iX)*_num_perline,_MAX_POW,iY,iX);
            }
        }
        // prepare last P lines
        for(uint16_t iY = 0; iY<height; ++iY){
            for(uint16_t iX = 0; iX<width; ++iX){
                matrixR_prepare(pArray+((height+iY)*width+iX)*_num_perline,_MAX_POW,iY,iX);
            }
        }
    }
    inline void matrixG_prepare(double* _image, double *pArray, uint16_t width, uint16_t height) const{
        uint32_t _total_lines = 2 * width * height;
        memset(pArray, 0, sizeof(double)*_total_lines*1);
        // prepare first P lines
        for(uint16_t iY = 0; iY < height; ++iY){
            for(uint16_t iX = 0; iX < width; ++iX){
                if(iX>0 && iX<width-1){
                    pArray[iY*width+iX] = _image[iY*width+iX+1]-_image[iY*width+iX-1];
                }
                else pArray[iY*width+iX]=0.;
            }
        }
        // prepare last P lines
        for(uint16_t iY = 0; iY<height; ++iY){
            for(uint16_t iX = 0; iX<width; ++iX){
                if(iY>0 && iY<height-1){
                    pArray[(iY+height)*width+iX] = _image[(iY+1)*width+iX]-_image[(iY-1)*width+iX];
                }
                else pArray[(iY+height)*width+iX]=0.;
            }
        }
    }
    bool operator()(uint16_t* _image, uint16_t _width, uint16_t _height, double _limit) const
    {
        uint32_t _total_line = 2*_width * _height;
        uint32_t _num_perline = m_iMaxDegree*(m_iMaxDegree+3)/2;
        double *arrayM = new double[_total_line * _num_perline];
        double *arrayG = new double[_total_line * 1];
        double *arrayGamma = new double[_total_line * 1];
        double *arrayTransM = new double[_total_line * _num_perline];
        double *arrayMTM = new double[_num_perline*_num_perline];
        double *arrayInvMTM = new double[_num_perline * _num_perline];
        // gaussian filter first
        double * temp_image = new (std::nothrow) double[_width * _height];
        if(nullptr == temp_image) return false;

        for (int16_t iRow = 0; iRow < _height; iRow++)
        {
            for (int16_t iColumn = 0; iColumn < _width; iColumn++)
            {
                temp_image[iRow * _width + iColumn] = static_cast<double>(_image[iRow * _width + iColumn]);
            }
        }
        gaussian_filter(temp_image, _width, _height, _limit);
        // log transform
        for (int16_t iRow = 0; iRow < _height; iRow++)
        {
            for (int16_t iColumn = 0; iColumn < _width; iColumn++)
            {
                temp_image[iRow * _width + iColumn] = std::log(temp_image[iRow * _width + iColumn]);
            }
        }
        // calculate all needed matrixes
        matrixM_prepare(arrayM, m_iMaxDegree, _width, _height);
        matrixG_prepare(temp_image, arrayG, _width, _height);
        cal_matrix_trans(arrayM, arrayTransM, _num_perline, _total_line);
        //double *temp_gamma = arrayM;
//        for(uint16_t out_index = 0; out_index < _total_line; ++out_index){
//            for(uint16_t in_index = 0; in_index < _num_perline; ++in_index){
//                //qDebug()<< std::abs(arrayM[in_index*_num_perline+out_index]-arrayTransM[out_index*_total_line+in_index]);
//                //qDebug()<< *temp_gamma++;
//            }
//        }
        matrix_mul(arrayTransM, arrayM, arrayMTM, _num_perline, _total_line, _num_perline);

        matrix_cal_inv(arrayMTM, arrayInvMTM, _num_perline);

        // reuse of matrrix M
        matrix_mul(arrayInvMTM, arrayTransM, arrayM, _num_perline, _num_perline, _total_line);
        // now matrix M is _num_perline*_total_line
        matrix_mul(arrayM, arrayG, arrayGamma, _num_perline, _total_line, 1);
//        temp_gamma = arrayGamma;
//        for(uint16_t out_index = 1; out_index <= m_iMaxDegree; ++out_index){
//            for(uint16_t in_index = 0; in_index <= out_index; ++in_index){
//                qDebug()<< *temp_gamma++;
//            }
//        }
        for(int iY = 0;iY<_height;++iY){
            for(int iX=0;iX < _width;++iX){
                double temp_pixel = [=]()->double{
                    double *temp_gamma = arrayGamma;
                    double ret = 0.;
                    for(uint16_t out_index = 1; out_index <= m_iMaxDegree; ++out_index){
                        for(uint16_t in_index = 0; in_index <= out_index; ++in_index){
                            ret += *temp_gamma * std::pow(iX, out_index-in_index)*std::pow(iY, in_index);
                            temp_gamma++;
                        }
                    }
                    return std::exp(ret);
                }();
                temp_image[iY*_width+iX] = temp_pixel;
                //qDebug()<<temp_pixel;
//                if(temp_pixel<0.) temp_pixel=0.;
//                if(temp_pixel>_limit) temp_pixel=_limit;
//                _image[iY*_width+iX] = static_cast<uint16_t>(temp_pixel);
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
                    if (temp_image[iY * _width + iX] > global_max) global_max = temp_image[iY * _width + iX];
                    if (temp_image[iY * _width + iX] < global_min) global_min = temp_image[iY * _width + iX];
                }
            }
        }();
//qDebug()<<global_max<<global_min;

        for (uint16_t iY = 0; iY < _height; ++iY)
        {
            for (uint16_t iX = 0; iX < _width; ++iX)
            {
                double temp_value = _limit * (temp_image[iY * _width + iX] - global_min) / (global_max - global_min);
                temp_value = _image[iY * _width + iX]-temp_value*m_fRatio;
                if(temp_value<0.) temp_value=0.;
                if(temp_value>_limit) temp_value=_limit;
                _image[iY * _width + iX] = static_cast<uint16_t>(temp_value);
            }
        }

        delete[] temp_image;
        delete[] arrayM;
        delete[] arrayG;
        delete[] arrayGamma;
        delete[] arrayTransM;
        delete[] arrayMTM;
        delete[] arrayInvMTM;
        return true;
    }

private:
    uint16_t m_iMaxDegree;
    double m_fRatio;
    uint16_t m_iKernelSize;
    float * m_aGaussianMat;
};

#endif // KGAMMAFIT

