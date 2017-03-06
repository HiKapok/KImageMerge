#ifndef QHOMOFILTER2
#define QHOMOFILTER2

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "KHomoFilter2.h"

class QHomoFilter2 : public MainWindow
{
    Q_OBJECT
public:
    QHomoFilter2(QWidget* parent= 0, QString _title = QObject::tr("同态滤波（巴特沃斯带阻）矫正算法"))
        : MainWindow(parent)
        , m_gammaHigh(3.5)
        , m_gammaLow(.8)
        , m_shapren(.36)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QHomoFilter2(){}

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

    // QWidget interface
protected:
    virtual void customUI()
    {
        add_input_box("gamma上限：", m_gammaHigh, this,SLOT(setGammaHigh(const QString)));
        add_input_box("gamma下限：", m_gammaLow, this,SLOT(setGammaLow(const QString)));
        add_input_box("锐化系数：", m_shapren, this,SLOT(setSharpen(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        return KHomoFilter2(m_gammaHigh, m_gammaLow, m_shapren)(_image, _width, _height, _limit);
    }
private:
    double m_gammaHigh;
    double m_gammaLow;
    double m_shapren;
};


#endif // QHOMOFILTER2

