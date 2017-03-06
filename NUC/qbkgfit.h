#ifndef QBKGFIT
#define QBKGFIT

#include <QString>
#include <QDebug>

#include "mainwindow.h"
#include "kbkgfit.h"

class QBkgFit : public MainWindow
{
    Q_OBJECT
public:
    QBkgFit(QWidget* parent= 0, QString _title = QObject::tr("多项式背景拟合矫正算法"))
        : MainWindow(parent)
    {
        this->setWindowTitle(_title);
        customUI();
    }

    ~QBkgFit(){}

private slots:

    // QWidget interface
protected:
    virtual void customUI()
    {
        adjust_size();
    }

    // MainWindow interface
public:
    virtual bool runCustomAlogrithm(uint16_t * _image, uint16_t _width, uint16_t _height, double _limit){
        return KBkgFit()(_image, _width, _height, _limit);
    }
private:

};

#endif // QBKGFIT

