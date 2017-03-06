#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QDebug>
#include <QFileDialog>

#include "KRawDblIntImg.h"

namespace Ui {
class MainWindow;
}


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    virtual void customUI();
    // the last param is either 255 or 65535
    virtual bool runCustomAlogrithm(uint16_t *, uint16_t, uint16_t, double){
        return true;
    }
    // forward to fit different type
    template <typename T>
    void add_input_box(const char * text, T value, QObject * obj, const char * slot){
        __add_input_box(text, QObject::tr("%1").arg(value), obj, slot);
    }

    void adjust_size(bool = false);
    void load_image();
    bool run_algorithm();
    void clear_message();
    // set the message of status bar
    void set_message(QString, int32_t _timeout = 2000);

private slots:
    void open_file(){
        m_sOpenFileName = QFileDialog::getOpenFileName(this,
            tr("打开图像"), ".", tr("Image Files (*.DAT *.jpg *.bmp)"));
        if(!(m_sOpenFileName.isEmpty())) load_image();
        else set_message(QObject::tr("提示：未打开任何图像！"));
        //qDebug()<<m_sOpenFileName;
    }
    void get_pre_image();
    void get_next_image();
    void set_colored(bool);
    void image_adjust(){
        set_message(QObject::tr("提示：算法执行中..."), 0);
        run_algorithm();
        clear_message();
    }

protected:
    Ui::MainWindow *ui;
    uint16_t * m_image;
    uint16_t m_iMinWidth;
    uint16_t m_iMinHeight;
    uint16_t m_imageWidth;
    uint16_t m_imageHeight;
    bool m_bOtherImage;
    QString m_sOpenFileName;
    KRawDblIntImg * m_imageContainer;
    void __add_input_box(const char *, QString, QObject *, const char *);

    // QWidget interface
protected:
    virtual void keyReleaseEvent(QKeyEvent *);
};

#endif // MAINWINDOW_H
