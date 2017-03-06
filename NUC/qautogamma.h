#ifndef QAUTOGAMMA
#define QAUTOGAMMA

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "kautogamma.h"

class QAutoGamma : public MainWindow
{
    Q_OBJECT
public:
    QAutoGamma(QWidget* parent= 0, QString _title = QObject::tr("自动Gamma矫正算法"))
        : MainWindow(parent)
        , m_rou(0.08)
        , m_scaleParam(0.4)
        , m_paramA(1.4)
        , m_paramB(0.7)
        , m_paramC(0.8)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QAutoGamma(){}

private slots:
    void setRou(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入小数！"));
            return;
        }
        m_rou = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }

    void setScaleParam(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入小数！"));
            return;
        }
        m_scaleParam = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setParamA(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入小数！"));
            return;
        }
        m_paramA = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setParamB(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入小数！"));
            return;
        }
        m_paramB = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setParamC(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入小数！"));
            return;
        }
        m_paramC = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    // QWidget interface
protected:
    virtual void customUI()
    {
        add_input_box("f2的振幅系数：", m_rou, this, SLOT(setRou(const QString)));
        add_input_box("拉伸系数：", m_scaleParam, this, SLOT(setScaleParam(const QString)));
        add_input_box("参数A：", m_paramA, this, SLOT(setParamA(const QString)));
        add_input_box("参数B：", m_paramB, this, SLOT(setParamB(const QString)));
        add_input_box("参数C：", m_paramC, this, SLOT(setParamC(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        return KAutoGamma(m_rou, m_scaleParam, m_paramA, m_paramB, m_paramC)(_image, _width, _height, _limit);
    }
private:
    double m_rou;
    double m_scaleParam;
    double m_paramA;
    double m_paramB;
    double m_paramC;
};

#endif // QAUTOGAMMA

