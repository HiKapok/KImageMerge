#ifndef QWALLISADJUST
#define QWALLISADJUST

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "KWallisAdjust.h"

class QWallisAdjust : public MainWindow
{
    Q_OBJECT
public:
    QWallisAdjust(QWidget* parent= 0, QString _title = QObject::tr("Wallis滤波器矫正算法"))
        : MainWindow(parent)
        , m_kernelSize(9)
        , m_targetMean(32768)
        , m_targetSigma(2600)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QWallisAdjust(){}

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
    void setTargetMean(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入小数！"));
            return;
        }
        m_targetMean = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setTargetSigma(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入小数！"));
            return;
        }
        m_targetSigma = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    // QWidget interface
protected:
    virtual void customUI()
    {
        add_input_box("窗口大小：", m_kernelSize, this,SLOT(setKernelSize(const QString)));
        add_input_box("目标均值：", m_targetMean, this,SLOT(setTargetMean(const QString)));
        add_input_box("目标方差：", m_targetSigma, this,SLOT(setTargetSigma(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        return KWallisAdjust(m_targetMean, m_targetSigma, m_kernelSize)(_image, _width, _height, _limit);
    }
private:
    uint16_t m_kernelSize;
    double m_targetMean;
    double m_targetSigma;
};

#endif // QWALLISADJUST

