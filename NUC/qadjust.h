#ifndef QADJUST
#define QADJUST

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "KAdjust.h"

class QAdjust : public MainWindow
{
    Q_OBJECT
public:
    QAdjust(QWidget* parent= 0, QString _title = QObject::tr("局部最大熵矫正算法"))
        : MainWindow(parent)
        , m_bright(10)
        , m_width(11)
        , m_height(11)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QAdjust(){}

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
    // QWidget interface
protected:
    virtual void customUI()
    {
        add_input_box("平均亮度：", m_bright, this,SLOT(setBright(const QString)));
        add_input_box("区块宽：", m_width, this,SLOT(setBlockWidth(const QString)));
        add_input_box("区块高：", m_height, this,SLOT(setBlockHeight(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        qDebug() << "average_bright:" << m_bright<<"block width:"<<m_width<<"block height:"<<m_height;
        return KAdjust(m_bright, m_width, m_height)(_image, _width, _height, _limit);
    }
private:
    uint16_t m_bright;
    uint16_t m_width;
    uint16_t m_height;
};

#endif // QADJUST

