#ifndef KRESTORATION_H
#define KRESTORATION_H

#include "kparamcontainer.h"
#include "krawdblintimg.h"

#include <deque>
#include <future>

class KRestoration
{
public:
    KRestoration();
private:
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
    std::string m_sCurDirectory;
    std::string m_sDesDirectory;
    std::vector<std::string> m_vecFileLists;
    std::deque<std::packaged_task<bool()>> m_vecTask;
    std::mutex m_mutex;
    std::once_flag m_oneFlag;
    bool loadFileFromDir();
    bool fixAngleDistortion();
    bool runAngleCorrection(std::string, double, double);
    void runTask();
    void fixPixelAngleDistor(double,double,uint16_t&);
    template <typename _callable>
    bool transformCopy(std::string, std::string, _callable);
};

#endif // KRESTORATION_H
