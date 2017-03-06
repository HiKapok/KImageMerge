#ifndef QGUMADJUST
#define QGUMADJUST


#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "kgumadjust.h"

class QGUMAdjust : public MainWindow
{
    Q_OBJECT
public:
    QGUMAdjust(QWidget* parent= 0, QString _title = QObject::tr("非锐化掩膜矫正算法"))
        : MainWindow(parent)
        , m_bright(32768)
        , m_scaleParam(.4f)
        , m_sigma(.2)
        , m_gamma(5)
        , m_kernelSize(5)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QGUMAdjust(){}

private slots:
    void setBright(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入整数！"));
            return;
        }
        m_bright = static_cast<uint16_t>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }

    void setKernelSize(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入整数！"));
            return;
        }
        m_kernelSize = static_cast<uint16_t>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setScaleParam(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入一个小数！"));
            return;
        }
        m_scaleParam = static_cast<float>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setSigma(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入一个小数！"));
            return;
        }
        m_sigma = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setGamma(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入一个小数！"));
            return;
        }
        m_gamma = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    // QWidget interface
protected:
    virtual void customUI()
    {
        add_input_box("平均亮度：", m_bright, this,SLOT(setBright(const QString)));
        add_input_box("对比度缩放尺度：", m_scaleParam, this,SLOT(setScaleParam(const QString)));
        add_input_box("Sigma：", m_sigma, this,SLOT(setSigma(const QString)));
        add_input_box("核大小：", m_kernelSize, this,SLOT(setKernelSize(const QString)));
        add_input_box("gamma：", m_gamma, this, SLOT(setGamma(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        return KGUMAdjust(m_bright, m_scaleParam, m_sigma, m_gamma, m_kernelSize)(_image, _width, _height, _limit);
    }
private:
    uint16_t m_bright;
    float m_scaleParam;
    double m_sigma;
    double m_gamma;
    uint16_t m_kernelSize;
};


#endif // QGUMADJUST

