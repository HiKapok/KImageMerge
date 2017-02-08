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

#include <unordered_map>


class KFixPointAnalyse
{
public:
    typedef enum{
        LIST_VERTICAL = 0,
        LIST_HORIZONTAL = 1
    } outputformat;
    KFixPointAnalyse(uint16_t _width, uint16_t _height, double _viewWStep=1., double _viewHStep=1.,
                     double _oaWidthLow=-10., double _oaWidthHigh=10., double _oaHeightLow=2.5, double _oaHeightHigh=-19.5);
    bool BuildFileList();
    bool DrawCurve(outputformat, std::string, uint16_t, std::tuple<uint16_t, uint16_t>);
private:
    uint16_t m_iImgWidth;
    uint16_t m_iImgHeight;
    double m_fWStepAngle;
    double m_fHStepAngle;
    uint16_t m_iTotalWCount;
    uint16_t m_iTotalHCount;
    double m_fOAWidthLow;
    double m_fOAWidthHigh;
    double m_fOAHeightLow;
    double m_fOAHeightHigh;
    std::unordered_map<uint32_t, std::string> m_mFileBuff;
    bool get_point_value(std::string, std::tuple<uint16_t,uint16_t>, uint16_t&, bool=true);
    bool DumpPoint2File(outputformat, std::string, uint16_t, std::tuple<uint16_t, uint16_t>);

};

#endif // KFIXPOINTANALYSE_H
