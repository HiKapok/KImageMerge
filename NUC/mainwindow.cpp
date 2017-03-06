#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <chrono>
#include <cstdint>

#include <QLineEdit>
#include <QDesktopWidget>
#include <QDir>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
    , m_image(nullptr)
    , m_iMinWidth(400)
    , m_iMinHeight(0)
    , m_imageWidth(0)
    , m_imageHeight(0)
    , m_bOtherImage(false)
    , m_sOpenFileName("")
    , m_imageContainer(nullptr)
{
    ui->setupUi(this);
    connect(ui->btnOpenFile, SIGNAL(released()), this, SLOT(open_file()));
    connect(ui->btnPreImage, SIGNAL(released()), this, SLOT(get_pre_image()));
    connect(ui->btnNextImage, SIGNAL(released()), this, SLOT(get_next_image()));
    connect(ui->btnAjust, SIGNAL(released()), this, SLOT(image_adjust()));
    connect(ui->ifShowColor, SIGNAL(toggled(bool)), this, SLOT(set_colored(bool)));
    set_message(QObject::tr("提示：打开图像以进行处理！"));
}

void MainWindow::customUI(){

}

void MainWindow::__add_input_box(const char* label_name, QString default_value, QObject * obj, const char * _slot){

    QLabel *label = new QLabel(ui->layoutWidget);
    uint16_t num_count = ui->gridLayout->count();

    label->setText(QApplication::translate("MainWindow", label_name, 0));
    label->setWordWrap(true);
    label->setMinimumSize(70,15);
    label->setMaximumHeight(15);
    label->setAlignment(Qt::AlignRight);
    ui->gridLayout->addWidget(label, num_count / 4, num_count % 4, 1, 1);

    QLineEdit *lineEdit = new QLineEdit(ui->layoutWidget);
    lineEdit->setText(default_value);
    ++num_count;
    lineEdit->setAlignment(Qt::AlignLeft);
    lineEdit->setMinimumSize(70,15);
    lineEdit->setMaximumSize(70,15);
    ui->gridLayout->addWidget(lineEdit, num_count / 4, num_count % 4, 1, 1);
    QObject::connect(lineEdit, SIGNAL(textChanged(const QString &)), obj,  _slot);
}

void MainWindow::adjust_size(bool move2center)
{
    m_iMinWidth = 540;
    m_iMinHeight = 150;
    m_iMinWidth = std::max<uint16_t>(m_iMinWidth, (ui->gridLayout->spacing() * 3 + 2 * (70 + 70)) * 21 / 20);
    m_iMinHeight = std::max<uint16_t>(m_iMinHeight, (ui->gridLayout->spacing() * (ui->gridLayout->count() / 4) + ((ui->gridLayout->count() + 3) / 4) * 15) * 173 / 50);

    if(this->ui->label_image->has_image()){
        m_iMinWidth = std::max<uint16_t>(m_iMinWidth, this->ui->label_image->get_width() * 106 / 50);
        m_iMinHeight = std::max<uint16_t>(m_iMinHeight, this->ui->label_image->get_height() * 173 / 100);
    }else{
        m_iMinWidth = std::max<uint16_t>(m_iMinWidth, this->ui->label_image->width() * 106 / 50);
        m_iMinHeight = std::max<uint16_t>(m_iMinHeight, this->ui->label_image->height() * 173 / 100);
    }

    if(this->ui->label_adjust_image->has_image()){
        m_iMinWidth = std::max<uint16_t>(m_iMinWidth, this->ui->label_adjust_image->get_width() * 106 / 50);
        m_iMinHeight = std::max<uint16_t>(m_iMinHeight, this->ui->label_adjust_image->get_height() * 173 / 100);
    }else{
        m_iMinWidth = std::max<uint16_t>(m_iMinWidth, this->ui->label_adjust_image->width() * 106 / 50);
        m_iMinHeight = std::max<uint16_t>(m_iMinHeight, this->ui->label_adjust_image->height() * 173 / 100);
    }
    //qDebug()<<"m_iMinWidth"<<m_iMinWidth<<"m_iMinHeight"<<m_iMinHeight;
    ui->layoutWidget->setGeometry(QRect(32, 12, m_iMinWidth, m_iMinHeight));//t.width()*106/50, t.height()*173/100));
    ui->layoutWidget->setFixedSize(m_iMinWidth, m_iMinHeight);
    //ui->layoutWidget->adjustSize();
    ui->layoutWidget->updateGeometry();
    this->setFixedSize(m_iMinWidth + 64, m_iMinHeight + 24 + ui->statusBar->height());
    //this->setMinimumSize(m_iMinWidth + 20, m_iMinHeight + 20);
    this->adjustSize();
    //this->updateGeometry();
    QDesktopWidget *desktop=QApplication::desktop();
    int32_t desktop_width=desktop->width();
    int32_t desktop_height=desktop->height();
    if(desktop_width > this->width()) desktop_width = (desktop_width - this->width()) / 2;
    else desktop_width = 0;
    if(desktop_height > this->height()) desktop_height = (desktop_height - this->height()) / 2;
    else desktop_height = 0;
    if(move2center) this->move(desktop_width, desktop_height);
    //delete desktop;

}

void MainWindow::get_pre_image(){
    if(m_bOtherImage) set_message(QObject::tr("提示：只有一张图像！"));
    if(m_bOtherImage || nullptr == m_imageContainer) return;
    if(m_imageContainer->isvalid()){
        uint16_t * old_image = m_image;
        m_image = new uint16_t[320 * 256];
        if(m_imageContainer->get_last_image(m_image)){
            if(old_image) delete [] old_image;
            this->ui->label_image->setImage(m_image, 320, 256, 65536.);
            this->ui->label_adjust_image->setImage(m_image, 320, 256, 65536.);
            m_imageWidth = 320;
            m_imageHeight = 256;
            //suanfa
            run_algorithm();
            adjust_size();
        }else{
            if(m_image) delete[] m_image;
            m_image = old_image;
            set_message(QObject::tr("提示：没有上一张了！"));
        }
    }
}

void MainWindow::get_next_image(){
    if(m_bOtherImage) set_message(QObject::tr("提示：只有一张图像！"));
    if(m_bOtherImage || nullptr == m_imageContainer) return;
    if(m_imageContainer->isvalid()){
        uint16_t * old_image = m_image;
        m_image = new uint16_t[320 * 256];
        if(m_imageContainer->get_next_image(m_image)){
            if(old_image) delete [] old_image;
            this->ui->label_image->setImage(m_image, 320, 256, 65536.);
            this->ui->label_adjust_image->setImage(m_image, 320, 256, 65536.);
            m_imageWidth = 320;
            m_imageHeight = 256;
            //suanfa
            run_algorithm();
            adjust_size();
        }else{
            if(m_image) delete[] m_image;
            m_image = old_image;
            set_message(QObject::tr("提示：没有下一张了！"));
        }
    }
}

void MainWindow::set_colored(bool _sts)
{
    this->ui->label_image->setColored(_sts);
    this->ui->label_adjust_image->setColored(_sts);
}

void MainWindow::load_image()
{
    bool old_sts = m_bOtherImage;
    m_sOpenFileName = QDir::toNativeSeparators(m_sOpenFileName);
    if(".DAT" == m_sOpenFileName.toStdString().substr(m_sOpenFileName.toStdString().find_last_of("."))){
        m_bOtherImage = false;
    }else m_bOtherImage = true;

    if(m_bOtherImage){
        QImage temp_image;
        temp_image.load(m_sOpenFileName);
        if(temp_image.isNull())
        {
            m_bOtherImage = old_sts;
            return;
        }
        if(m_imageContainer) delete m_imageContainer;
        m_imageContainer = nullptr;
        if(m_image) delete [] m_image;
        m_imageWidth = temp_image.width();
        m_imageHeight = temp_image.height();
        m_image = new uint16_t[temp_image.width() * temp_image.height()];
        for (int iY = 0; iY < temp_image.height(); ++iY){
            for (int iX = 0; iX < temp_image.width(); ++iX){
                m_image[iY*temp_image.width() + iX] = static_cast<uint8_t>(temp_image.pixel(iX,iY));
            }
        }
        this->ui->label_image->setImage(m_image, temp_image.width(), temp_image.height(), 256.);
        this->ui->label_adjust_image->setImage(m_image, temp_image.width(), temp_image.height(), 256.);

    }
    else{
        KRawDblIntImg * old_comtainer = m_imageContainer;
        uint16_t * old_image = m_image;
        m_imageContainer = new KRawDblIntImg(m_sOpenFileName.toLocal8Bit().data(), 320, 256);
        if(m_imageContainer->isvalid()){
            if(old_comtainer) delete old_comtainer;
            m_image = new uint16_t[320 * 256];
            m_imageContainer->image_buff_reset();
            if(m_imageContainer->get_next_image(m_image)){
                if(old_image) delete [] old_image;
                m_imageWidth = 320;
                m_imageHeight = 256;
                this->ui->label_image->setImage(m_image, 320, 256, 65536.);
                this->ui->label_adjust_image->setImage(m_image, 320, 256, 65536.);
            }else{
                if(m_image) delete[] m_image;
                m_image = old_image;
                m_bOtherImage = old_sts;
                if(m_imageContainer) delete m_imageContainer;
                m_imageContainer = old_comtainer;
                return;
            }
        }else{
            m_bOtherImage = old_sts;
            if(m_imageContainer) delete m_imageContainer;
            m_imageContainer = old_comtainer;
            return;
        }
    }
    set_message(QObject::tr("提示：载入图像成功！图像路径：") + m_sOpenFileName + QObject::tr(" 输出刷新中..."));
    run_algorithm();
    adjust_size(true);
}

bool MainWindow::run_algorithm()
{
    bool _result = false;

    if(!(this->ui->label_image->has_image())) return false;
    uint16_t * temp_image = new (std::nothrow) uint16_t[m_imageWidth * m_imageHeight];
    if(nullptr == temp_image) return false;

    memcpy(temp_image, m_image, sizeof(uint16_t) * m_imageWidth * m_imageHeight);

    std::chrono::time_point<std::chrono::steady_clock> _startTime = std::chrono::steady_clock::now();

    if(m_bOtherImage) _result = runCustomAlogrithm(temp_image, m_imageWidth, m_imageHeight, 255.);
    else _result = runCustomAlogrithm(temp_image, m_imageWidth, m_imageHeight, 65535.);

    std::chrono::duration<double> time_diff = std::chrono::steady_clock::now()- _startTime;
    ui->elapsedTime->setText(QString("%1 ms").arg(time_diff.count()*1000.));

    if(!m_bOtherImage) this->ui->label_adjust_image->setImage(temp_image, m_imageWidth, m_imageHeight, 65536.);
    else this->ui->label_adjust_image->setImage(temp_image, m_imageWidth, m_imageHeight, 256.);

    delete [] temp_image;

    return _result;
}

void MainWindow::clear_message()
{
    ui->statusBar->clearMessage();
}

void MainWindow::set_message(QString mes, int32_t time_out)
{
    ui->statusBar->showMessage(mes, time_out);
}

MainWindow::~MainWindow()
{
    delete ui;
    if(m_image) delete [] m_image;
    m_image = nullptr;
    if(m_imageContainer) delete m_imageContainer;
    m_imageContainer = nullptr;
}

void MainWindow::keyReleaseEvent(QKeyEvent * keyEvent)
{
    if(keyEvent->key() == Qt::Key_Enter || keyEvent->key() == Qt::Key_Return){
        set_message(QObject::tr("提示：算法执行中..."), 0);
        run_algorithm();
        clear_message();
    }
}
