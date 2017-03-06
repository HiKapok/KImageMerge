#ifndef QLOCALGAMMAADJUST
#define QLOCALGAMMAADJUST

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "KLocalGammaAdjust.h"

class QLocalGamma : public MainWindow
{
    Q_OBJECT
public:
    QLocalGamma(QWidget* parent= 0, QString _title = QObject::tr("局部Gamma矫正算法"))
        : MainWindow(parent)
        , m_bright(32768)
        , m_width(5)
        , m_height(5)
        , m_wOverlap(3)
        , m_hOverlap(3)
        , m_thresMean(100.)
        , m_thresSigma(110.)
        , m_gammaRatio(.8)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QLocalGamma(){}

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
    void setOverlapWidth(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入整数！"));
            return;
        }
        m_wOverlap = static_cast<uint16_t>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setOverlapHeight(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入整数！"));
            return;
        }
        m_hOverlap = static_cast<uint16_t>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setGammaRatio(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入小数！"));
            return;
        }
        m_gammaRatio = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setMeanThres(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入小数！"));
            return;
        }
        m_thresMean = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    void setSigmaThres(const QString str){
        bool no_error = true;
        double temp = str.toDouble(&no_error);
        if(!no_error)
        {
            set_message(QObject::tr("错误：请输入小数！"));
            return;
        }
        m_thresSigma = static_cast<double>(temp);
        set_message(QObject::tr("提示：点击图像复原以刷新输出！"));
    }
    // QWidget interface
protected:
    virtual void customUI()
    {
        add_input_box("平均亮度：", m_bright, this, SLOT(setBright(const QString)));
        add_input_box("区块宽：", m_width, this, SLOT(setBlockWidth(const QString)));
        add_input_box("区块高：", m_height, this, SLOT(setBlockHeight(const QString)));
        add_input_box("重叠区块宽：", m_wOverlap, this, SLOT(setOverlapWidth(const QString)));
        add_input_box("重叠区块高：", m_hOverlap, this, SLOT(setOverlapHeight(const QString)));
        add_input_box("均值阈值：", m_thresMean, this, SLOT(setMeanThres(const QString)));
        add_input_box("方差阈值：", m_thresSigma, this, SLOT(setSigmaThres(const QString)));
        add_input_box("Gamma系数：", m_gammaRatio, this, SLOT(setGammaRatio(const QString)));
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        if(m_wOverlap+1>m_width || m_hOverlap+1>m_height){
            set_message(QObject::tr("错误：重叠区域大小不能大于分块区域大小！"));
            return false;
        }
        return KLocalGammaAdjust(m_bright, m_width, m_height, m_wOverlap, m_hOverlap, m_gammaRatio, m_thresMean, m_thresSigma)(_image, _width, _height, _limit);
    }
private:
    uint16_t m_bright;
    uint16_t m_width;
    uint16_t m_height;
    uint16_t m_wOverlap;
    uint16_t m_hOverlap;
    double m_thresMean;
    double m_thresSigma;
    double m_gammaRatio;
};

#endif // QLOCALGAMMAADJUST

