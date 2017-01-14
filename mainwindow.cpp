#include "mainwindow.h"

#include <QFileDialog>
#include <QDebug>

#include "ui_mainwindow.h"
#include "kimagecollector.h"
#include "krawdblintimg.h"
#include "kcurveanalyse.h"

#include <sstream>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_hOverlap(0),
    m_vOverlap(0),
    m_hImgCount(21),
    m_vImgCount(23),
    m_tMergeType(KImageMerge::SymmetryPrefer),
    m_bShowLine(false),
    m_bSaveColor(false),
    m_bStartBlue(true),
    m_iColorLevel(25),
    m_ptrBtnGroup(std::make_shared<QButtonGroup>(this))
{
    ui->setupUi(this);
    connect(ui->btnOpenDir, SIGNAL(released()), this, SLOT(get_target_dir()));
    connect(ui->btnSelectPath, SIGNAL(released()), this, SLOT(get_save_name()));
    connect(ui->btnMergeImage, SIGNAL(released()), this, SLOT(do_image_merge()));
    ui->editHOverlap->setText(QString("%1").arg(m_hOverlap));
    ui->editVOverlap->setText(QString("%1").arg(m_vOverlap));
    ui->editHCount->setText(QString("%1").arg(m_hImgCount));
    ui->editVCount->setText(QString("%1").arg(m_vImgCount));
    connect(ui->editHOverlap, SIGNAL(textChanged(const QString &)), this,  SLOT(setHOverlap(const QString &)));
    connect(ui->editVOverlap, SIGNAL(textChanged(const QString &)), this,  SLOT(setVOverlap(const QString &)));
    connect(ui->editHCount, SIGNAL(textChanged(const QString &)), this,  SLOT(setHImgCount(const QString &)));
    connect(ui->editVCount, SIGNAL(textChanged(const QString &)), this,  SLOT(setVImgCount(const QString &)));
    //QButtonGroup * btnGroup = new QButtonGroup(this);
    m_ptrBtnGroup->addButton(ui->rbForward, 1);
    m_ptrBtnGroup->addButton(ui->rbBackward, 2);
    m_ptrBtnGroup->addButton(ui->rbSymmetry, 3);
    m_ptrBtnGroup->addButton(ui->rbCenterReflect, 4);
    ui->rbSymmetry->setChecked(true);
    connect(ui->rbForward, SIGNAL(clicked()), this, SLOT(set_merge_type()));
    connect(ui->rbBackward, SIGNAL(clicked()), this, SLOT(set_merge_type()));
    connect(ui->rbSymmetry, SIGNAL(clicked()), this, SLOT(set_merge_type()));
    connect(ui->rbCenterReflect, SIGNAL(clicked()), this, SLOT(set_merge_type()));
    //connect(ui->cbMergeType, SIGNAL(toggled(bool)), this, SLOT(set_order(bool)));
    connect(ui->cbShowLine, SIGNAL(toggled(bool)), this, SLOT(set_show_line(bool)));
    connect(ui->cbSaveColor, SIGNAL(toggled(bool)), this, SLOT(set_show_color(bool)));
    creat_color_tabel(m_iColorLevel, m_bStartBlue);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::get_target_dir()
{
    QString targetDir = QFileDialog::getExistingDirectory(this, QObject::tr("选择目录"), "..", QFileDialog::ShowDirsOnly | QFileDialog::DontResolveSymlinks);
    if(targetDir.isEmpty() || targetDir.isNull()){
        return;
    }else m_sTarghetDir = targetDir;
    ui->statusBar->showMessage(QObject::tr("Tip：目标目录：") + m_sTarghetDir, 5000);
}

void MainWindow::get_save_name()
{
    QString fileName = QFileDialog::getSaveFileName(this, QObject::tr("选择保存路径"), QObject::tr("."), QObject::tr("Images (*.DAT)"));
    if(fileName.isEmpty() || fileName.isNull()){
        //fileName = QObject::tr("C:\\result.DAT");
        if(m_sSaveName.isEmpty() || m_sSaveName.isNull()) m_sSaveName = QObject::tr("C:\\result.DAT");
    }else m_sSaveName = fileName;
    this->setWindowTitle(QObject::tr("MainWindow--当前保存文件前缀：")+m_sSaveName);
}

void MainWindow::do_image_merge()
{
    ui->statusBar->clearMessage();
    if(m_sTarghetDir.isEmpty() || m_sTarghetDir.isNull() || m_sSaveName.isEmpty() || m_sSaveName.isNull()) return;
    //KImageMerge merge(320, 256, 21, 23, m_hOverlap, m_vOverlap, m_bShowLine);
    if(m_tMergeType == KImageMerge::CenterReflect && (m_hImgCount%2 == 0 || m_vImgCount%2 == 0)){
        ui->statusBar->showMessage(QObject::tr("Tip：中心对称填充要求航向线和俯仰线图像个数必须均为奇数"));
    }
    KImageMerge merge(320, 256, m_hImgCount, m_vImgCount, m_hOverlap, m_vOverlap, m_bShowLine);
    ui->statusBar->showMessage(QObject::tr("Tip：拼命合并中..."));

    if(merge.prepare_files(m_sTarghetDir)){
        merge.merge_image(m_tMergeType, m_sSaveName.toLocal8Bit().data());

        if(m_bSaveColor){
            std::pair<uint32_t, uint32_t> outputSize = merge.getOutputSize();
            std::ostringstream os;
            os<<outputSize.first<<"X"<<outputSize.second;
            QString tempQStr = m_sSaveName.left(m_sSaveName.lastIndexOf(QObject::tr("."))) + QObject::tr("_color_") + QObject::tr(os.str().c_str()) + QObject::tr(".bmp");
            ui->statusBar->showMessage(QObject::tr("Tip：输出彩色图像... 保存文件：") + tempQStr);

            std::string realFileName = merge.getOutputFile();
            uint16_t * outImage = new uint16_t[outputSize.first*outputSize.second];
            KRawDblIntImg::read_image_direct<uint16_t>(realFileName, outputSize.first, outputSize.second, outImage);
            //            qDebug()<<tempQStr;
            QImage *qImage = new QImage(outputSize.first, outputSize.second, QImage::Format_RGB888);
            for (uint32_t iY = 0; iY < outputSize.second; ++iY){
                for (uint32_t iX = 0; iX < outputSize.first; ++iX){
                    qImage->setPixel(iX, iY, m_veColorMap[static_cast<uint8_t>(outImage[iY*outputSize.first + iX] * 1. * m_veColorMap.size() / 65536.)]);
                }
            }
            delete[] outImage;
            qImage->save(tempQStr);
            delete qImage;
        }else{
            std::pair<uint32_t, uint32_t> outputSize = merge.getOutputSize();
            std::ostringstream os;
            os<<outputSize.first<<"X"<<outputSize.second;
            QString tempQStr = m_sSaveName.left(m_sSaveName.lastIndexOf(QObject::tr("."))) + QObject::tr("_gray_") + QObject::tr(os.str().c_str()) + QObject::tr(".bmp");
            ui->statusBar->showMessage(QObject::tr("Tip：输出灰度图像... 保存文件：") + tempQStr);

            std::string realFileName = merge.getOutputFile();
            uint16_t * outImage = new uint16_t[outputSize.first*outputSize.second];
            KRawDblIntImg::read_image_direct<uint16_t>(realFileName, outputSize.first, outputSize.second, outImage);
            //            qDebug()<<tempQStr;

            QImage *qImage = new QImage(outputSize.first, outputSize.second, QImage::Format_RGB32);
            for (uint32_t iY = 0; iY < outputSize.second; ++iY){
                for (uint32_t iX = 0; iX < outputSize.first; ++iX){
                    uint8_t gray = static_cast<uint8_t>(outImage[iY*outputSize.first + iX] / 256.);
                    qImage->setPixel(iX, iY, qRgb(gray,gray,gray));
                }
            }
            delete[] outImage;
            qImage->save(tempQStr);
            delete qImage;
        }
    }
    ui->statusBar->clearMessage();
}

void MainWindow::setHOverlap(const QString str)
{
    bool no_error = true;
    double temp = str.toDouble(&no_error);
    if(!no_error || temp < 0.)
    {
        ui->statusBar->showMessage(QObject::tr("Error：请输入正整数！"), 5000);
        return;
    }
    m_hOverlap = static_cast<uint16_t>(temp);
    ui->statusBar->showMessage(QObject::tr("Tip：点击合并图像以刷新输出！"));
}
void MainWindow::setVOverlap(const QString str)
{
    bool no_error = true;
    double temp = str.toDouble(&no_error);
    if(!no_error || temp < 0.)
    {
        ui->statusBar->showMessage(QObject::tr("Error：请输入正整数！"), 5000);
        return;
    }
    m_vOverlap = static_cast<uint16_t>(temp);
    ui->statusBar->showMessage(QObject::tr("Tip：点击合并图像以刷新输出！"));
}

void MainWindow::setHImgCount(const QString str)
{
    bool no_error = true;
    double temp = str.toDouble(&no_error);
    if(!no_error || temp < 0.)
    {
        ui->statusBar->showMessage(QObject::tr("Error：请输入正整数！"), 5000);
        return;
    }
    m_hImgCount = static_cast<uint16_t>(temp);
    ui->statusBar->showMessage(QObject::tr("Tip：点击合并图像以刷新输出！"));
}

void MainWindow::setVImgCount(const QString str)
{
    bool no_error = true;
    double temp = str.toDouble(&no_error);
    if(!no_error || temp < 0.)
    {
        ui->statusBar->showMessage(QObject::tr("Error：请输入正整数！"), 5000);
        return;
    }
    m_vImgCount = static_cast<uint16_t>(temp);
    ui->statusBar->showMessage(QObject::tr("Tip：点击合并图像以刷新输出！"));
}

void MainWindow::set_merge_type()
{
    switch(m_ptrBtnGroup->checkedId())
    {
    case 1:
       m_tMergeType = KImageMerge::FormerPrefer;
       break;
    case 2:
       m_tMergeType = KImageMerge::LatterPrefer;
       break;
    case 3:
       m_tMergeType = KImageMerge::SymmetryPrefer;
       break;
    case 4:
       m_tMergeType = KImageMerge::CenterReflect;
       break;
    }
}

void MainWindow::set_show_line(bool sts)
{
    m_bShowLine = sts;
}

void MainWindow::set_show_color(bool sts)
{
    m_bSaveColor = sts;
}
