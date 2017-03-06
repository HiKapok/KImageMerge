#ifndef QHOMOFILTER3
#define QHOMOFILTER3

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "khomofilter3.h"

class QHomoFilter3 : public MainWindow
{
    Q_OBJECT
public:
    QHomoFilter3(QWidget* parent= 0, QString _title = QObject::tr("同态滤波（高斯型滤波器）矫正算法"))
        : MainWindow(parent)
        , m_gammaHigh(2.2)
        , m_gammaLow(.5)
        , m_shapren(1.0)
        , m_cutOff(100.)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QHomoFilter3(){}

private slots:

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
    void setSharpen(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入一个小数！"));
            return;
        }
        m_shapren = static_cast<double>(temp);
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
        add_input_box("gamma上限：", m_gammaHigh, this,SLOT(setGammaHigh(const QString)));
        add_input_box("gamma下限：", m_gammaLow, this,SLOT(setGammaLow(const QString)));
        add_input_box("锐化系数：", m_shapren, this,SLOT(setSharpen(const QString)));
        add_input_box("截止频率：", m_cutOff, this,SLOT(setCutOff(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        return KHomoFilter3(m_gammaHigh, m_gammaLow, m_shapren, m_cutOff)(_image, _width, _height, _limit);
    }
private:
    double m_gammaHigh;
    double m_gammaLow;
    double m_shapren;
    double m_cutOff;
};

#endif // QHOMOFILTER3

