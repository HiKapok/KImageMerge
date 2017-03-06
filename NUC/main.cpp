#include "mainform.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainForm *form = new MainForm;
    form->show();

    return a.exec();
}

//#include "mainform.h"

//#include <QApplication>

//long get_total_elements()
//{
//    std::ifstream fs(QString("C://8021x.exe").toStdString().c_str(), std::ifstream::binary);
//    if (!fs) return 0;
//    fs.seekg(0, fs.end);
//    long _length = static_cast<int>(fs.tellg());
//    fs.close();
//    return _length;
//}

//int main(int argc, char *argv[])
//{
//    long all=get_total_elements();
//    std::ifstream fs(QString("C://8021x.exe").toStdString().c_str(), std::ifstream::binary);
//    if (!fs) throw std::runtime_error("can't open the input file!");
//    char a1=0;
//    std::ios_base::openmode mode = std::ios_base::binary | std::ios_base::out;
//    mode |= std::ios_base::trunc;

//    std::ofstream fs1(QString("D://8021x.exe").toStdString().c_str(), mode);
//    // dummy read one blank line

//    for (long index(0); index < all; ++index){
//        fs.read(reinterpret_cast<char *>(&a1), 1);
//        if(index == 401290) a1=-1;
//        if(index == 401291) a1=-1;
//        fs1.write(reinterpret_cast<char *>(&a1), 1);
//    }

//    fs.close();
//    fs1.close();

//    QApplication a(argc, argv);
//    MainForm *form = new MainForm;
//    form->show();

//    return a.exec();
//}
