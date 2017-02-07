#ifndef KFIXPOINTANALYSE_H
#define KFIXPOINTANALYSE_H

#include <QString>

#include <cstdint>
#include <tuple>
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <sstream>
#include <fstream>


class KFixPointAnalyse
{
public:
    typedef enum{
        LIST_VERTICAL = 0,
        LIST_HORIZONTAL = 1
    } outputformat;
    KFixPointAnalyse(uint16_t _width, uint16_t _height, double _viewWStep=1., double _viewHStep=1.,
                     double _oaWidthLow=-10., double _oaWidthHigh=10., double _oaHeightLow=2.5, double _oaHeightHigh=-19.5);
private:
    uint16_t m_iImgWidth;
    uint16_t m_iImgHeight;
    double m_fWStepAngle;
    double m_fHStepAngle;
    uint16_t m_iTotalWidth;
    uint16_t m_iTotalHeight;
    uint16_t m_iTotalWCount;
    uint16_t m_iTotalHCount;
};

#endif // KFIXPOINTANALYSE_H
