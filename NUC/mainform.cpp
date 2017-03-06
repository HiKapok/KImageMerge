#include "mainform.h"
#include "ui_mainform.h"

MainForm::MainForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MainForm)
{
    ui->setupUi(this);
    connect(ui->btnAdjust, SIGNAL(released()), this, SLOT(doAdjust()));
    connect(ui->btnLinearAdjust, SIGNAL(released()), this, SLOT(doLinearAdjust()));
    connect(ui->btnGammaAdjust, SIGNAL(released()), this, SLOT(doGammaAdjust()));
    connect(ui->btnHomomorphicFilter, SIGNAL(released()), this, SLOT(doHomomorphicFilter()));
    connect(ui->btnWallisAdjust, SIGNAL(released()), this, SLOT(doWallisAdjust()));
    connect(ui->btnNakaAdjust, SIGNAL(released()), this, SLOT(doNakaAdjust()));
    connect(ui->btnMaskDodAdjust, SIGNAL(released()), this, SLOT(doMaskDodAdjust()));
    connect(ui->btnGlapowAdjust, SIGNAL(released()), this, SLOT(doGlapowAdjust()));
    connect(ui->btnHomoFilter2, SIGNAL(released()), this, SLOT(doHomoFilter2()));
    connect(ui->btnHomoFilter3, SIGNAL(released()), this, SLOT(doHomoFilter3()));
    connect(ui->btnHomoFilter4, SIGNAL(released()), this, SLOT(doHomoFilter4()));
    connect(ui->btnAutoGamma, SIGNAL(released()), this, SLOT(doAutoGamma()));
    connect(ui->btnEstimateBkg, SIGNAL(released()), this, SLOT(doEstimateBkg()));
    connect(ui->btnLocalGamma, SIGNAL(released()), this, SLOT(doLocalGamma()));
    connect(ui->btnGumAdjust, SIGNAL(released()), this, SLOT(doGUM()));
    connect(ui->btnMultiAdjust, SIGNAL(released()), this, SLOT(doMultiAdjust()));
    connect(ui->btnBkgFitAdjust, SIGNAL(released()), this, SLOT(doBkgFit()));
    connect(ui->btnGammaFitAdjust, SIGNAL(released()), this, SLOT(doGammaFit()));
    ui->btnAdjust->setToolTip(QObject::tr("基于局部最大熵的辐射校正方法-田金文等"));
    ui->btnLinearAdjust->setToolTip(QObject::tr("一种光照不均匀图像的灰度校正方法-林天圆等"));
    ui->btnGammaAdjust->setToolTip(QObject::tr("基于二维伽马函数的光照不均匀图像自适应校正算法-刘志成等"));
    ui->btnGlapowAdjust->setToolTip(QObject::tr("A New Method of Correcting Uneven Illumination Problem in Fundus Image--A.M.Aibinu etc."));
    ui->btnNakaAdjust->setToolTip(QObject::tr("A Combined Method to Detect Retinal Fundus Features--V.Bevilacqua etc."));
    ui->btnMaskDodAdjust->setToolTip(QObject::tr("一种数字航空影像的匀光方法-王密等"));
    ui->btnWallisAdjust->setToolTip(QObject::tr("反差一致性保持的影像匀光算法-周丽雅等"));
    ui->btnHomomorphicFilter->setToolTip(QObject::tr("Practical considerations on color image enhancement using homomorphic filtering--Liviu I.Voicu etc."));
    ui->btnHomoFilter2->setToolTip(QObject::tr("基于同态滤波的图像照度补偿算法研究-周龙杰等"));
    ui->btnHomoFilter3->setToolTip(QObject::tr("光照不均匀图像校正研究-廖广军等"));
    ui->btnHomoFilter4->setToolTip(QObject::tr("同态滤波在光照补偿中的应用-罗海霞等"));
    ui->btnAutoGamma->setToolTip(QObject::tr("照度不均匀图像的自动Gamma灰度校正-储霞等"));
    ui->btnEstimateBkg->setToolTip(QObject::tr("数字图像光照不均匀校正及Matlab实现-孙忠贵等"));
    ui->btnLocalGamma->setToolTip(QObject::tr("照度不均图像的局部幂函数映射增强算法-吴刚等"));
    ui->btnGumAdjust->setToolTip(QObject::tr("A generalized unsharp masking algorithm--GuangDeng etc."));
    ui->btnMultiAdjust->setToolTip(QObject::tr("多方案融合"));
    ui->btnBkgFitAdjust->setToolTip(QObject::tr("减小光强不均对CCD非均匀性校正的影响-苏成志等"));
    ui->btnGammaFitAdjust->setToolTip(QObject::tr("NON-UNIFORM ILLUMINATION CORRECTION IN TRANSMISSION ELECTRON MICROSCOPY--Tolga Tasdizen etc."));
}

MainForm::~MainForm()
{
    delete ui;
}
