#ifndef KCURVEANALYSE_H
#define KCURVEANALYSE_H

#include <QString>

#include <cstdint>
#include <tuple>
#include <iostream>
#include <vector>
#include <algorithm>
#include <cmath>
#include <string>
#include <sstream>

class KCurveAnalyse
{
public:
    KCurveAnalyse(uint16_t _width, uint16_t _height, double _viewWidth=6.25, double _viewHiehgt=5.,
                  double _oaWidthLow=-10., double _oaWidthHigh=10., double _oaHeightLow=2.5, double _oaHeightHigh=-19.5);
    /* invoke this yourself */
    bool get_filelist();
    /* invoke this to get pixel value list related to the position */
    bool get_pixel_list(std::tuple<uint16_t, uint16_t>, std::vector<std::vector<uint16_t>>&);

private:
    uint16_t m_iImgWidth;
    uint16_t m_iImgHeight;
    uint16_t m_iStep;
    uint16_t m_itotalWidth;
    uint16_t m_itotalHeight;
    double m_fOAWidthLow;
    double m_fOAWidthHigh;
    double m_fOAHeightLow;
    double m_fOAHeightHigh;
    std::vector<std::string> m_vecFileLists;
    std::vector<uint16_t> m_vecXImageIndex;
    std::vector<uint16_t> m_vecYImageIndex;

    /* translate the pixel position in the whole big image to a vector of image filenames in which contains this pixel */
    bool pixel2file(uint16_t, uint16_t, std::vector<std::string>&);
    /* translate the pixel position in the whole big image to a vector of image indexes in which contains this pixel */
    uint16_t pixel2imagenum(uint16_t, uint16_t, uint16_t, std::vector<uint16_t>&);

    /* don't call this */
    bool get_pixel_gray(std::string, std::tuple<uint16_t, uint16_t>, uint16_t&, bool=false);

    void unittest_pixel2image(){
        for(uint16_t pixel = 0;pixel<m_itotalWidth;++pixel){
            std::vector<uint16_t> lhs;
            std::vector<uint16_t> rhs;
            for(uint16_t index = 0;index<21;++index){
                if(pixel>=index*m_iStep && pixel<m_iImgWidth+index*m_iStep){
                    lhs.push_back(index+1);
                }
            }
            int16_t min_bound = std::min(std::floor((pixel*1.)/m_iStep+1),std::ceil(((pixel*1.)-m_iImgWidth+1)/m_iStep)+1);
            int16_t max_bound = std::max(std::floor((pixel*1.)/m_iStep+1),std::ceil(((pixel*1.)-m_iImgWidth+1)/m_iStep)+1);
            min_bound = std::max(min_bound, static_cast<int16_t>(1));
            max_bound = std::min(max_bound, static_cast<int16_t>(21));
            for(int16_t index = min_bound;index <= max_bound;++index){
                rhs.push_back(static_cast<uint16_t>(index));
            }
            if(lhs.size() != rhs.size()){
                std::cout<<"test failed (@comp size)."<<std::endl;
                std::cout<<"pixel:"<<pixel<<" m_iStep:"<<m_iStep<<std::endl;
                std::cout<<"min_bound:"<<min_bound<<" max_bound:"<<max_bound<<std::endl;

                std::cout<<"left:";
                for(auto item : lhs) std::cout<<item<<" ";
                std::cout<<std::endl;

                std::cout<<"right:";
                for(auto item : rhs) std::cout<<item<<" ";
                std::cout<<std::endl;

                return;
            }
            else{
                for(uint16_t index = 0;index<lhs.size();++index){
                    if(lhs[index] !=rhs[index]){
                        std::cout<<"test failed (@comp item)."<<std::endl;
                        std::cout<<"pixel:"<<pixel<<" m_iStep:"<<m_iStep<<std::endl;
                        std::cout<<"min_bound:"<<min_bound<<" max_bound:"<<max_bound<<std::endl;

                        std::cout<<"left:";
                        for(auto item : lhs) std::cout<<item<<" ";
                        std::cout<<std::endl;

                        std::cout<<"right:";
                        for(auto item : rhs) std::cout<<item<<" ";
                        std::cout<<std::endl;

                        return;
                    }
                }
            }
        }
        std::cout<<"test passed."<<std::endl;
    }
};

#endif // KCURVEANALYSE_H
