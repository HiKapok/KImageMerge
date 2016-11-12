#ifndef KIMAGECOLLECTOR
#define KIMAGECOLLECTOR

#include <fstream>
#include <vector>
#include <string>
#include <functional>

class QString;

class KImageCollector
{
public:
    KImageCollector(/*QWidget *pWidget = nullptr*/);
    ~KImageCollector();
    bool prepare_lists(QString);
    std::size_t total_num() const{
        return m_vecFileLists.size();
    }
    void each_file(uint16_t width, uint16_t height, std::function<bool(uint16_t*)> mergeCore);
private:
    //QWidget* m_pMainWidget;
    std::vector<std::string> m_vecFileLists;

};


#endif // KIMAGECOLLECTOR

