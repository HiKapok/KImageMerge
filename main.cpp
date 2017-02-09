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

//    KFixPointAnalyse a(320,256, 1., 1., -10., 10., 2.5, -19.5);
//    a.BuildFileList();
//    std::ostringstream os;
//    for(uint16_t hang = 0;hang<=6;hang+=6){
//        for(uint16_t x = 10;x<=300;x+=50){
//            for(uint16_t y = 10;y<=250;y+=50){
//                os.str("");
//                os<<"D://A_H_"<<x<<"-"<<y<<"_"<<hang<<".txt";
//                a.DrawCurve(KFixPointAnalyse::LIST_HORIZONTAL, os.str(), hang, std::make_tuple(x,y));

//            }
//        }

//    }

    KCurveAnalyse a(320,256, 1., 1., -10., 10., 2.5, -19.5, 6.25, 5.);
    //KCurveAnalyse a(320,256, 0.5, 0.575, -10., 10., 3., -20., 6.25, 5.);
    std::vector<std::vector<uint16_t>> points;
    if(a.get_filelist()){
        a.get_pixel_point(std::make_tuple(672,669),std::make_tuple(100,100),std::cout,KCurveAnalyse::LIST_HORIZONTAL);
        //a.get_pixel_list(std::make_tuple(193,679), points);
        a.draw_curve("D://temp.txt");
    }
    return 0;

    MainWindow wnd;
    wnd.show();
    return app.exec();
}
