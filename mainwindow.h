#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QString>
#include <QImage>
#include <QColor>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    uint16_t creat_color_tabel(uint8_t _step, bool _b_start_blue)
    {
        int iB = 255;
        int iG = 0;
        int iR = 160;
        m_veColorMap.clear();
        if (_b_start_blue) iR = _step;
        else m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        for (iR -= _step; iR > 0; iR -= _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        if (std::abs(iR) > 2 * _step / 3) { iR = 0; iG += _step; }
        for (iR = 0; iG < 128; iG += _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        for (; iG < 255 && iB > 0; iG += _step, iB -= 2 * _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        if (std::abs(iG - 255) > 2 * _step / 3 && std::abs(iB) > 2 * _step / 3) { iG = 255; iB = 0; iR += 2 * _step; }
        for (iG = 255, iB = 0; iR < 255; iR += 2 * _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        if (std::abs(iR - 255) > 2 * _step / 3) { iR = 255; iG -= _step; }
        for (iR = 255; iG > 0; iG -= _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));
        for (iG = 255 / 5, iR -= 2 * _step; iR > 2 * iG; iR -= _step) m_veColorMap.push_back(qRgb(static_cast<uchar>(iR), static_cast<uchar>(iG), static_cast<uchar>(iB)));

        return m_veColorMap.size();
    }

private slots:
    void get_target_dir();
    void get_save_name();
    void do_image_merge();
    void setHOverlap(const QString str);
    void setVOverlap(const QString str);
    void set_order(bool);
    void set_show_line(bool);
    void set_show_color(bool);

private:
    Ui::MainWindow *ui;
    QString m_sTarghetDir;
    QString m_sSaveName;
    uint16_t m_hOverlap;
    uint16_t m_vOverlap;
    bool m_bPreferLatter;
    bool m_bShowLine;
    bool m_bSaveColor;
    bool m_bStartBlue;
    uint16_t m_iColorLevel;
    std::vector<QRgb> m_veColorMap;
};

#endif // MAINWINDOW_H
