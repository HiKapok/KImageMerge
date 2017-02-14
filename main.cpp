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

//    KFixPointAnalyse a(320,256, 0.5, 0.575, -10., 10., 3., -20.);
//    a.BuildFileList();
//    std::ostringstream os;
//    for(uint16_t hang = 20;hang<=20;hang+=6){
//        for(uint16_t x = 10;x<=300;x+=50){
//            for(uint16_t y = 10;y<=250;y+=50){
//                os.str("");
//                os<<"D://C_H_"<<x<<"-"<<y<<"_"<<hang<<".txt";
//                a.DrawCurve(KFixPointAnalyse::LIST_HORIZONTAL, os.str(), hang, std::make_tuple(x,y));

//            }
//        }

//    }
//return 0;

    KCurveAnalyse a(320,256, 1., 1., -10., 10., 2.5, -19.5, 6.25, 5.);
    //KCurveAnalyse a(320,256, 0.5, 0.575, -10., 10., 3., -20., 6.25, 5.);
    //std::vector<std::vector<uint16_t>> points;
    if(a.get_filelist()){
        a.get_pixel_point(std::make_tuple(72,691), std::make_tuple(100,100), KCurveAnalyse::LIST_HORIZONTAL);
        //a.get_pixel_list(std::make_tuple(193,679), points);
        a.draw_curve("D://temp.txt");

//        float temp = -2.5;
//        std::ostringstream os("", std::ios_base::ate);
//        os.precision(3);
//        os.setf(std::ios::fixed);
//        std::vector<std::string> verTemp;
//        for(int a = 16;a<22;a++){
//            os.str(QObject::tr("D:/HustProjects/[非密]测试图像/环境40度黑体30度2016年06月08日13时08分02秒/航向线/AutoLvdsImage_").toLocal8Bit().constData());
//            os<<a<<"_"<<temp<<"_-8.500.DAT";
//            temp+=0.5;
//            verTemp.push_back(os.str());
//           //std::cout<< os.str()<<std::endl;
//            //a.get_pixel_gray();
//        }
//        for(int a = 25;a<32;a++){
//            os.str(QObject::tr("D:/HustProjects/[非密]测试图像/环境40度黑体30度2016年06月08日13时08分02秒/航向线/AutoLvdsImage_").toLocal8Bit().constData());
//            os<<a<<"_"<<temp<<"_-8.500.DAT";
//            temp+=0.5;
//            verTemp.push_back(os.str());
//           //std::cout<< os.str()<<std::endl;
//            //a.get_pixel_gray();
//        }
//        int count = 15;
//        uint16_t pixel_value = 0;
//        for(auto &item : verTemp){
//            if(a.get_pixel_gray(item,std::make_tuple(693 - count*25 , 99), pixel_value, true)){
//            qDebug()<<pixel_value;
//            }

//            ++count;
//        }


    }
    return 0;

    MainWindow wnd;
    wnd.show();
    return app.exec();
}
