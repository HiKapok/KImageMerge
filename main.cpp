#include "mainwindow.h"

#include <QApplication>
#include <QString>
#include <QFileDialog>
#include <QObject>
#include <QDebug>
#include <QRegularExpression>


#include "kcurveanalyse.h"
#include "kfixpointanalyse.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);



//    KCurveAnalyse a(320,256, 1., 1., -10., 10., 2.5, -19.5, 6.25, 5.);
//    //KCurveAnalyse a(320,256, 0.5, 0.575, -10., 10., 3., -20., 6.25, 5.);
//    std::vector<std::vector<uint16_t>> points;
//    if(a.get_filelist()){
//        a.get_pixel_point(std::make_tuple(672,669),std::make_tuple(100,100),std::cout,KCurveAnalyse::LIST_HORIZONTAL);
//        //a.get_pixel_list(std::make_tuple(193,679), points);
//        a.draw_curve("D://temp.txt");
//    }
    return 0;

    MainWindow wnd;
    wnd.show();
    return app.exec();
}
