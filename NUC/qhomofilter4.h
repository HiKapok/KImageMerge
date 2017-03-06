#ifndef QHOMOFILTER4_H
#define QHOMOFILTER4_H

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "khomofilter4.h"

class QHomoFilter4 : public MainWindow
{
    Q_OBJECT
public:
    QHomoFilter4(QWidget* parent= 0, QString _title = QObject::tr("同态滤波（带阻自动）矫正算法"))
        : MainWindow(parent)
        , m_shapren(1.0)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QHomoFilter4(){}

private slots:

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
        add_input_box("锐化系数：", m_shapren, this,SLOT(setSharpen(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        return KHomoFilter4(m_shapren)(_image, _width, _height, _limit);
    }
private:
    double m_shapren;
};

#endif // QHOMOFILTER4_H

