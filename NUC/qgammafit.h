#ifndef QGAMMAFIT
#define QGAMMAFIT

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "kgammafit.h"

class QGammaFit : public MainWindow
{
    Q_OBJECT
public:
    QGammaFit(QWidget* parent= 0, QString _title = QObject::tr("多项式背景拟合矫正算法"))
        : MainWindow(parent),
          m_maxDegree(5),
          m_sigma(10),
          m_ratio(1.),
          m_kernelSize(9)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QGammaFit(){}

private slots:
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
    void setMaxDegree(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入整数！"));
            return;
        }
        if(m_maxDegree<1)
        {
            set_message(QObject::tr("错误：请输入大于0的正整数！"));
            return;
        }
        m_maxDegree = static_cast<uint16_t>(temp);
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
    void setRatio(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入一个小数！"));
            return;
        }
        m_ratio = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    // QWidget interface
protected:
    virtual void customUI()
    {
        add_input_box("拟合多项式最高次数：", m_maxDegree, this,SLOT(setMaxDegree(const QString)));
        add_input_box("Sigma：", m_sigma, this,SLOT(setSigma(const QString)));
        add_input_box("Ratio：", m_ratio, this,SLOT(setRatio(const QString)));
        add_input_box("核大小：", m_kernelSize, this,SLOT(setKernelSize(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        return KGammaFit(m_maxDegree,m_ratio,m_sigma,m_kernelSize)(_image, _width, _height, _limit);
    }
private:
    uint16_t m_maxDegree;
    double m_sigma;
    double m_ratio;
    uint16_t m_kernelSize;
};

#endif // QGAMMAFIT

