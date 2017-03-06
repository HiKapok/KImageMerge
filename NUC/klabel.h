#ifndef KLABEL_H
#define KLABEL_H

#include <cstdint>
#include <cfloat>
#include <memory>

#include <QObject>
#include <QtWidgets/QLabel>
#include <QImage>
#include <QColor>
#include <QPaintEvent>
#include <QString>
#include <QMainWindow>

class KLabel : public QLabel
{
    Q_OBJECT
public:
    KLabel(QWidget* = 0);
    ~KLabel();

    // the param _limit is either 256 or 65536 to scale the input
    template<typename T>
    void setImage(T * _image, uint16_t _width, uint16_t _height, double _limit = 256.){
        if(nullptr == _image) return;
        if(nullptr != m_uImage){
            delete [] m_uImage;
            m_uImage = nullptr;
        }

        m_uImage = new (std::nothrow) uint8_t[_width * _height];
        if(nullptr == m_uImage) return;

        m_iWidth = _width;
        m_iHeight = _height;

        uint32_t image_length =  m_iWidth * m_iHeight;
        for(uint32_t index = 0;index < image_length;++index){
            if(std::fabs(256. - _limit) < DBL_EPSILON)
                m_uImage[index] = static_cast<uint8_t>(_image[index]);
            else if(std::fabs(65536. - _limit) < DBL_EPSILON)
                m_uImage[index] = static_cast<uint8_t>(_image[index] / 256.);
        }

        fillQImage(m_uImage, m_iWidth, m_iHeight, m_qImage);

        this->update();
    }

    void setColorLevel(uint8_t _level){
        m_iColorLevel = _level;
        creat_color_tabel(m_iColorLevel, m_bStartBlue);
        if(m_beColored && nullptr != m_uImage){
            fillQImage(m_uImage, m_iWidth, m_iHeight, m_qImage);
            this->update();
        }
    }

    void setColored(bool _sts){
        m_beColored = _sts;
        if(m_beColored) creat_color_tabel(m_iColorLevel, m_bStartBlue);
        if(nullptr != m_uImage){
            fillQImage(m_uImage, m_iWidth, m_iHeight, m_qImage);
            this->update();
        }
    }

    uint16_t get_width() const{
        return m_iWidth;
    }
    uint16_t get_height() const{
        return m_iHeight;
    }

    bool has_image() const{
        if(m_qImage) return true;
        return false;
    }

private:
    static uint16_t creat_color_tabel(uint8_t _step, bool _b_start_blue)
    {
        int iB = 255;
        int iG = 0;
        int iR = 160;
        m_veColorMap.clear();
        if (_b_start_blue) iR = _step;
        else m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        for (iR -= _step; iR > 0; iR -= _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        if (std::abs(iR) > 2 * _step / 3) { iR = 0; iG += _step; }
        for (iR = 0; iG < 128; iG += _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        for (; iG < 255 && iB > 0; iG += _step, iB -= 2 * _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        if (std::abs(iG - 255) > 2 * _step / 3 && std::abs(iB) > 2 * _step / 3) { iG = 255; iB = 0; iR += 2 * _step; }
        for (iG = 255, iB = 0; iR < 255; iR += 2 * _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        if (std::abs(iR - 255) > 2 * _step / 3) { iR = 255; iG -= _step; }
        for (iR = 255; iG > 0; iG -= _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        for (iG = 255 / 5, iR -= 2 * _step; iR > 2 * iG; iR -= _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        return m_veColorMap.size();
    }

    void fillQImage(const uint8_t * _image, uint16_t _width, uint16_t _height, std::shared_ptr<QImage>& qImage){
        if(m_beColored){
            qImage.reset(new QImage(_width, _height, QImage::Format_RGB888));
            for (int iY = 0; iY < _height; ++iY){
                for (int iX = 0; iX < _width; ++iX){
                    qImage->setPixel(iX, iY, m_veColorMap[static_cast<uint8_t>(_image[iY*_width + iX] * 1. * m_veColorMap.size() / 256.)]);
                }
            }
        }else{
            qImage.reset(new QImage(_width, _height, QImage::Format_RGB32));
            for (int iY = 0; iY < _height; ++iY){
                for (int iX = 0; iX < _width; ++iX){
                    uint8_t gray = static_cast<uint8_t>(_image[iY*_width + iX]);
                    qImage->setPixel(iX, iY, qRgb(gray,gray,gray));
                }
            }
        }

    }
    // QWidget interface
protected:
    virtual void paintEvent(QPaintEvent *);
private:
    uint8_t *m_uImage;
    uint16_t m_iWidth;
    uint16_t m_iHeight;
    bool m_beColored;
    std::shared_ptr<QImage> m_qImage;
    bool m_bStartBlue;
    QString m_sText;
    static uint8_t m_iColorLevel;
    static std::vector<QRgb> m_veColorMap;
};

#endif // KLABEL_H
