#include "mainwindow.h"

#include <QApplication>
#include <QString>
#include <QFileDialog>
#include <QObject>

#include <QDebug>

#include <QRegularExpression>


#include "kcurveanalyse.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    KCurveAnalyse a(320,256);
    std::vector<std::vector<uint16_t>> points;
    if(a.get_filelist()){
        a.get_pixel_list(std::make_tuple<uint16_t, uint16_t>(673,689), points);
    }
    //QRegularExpression re((QObject::tr("_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})_")));
    //QRegularExpression re((QObject::tr("_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})\\.DAT")));

    MainWindow wnd;
    wnd.show();
    return app.exec();
}
