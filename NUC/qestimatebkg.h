#ifndef QESTIMATEBKG
#define QESTIMATEBKG

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "kestimatebkg.h"

class QEstimateBkg : public MainWindow
{
    Q_OBJECT
public:
    QEstimateBkg(QWidget* parent= 0, QString _title = QObject::tr("基于概率的背景估计矫正算法"))
        : MainWindow(parent)
        , m_width(10)
        , m_height(10)
        , m_kernelSize(3)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QEstimateBkg(){}

private slots:
    void setBlockWidth(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入整数！"));
            return;
        }
        m_width = static_cast<uint16_t>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setBlockHeight(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入整数！"));
            return;
        }
        m_height = static_cast<uint16_t>(temp);
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
        add_input_box("区块宽度：", m_width, this, SLOT(setBlockWidth(const QString)));
        add_input_box("区块高度：", m_height, this, SLOT(setBlockHeight(const QString)));
        add_input_box("中值滤波核大小：", m_kernelSize, this, SLOT(setKernelSize(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        return KEstimateBkg(m_width, m_height, m_kernelSize)(_image, _width, _height, _limit);
    }
private:
    uint16_t m_width;
    uint16_t m_height;
    uint16_t m_kernelSize;
};

#endif // QESTIMATEBKG

