#include "mainwindow.h"

#include <QApplication>
#include <QString>
#include <QFileDialog>
#include <QObject>

#include <QDebug>

#include <QRegularExpression>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    //QRegularExpression re((QObject::tr("_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})_")));
    //QRegularExpression re((QObject::tr("_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})\\.DAT")));

    MainWindow wnd;
    wnd.show();
    return app.exec();
}
