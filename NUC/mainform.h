#ifndef MAINFORM_H
#define MAINFORM_H

#include <QWidget>
#include "mainwindow.h"
#include "qlineradjust.h"
#include "qadjust.h"
#include "qnakaadjust.h"
#include "qgammaadjust.h"
#include "qglapowadjust.h"
#include "qhomomorphicfilter.h"
#include "qmaskdodadjust.h"
#include "qwallisadjust.h"
#include "qhomofilter2.h"
#include "qhomofilter3.h"
#include "qhomofilter4.h"
#include "qautogamma.h"
#include "qestimatebkg.h"
#include "qlocalgammaadjust.h"
#include "qgumadjust.h"
#include "qmultiadjust.h"
#include "qbkgfit.h"
#include "qgammafit.h"

namespace Ui {
class MainForm;
}

class MainForm : public QWidget
{
    Q_OBJECT

public:
    explicit MainForm(QWidget *parent = 0);
    ~MainForm();
private slots:
    void doAdjust(){
        MainWindow *wnd = new QAdjust(this);
        wnd->show();
    }
    void doLinearAdjust(){
        MainWindow *wnd = new QLinearAdjust(this);
        wnd->show();
    }
    void doGammaAdjust(){
        MainWindow *wnd = new QGammaAdjust(this);
        wnd->show();
    }
    void doHomomorphicFilter(){
        MainWindow *wnd = new QHomomorphicFilter(this);
        wnd->show();
    }
    void doWallisAdjust(){
        MainWindow *wnd = new QWallisAdjust(this);
        wnd->show();
    }
    void doNakaAdjust(){
        MainWindow *wnd = new QNakaAdjust(this);
        wnd->show();
    }
    void doMaskDodAdjust(){
        MainWindow *wnd = new QMaskDodAdjust(this);
        wnd->show();
    }
    void doGlapowAdjust(){
        MainWindow *wnd = new QGlapowAdjust(this);
        wnd->show();
    }
    void doHomoFilter2(){
        MainWindow *wnd = new QHomoFilter2(this);
        wnd->show();
    }
    void doHomoFilter3(){
        MainWindow *wnd = new QHomoFilter3(this);
        wnd->show();
    }
    void doHomoFilter4(){
        MainWindow *wnd = new QHomoFilter4(this);
        wnd->show();
    }
    void doAutoGamma(){
        MainWindow *wnd = new QAutoGamma(this);
        wnd->show();
    }

    void doEstimateBkg(){
        MainWindow *wnd = new QEstimateBkg(this);
        wnd->show();
    }
    void doLocalGamma(){
        MainWindow *wnd = new QLocalGamma(this);
        wnd->show();
    }
    void doGUM(){
        MainWindow *wnd = new QGUMAdjust(this);
        wnd->show();
    }
    void doMultiAdjust(){
        MainWindow *wnd = new QMultiAdjust(this);
        wnd->show();
    }
    void doBkgFit(){
        MainWindow *wnd = new QBkgFit(this);
        wnd->show();
    }
    void doGammaFit(){
        MainWindow *wnd = new QGammaFit(this);
        wnd->show();
    }

private:
    Ui::MainForm *ui;
};

#endif // MAINFORM_H
