#ifndef QHOMOMORPHICFILTER
#define QHOMOMORPHICFILTER

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "KHomomorphicFilter.h"

class QHomomorphicFilter : public MainWindow
{
    Q_OBJECT
public:
    QHomomorphicFilter(QWidget* parent= 0, QString _title = QObject::tr("同态滤波矫正算法"))
        : MainWindow(parent)
        , m_bright(32768)
        , m_gammaHigh(6.)
        , m_gammaLow(0.9)
        , m_cutOff(200.)
        , m_kernelSize(9)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QHomomorphicFilter(){}

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
    void setGammaHigh(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入一个小数！"));
            return;
        }
        m_gammaHigh = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setGammaLow(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入一个小数！"));
            return;
        }
        m_gammaLow = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setCutOff(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入一个小数！"));
            return;
        }
        m_cutOff = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }

    // QWidget interface
protected:
    virtual void customUI()
    {
        add_input_box("平均亮度：", m_bright, this,SLOT(setBright(const QString)));
        add_input_box("gamma上限：", m_gammaHigh, this,SLOT(setGammaHigh(const QString)));
        add_input_box("gamma下限：", m_gammaLow, this,SLOT(setGammaLow(const QString)));
        add_input_box("rou：", m_cutOff, this,SLOT(setCutOff(const QString)));
        add_input_box("核大小：", m_kernelSize, this,SLOT(setKernelSize(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        return KHomomorphicFilter(m_bright, m_gammaHigh, m_gammaLow, m_cutOff, m_kernelSize)(_image, _width, _height, _limit);
    }
private:
    uint16_t m_bright;
    double m_gammaHigh;
    double m_gammaLow;
    double m_cutOff;
    uint16_t m_kernelSize;
};


#endif // QHOMOMORPHICFILTER

