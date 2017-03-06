#ifndef __KRAWDBLINTIMG_H__
#define __KRAWDBLINTIMG_H__

#include <fstream>

class KRawDblIntImg
{
public:
    KRawDblIntImg(std::string _filename, uint16_t _width, uint16_t _height, std::string _savefile="D://save_image.DAT")
		: m_sFileName(_filename)
		, m_sSaveFileName(_savefile)
		, m_iWidth(_width)
		, m_iHeight(_height)
		, m_iTotalElems(0)
		, m_iLastPos(0)
		, m_iImageCount(0)
	{
		m_iTotalElems = get_total_elements();
		m_iImageCount = m_iTotalElems / (_width * (_height + 1));
        std::ofstream fs(m_sSaveFileName.c_str(), std::ios_base::out | std::ios_base::trunc);
		fs.close();
	}

    bool isvalid(){
        std::ifstream fs(m_sFileName.c_str(), std::ifstream::binary);
        if (!fs) return false;
        fs.close();
        return true;
    }

	void image_buff_reset(){
		m_iLastPos = 0;
	}

	bool save_image(uint16_t *_buff, bool _bAppend = false)
	{
		std::ios_base::openmode mode = std::ios_base::binary | std::ios_base::out;
		if (_bAppend) mode |= std::ios_base::ate | std::ios_base::app;
		else mode |= std::ios_base::trunc;

        std::ofstream fs(m_sSaveFileName.c_str(), mode);
		for (int32_t index(0); index < m_iWidth * m_iHeight; ++index){
			m_iTempElem.split_into_raw(_buff[index]);
			fs.write(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
		}	
		for (int32_t index(0); index < m_iWidth; ++index){
			m_iTempElem.split_into_raw(0);
			fs.write(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
		}
		fs.close();
		return true;
	}
	bool get_next_image(uint16_t * _buff)
	{
        std::ifstream fs(m_sFileName.c_str(), std::ifstream::binary);
		if (!fs) throw std::runtime_error("can't open the input file!");
        if(m_vecLastPos.size() == m_iImageCount) return false;
        fs.seekg(m_iLastPos, fs.beg);
        m_vecLastPos.push_back(m_iLastPos);
		for (int32_t index(0); index < m_iWidth * m_iHeight; ++index){
			fs.read(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
			_buff[index] = m_iTempElem.get_real_value();
		}
		// dummy read one blank line
		for (int32_t index(0); index < m_iWidth; ++index){
			fs.read(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
		}
		m_iLastPos = static_cast<uint32_t>(fs.tellg());

		fs.close();
		//m_iLastPos += m_iWidth * (m_iHeight + 1) * 2;
		return true;
	}
    bool get_last_image(uint16_t * _buff)
    {
        std::ifstream fs(m_sFileName.c_str(), std::ifstream::binary);
        if (!fs) throw std::runtime_error("can't open the input file!");
        if(m_vecLastPos.size() < 2) return false;
        m_iLastPos = m_vecLastPos[m_vecLastPos.size() - 2];
        m_vecLastPos.resize(m_vecLastPos.size() - 1);
        fs.seekg(m_iLastPos, fs.beg);
        for (int32_t index(0); index < m_iWidth * m_iHeight; ++index){
            fs.read(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
            _buff[index] = m_iTempElem.get_real_value();
        }
        // dummy read one blank line
        for (int32_t index(0); index < m_iWidth; ++index){
            fs.read(reinterpret_cast<char *>(&m_iTempElem), sizeof(KStructMem));
        }
        m_iLastPos = static_cast<uint32_t>(fs.tellg());
        fs.close();
        //m_iLastPos += m_iWidth * (m_iHeight + 1) * 2;
        return true;
    }
	uint16_t get_image_count() const
	{
		return m_iImageCount;
	}
private:
    std::string m_sFileName;
    std::string m_sSaveFileName;
	uint16_t m_iWidth;
	uint16_t m_iHeight;
	uint32_t m_iTotalElems;
	uint32_t m_iLastPos;
	uint16_t m_iImageCount;
    std::vector<uint32_t> m_vecLastPos;
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

	int get_total_elements() const
	{
        std::ifstream fs(m_sFileName.c_str(), std::ifstream::binary);
		if (!fs) return 0;
		fs.seekg(0, fs.end);
		int _length = static_cast<int>(fs.tellg()) / 2;
		fs.close();
		return _length;
	}
};


#endif /* __KRAWDBLINTIMG_H__ */
