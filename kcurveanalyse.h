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
#include <fstream>

class KCurveAnalyse
{
public:
    typedef enum{
        LIST_VERTICAL = 0,
        LIST_HORIZONTAL = 1
    } outputformat;

    KCurveAnalyse(uint16_t _width, uint16_t _height, double _viewWStep=1., double _viewHStep=1.,
                  double _oaWidthLow=-10., double _oaWidthHigh=10., double _oaHeightLow=2.5, double _oaHeightHigh=-19.5, double _viewWidth=6.25, double _viewHiehgt=5.);
    /* invoke this yourself */
    bool get_filelist();
    /* invoke this to get pixel value list related to the position */
    bool get_pixel_list(std::tuple<uint16_t, uint16_t>, std::vector<std::vector<uint16_t>>&);
    // get pixel value for draw curve
    bool get_pixel_point(std::tuple<uint16_t, uint16_t>, std::tuple<uint16_t, uint16_t>, outputformat = LIST_HORIZONTAL);

    bool draw_curve(std::string);
private:
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
    std::string m_sCurDirectory;
    std::vector<std::string> m_vecFileLists;
    std::vector<uint16_t> m_vecXImageIndex;
    std::vector<uint16_t> m_vecYImageIndex;
    std::vector<std::string> m_vecOutFileLists;

    /* translate the pixel position in the whole big image to a vector of image filenames in which contains this pixel */
    bool pixel2file(uint16_t, uint16_t, std::vector<std::string>&);
    /* translate the pixel position in the whole big image to a vector of image indexes in which contains this pixel */
    uint16_t pixel2imagenum(uint16_t, uint16_t, uint16_t, uint16_t, std::vector<uint16_t>&);

    /* don't call this */
    bool get_pixel_gray(std::string, std::tuple<uint16_t, uint16_t>, uint16_t&, bool=true);

    class KPointDetail{
    public:
        KPointDetail(const std::tuple<uint16_t, uint16_t> &position
                     ,const std::tuple<uint16_t, uint16_t> &numimages
                     ,const std::tuple<uint16_t, uint16_t> &horindexbound
                     ,const std::tuple<uint16_t, uint16_t> &verindexbound
                     ,outputformat type)
            :m_pOriginalPoint(position),
            m_pHorIndexBound(horindexbound),
            m_pVerIndexBound(verindexbound),
            m_oDirection(type),
            m_iGroupCount( (LIST_VERTICAL == m_oDirection)?(std::get<0>(numimages)):(std::get<1>(numimages)) ),
            m_iLineCount( (LIST_VERTICAL == m_oDirection)?(std::get<1>(numimages)):(std::get<0>(numimages)) ){
            //m_vecPoints.clear();

            m_vecPoints.resize(m_iGroupCount);
            for(uint16_t index = 0;index < m_iGroupCount;++index){
                m_vecPoints[index].resize(m_iLineCount);
            }
        }
        KPointDetail(const KPointDetail& rhs){
            this->m_pOriginalPoint = rhs.m_pOriginalPoint;
            this->m_pHorIndexBound = rhs.m_pHorIndexBound;
            this->m_pVerIndexBound = rhs.m_pVerIndexBound;
            this->m_oDirection = rhs.m_oDirection;
            this->m_iGroupCount = rhs.m_iGroupCount;
            this->m_iLineCount = rhs.m_iLineCount;
            this->m_vecPoints.clear();

            for(uint16_t index = 0;index < m_iGroupCount;++index){
                this->m_vecPoints.push_back( [&](uint16_t groupindex) -> std::vector<uint16_t> {
                        if(groupindex < rhs.m_vecPoints.size()) return rhs.m_vecPoints[index];
                        return {};
                }(index) );
            }
        }

        // the elements in coord are orgnized "which row", "which column"
        void put_point_by_coord(uint16_t value, const std::tuple<uint16_t, uint16_t>& coord){
            if(LIST_HORIZONTAL == m_oDirection){
                if(std::get<0>(coord) < m_iGroupCount && std::get<1>(coord) < m_iLineCount){
                    m_vecPoints[std::get<0>(coord)][std::get<1>(coord)] = value;
                }
            }else{
                if(std::get<0>(coord) < m_iLineCount && std::get<1>(coord) < m_iGroupCount){
                    m_vecPoints[std::get<1>(coord)][std::get<0>(coord)] = value;
                }
            }
        }

        bool get_point_by_realcoord(uint16_t group_index, uint16_t point_index, uint16_t &value){
            if(LIST_HORIZONTAL == m_oDirection){
                if(group_index >= m_iGroupCount){ value = 0; return false; }
                if(point_index >= std::get<0>(m_pHorIndexBound)
                        && point_index <= std::get<1>(m_pHorIndexBound)){
                    value = m_vecPoints[group_index][point_index-std::get<0>(m_pHorIndexBound)];
                }else{ value = 0; }
            }else{
                if(group_index >= m_iGroupCount){ value = 0; return false; }
                if(point_index >= std::get<0>(m_pVerIndexBound)
                        && point_index <= std::get<1>(m_pVerIndexBound)){
                    value = m_vecPoints[group_index][point_index-std::get<0>(m_pVerIndexBound)];
                }else{ value = 0; }
            }
            return true;
        }

//        std::vector<uint16_t> get_point_column(uint16_t index) const{
//            if(index >= std::get<0>(m_pNumImage)) return {};
//            std::vector<uint16_t> temp;
//            for(auto & itemVec : m_vecPoints){
//                temp.push_back(itemVec[index]);
//            }
//            return temp;
//        }

        uint16_t get_num_line() const { return m_iLineCount; }
        uint16_t get_num_group() const { return m_iGroupCount; }
        std::tuple<uint16_t, uint16_t> get_org_coord(){
            return this->m_pOriginalPoint;
        }

        std::tuple<uint16_t, uint16_t> get_hor_bound(){
            return this->m_pHorIndexBound;
        }
        std::tuple<uint16_t, uint16_t> get_ver_bound(){
            return this->m_pVerIndexBound;
        }

    private:
        KPointDetail(){ }
        const KPointDetail & operator = (const KPointDetail&){ return *this; }

        std::tuple<uint16_t, uint16_t> m_pOriginalPoint;
        std::tuple<uint16_t, uint16_t> m_pHorIndexBound;
        std::tuple<uint16_t, uint16_t> m_pVerIndexBound;
        outputformat m_oDirection;
        uint16_t m_iGroupCount;
        uint16_t m_iLineCount;
        std::vector<std::vector<uint16_t>> m_vecPoints;
    };

    void unittest_pixel2image(){
        uint16_t count = m_iTotalWCount;
        uint16_t step = m_iWStep;
        for(uint16_t pixel = 0;pixel<m_iTotalWidth;++pixel){
            std::vector<uint16_t> lhs;
            std::vector<uint16_t> rhs;
            for(uint16_t index = 0;index<count;++index){
                if(pixel>=index*step && pixel<m_iImgWidth+index*step){
                    lhs.push_back(index+1);
                }
            }
            int16_t min_bound = std::min(std::floor((pixel*1.)/step+1),std::ceil(((pixel*1.)-m_iImgWidth+1)/step)+1);
            int16_t max_bound = std::max(std::floor((pixel*1.)/step+1),std::ceil(((pixel*1.)-m_iImgWidth+1)/step)+1);

            min_bound = std::max(min_bound, static_cast<int16_t>(1));
            min_bound = std::min(min_bound, static_cast<int16_t>(count));
            max_bound = std::min(max_bound, static_cast<int16_t>(count));
            for(int16_t index = min_bound;index <= max_bound;++index){
                rhs.push_back(static_cast<uint16_t>(index));
            }
            if(lhs.size() != rhs.size()){
                std::cout<<"test failed (@comp size)."<<std::endl;
                std::cout<<"pixel:"<<pixel<<" m_iStep:"<<step<<std::endl;
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
                        std::cout<<"pixel:"<<pixel<<" m_iStep:"<<step<<std::endl;
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
