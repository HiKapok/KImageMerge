#ifndef QGLAPOWADJUST
#define QGLAPOWADJUST

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "KGlapowAdjust.h"

class QGlapowAdjust : public MainWindow
{
    Q_OBJECT
public:
    QGlapowAdjust(QWidget* parent= 0, QString _title = QObject::tr("Global-Local自适应直方图均衡化矫正算法"))
        : MainWindow(parent)
        , m_bright(32768)
        , m_kernelSize(5)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QGlapowAdjust(){}

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

    // QWidget interface
protected:
    virtual void customUI()
    {
        add_input_box("平均亮度：", m_bright, this,SLOT(setBright(const QString)));
        add_input_box("窗口大小：", m_kernelSize, this,SLOT(setKernelSize(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        return KGlapowAdjust(m_bright, m_kernelSize)(_image, _width, _height, _limit);
    }
private:
    uint16_t m_bright;
    uint16_t m_kernelSize;
};

#endif // QGLAPOWADJUST

