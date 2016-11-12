#include "mainwindow.h"

#include <QApplication>
#include <QString>
#include <QFileDialog>
#include <QObject>

#include <QDebug>



int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow wnd;
    wnd.show();
    return app.exec();
}
