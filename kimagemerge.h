#ifndef KIMAGEMERGE
#define KIMAGEMERGE

#include <fstream>
#include <vector>
#include <string>
#include <memory>
#include <utility>
#include <map>

class KImageCollector;
class QWidget;
class QString;
class KImageMerge
{
public:
    typedef enum
    {
        MergeTypeStart = 0,
        FormerPrefer = 1,
        LatterPrefer = 2,
        SymmetryPrefer = 3,
        CenterReflect = 4,
        MergeTypeEnd
    } MergeType;
    explicit KImageMerge(uint16_t _width, uint16_t _height, uint16_t _hCounts, uint16_t _vCounts, uint16_t _hOverlap = 0, uint16_t _vOverlap = 0, bool bShowLine = false);
    ~KImageMerge();
    // call this after created immediately if you want to set a widget
    //bool set_widget(QWidget *);
    bool merge_image(MergeType, std::string);
    bool prepare_files(QString);
    // TODO: reconstruction the follow two function
   const std::string getOutputFile() const{
        return m_sOutputFile;
    }
    const std::pair<uint32_t, uint32_t> getOutputSize() const{
        return std::make_pair(m_iOutputWidth, m_iOutputHeight);
    }

private:
    // TODO: remove redundancy
    uint16_t m_iImageWidth;
    uint16_t m_iImageHeight;
    uint16_t m_iHImageCounts;
    uint16_t m_iVImageCounts;
    uint16_t m_iHOverLap;
    uint16_t m_iVOverLap;
    uint32_t m_iHCurCounts;
    uint32_t m_iVCurCounts;
    bool m_ifShowLine;
    std::string m_sOutputFile;
    uint32_t m_iOutputWidth;
    uint32_t m_iOutputHeight;
    uint32_t m_iFileIndex;
    //uint32_t m_iLastLine;
    //uint32_t m_iLastColumn;
    uint16_t *m_pTempImage;
    class KStructMem
    {
        uint8_t _high;
        uint8_t _low;
    public:
        KStructMem() : _high(0), _low(0){}
        uint16_t get_real_value() const
        {
            return static_cast<uint16_t>(_high * 256 + _low);
        }
        void split_into_raw(uint16_t _data)
        {
            _high = _data / 256;
            _low = _data % 256;
        }
    } m_iTempElem;
    std::shared_ptr<KImageCollector> m_fileHolder;
    KImageMerge(KImageMerge&){}
    KImageMerge& operator = (const KImageMerge &){ return *this; }
    bool doFormerMerge(std::string, uint16_t*);
    bool doLatterMerge(std::string, uint16_t*);
    bool doSymmetryMerge(std::string, uint16_t*);
    bool doCenterReflectMerge(std::string, uint16_t*);
    bool write2file(KImageMerge::MergeType, std::string, uint32_t, uint32_t, uint32_t, uint16_t*);
    bool write2file(std::string, uint32_t, uint32_t, uint32_t, uint16_t*);
    bool write2file(std::string, std::string);
};


#endif // KIMAGEMERGE

