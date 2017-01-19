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



    //KCurveAnalyse a(320,256, 1., 1., -10., 10., 2.5, -19.5, 6.25, 5.);
    KCurveAnalyse a(320,256, 0.5, 0.575, -10., 10., 3., -20., 6.25, 5.);
    std::vector<std::vector<uint16_t>> points;
    if(a.get_filelist()){
        a.get_pixel_point(std::make_tuple<uint16_t, uint16_t>(793,679),std::make_tuple<uint16_t, uint16_t>(100,100),std::cout);
        //a.get_pixel_list(std::make_tuple<uint16_t, uint16_t>(193,679), points);
    }
    return 0;
    //QRegularExpression re((QObject::tr("_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})_")));
    //QRegularExpression re((QObject::tr("_(\\-{0,1}[0-9]{1,2}\\.[0-9]{3})\\.DAT")));

    MainWindow wnd;
    wnd.show();
    return app.exec();
}
