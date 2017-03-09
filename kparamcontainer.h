#ifndef KPARAMCONTAINER_H
#define KPARAMCONTAINER_H

#include <mutex>
#include <iostream>
#include <fstream>
#include <functional>
#include <cstdint>
#include <string>
#include <vector>
#include <cmath>


class KParamContainer{

public:
    static KParamContainer &getInstance(){
        std::lock_guard<std::mutex> locker(m_mutex);
        static KParamContainer instance;
        std::call_once(m_flag, std::bind(setParamFromFile, &instance));
        return instance;
    }
    void setParamFromFile(){
        std::ifstream ifile(std::string("param.ini"));
        double view_width = 0.;
        double view_height = 0.;
        if(ifile.is_open()){
            std::string temp("");
            uint16_t num_args = 10;
            std::vector<double> temp_args;
            temp_args.reserve(num_args);
            while(num_args-->0){
                std::getline(ifile, temp);
                try{
                    temp_args.push_back( std::stod(temp.substr(0, temp.find_first_of('#'))) );
                }catch(std::invalid_argument&){
                    std::cerr << "invalid_argument @line " << 10-num_args << ", assume 0." <<std::endl;
                    temp_args.push_back(0.);
                }
            }
            m_iImgWidth = static_cast<uint16_t>(temp_args[0]);
            m_iImgHeight = static_cast<uint16_t>(temp_args[1]);
            m_fWStepAngle = temp_args[2];
            m_fHStepAngle = temp_args[3];
            m_fOAWidthLow = temp_args[4];
            m_fOAWidthHigh = temp_args[5];
            m_fOAHeightLow = temp_args[6];
            m_fOAHeightHigh = temp_args[7];
            view_width = temp_args[8];
            view_height = temp_args[9];
        }else{
            std::cerr << "miss param file" << ", load default." <<std::endl;
            m_iImgWidth = 320;
            m_iImgHeight = 256;
            m_fWStepAngle = 1.;
            m_fHStepAngle = 1.;
            m_fOAWidthLow = -10.;
            m_fOAWidthHigh = 10.;
            m_fOAHeightLow = 2.5;
            m_fOAHeightHigh = -19.5;
            view_width = 6.25;
            view_height = 5.;
        }

        m_iWStep = static_cast<uint16_t>(m_fWStepAngle*m_iImgWidth/view_width);
        m_iHStep = static_cast<uint16_t>(m_fHStepAngle*m_iImgHeight/view_height);
        m_iTotalWidth = static_cast<uint16_t>((std::fabs(m_fOAWidthLow-m_fOAWidthHigh)+view_width)*m_iImgWidth/view_width);
        m_iTotalHeight = static_cast<uint16_t>((std::fabs(m_fOAHeightLow-m_fOAHeightHigh)+view_height)*m_iImgHeight/view_height);
        m_iTotalWCount = static_cast<uint16_t>(std::fabs(m_fOAWidthLow-m_fOAWidthHigh)/m_fWStepAngle+1);
        m_iTotalHCount = static_cast<uint16_t>(std::fabs(m_fOAHeightLow-m_fOAHeightHigh)/m_fHStepAngle+1);
    }

    uint16_t m_iImgWidth;
    uint16_t m_iImgHeight;
    double m_fWStepAngle;
    double m_fHStepAngle;
    uint16_t m_iWStep;
    uint16_t m_iHStep;
    uint16_t m_iTotalWidth;
    uint16_t m_iTotalHeight;
    uint16_t m_iTotalWCount;
    uint16_t m_iTotalHCount;
    double m_fOAWidthLow;
    double m_fOAWidthHigh;
    double m_fOAHeightLow;
    double m_fOAHeightHigh;
private:
    KParamContainer()=default;
    KParamContainer(KParamContainer&)=default;
    KParamContainer(KParamContainer&&)=default;
    KParamContainer& operator=(const KParamContainer&)=default;
    KParamContainer& operator=(KParamContainer&&)=default;
    static std::mutex m_mutex;
    static std::once_flag m_flag;
};

#endif // KPARAMCONTAINER_H
