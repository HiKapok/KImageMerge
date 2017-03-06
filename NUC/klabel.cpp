#include "klabel.h"

#include <QPainter>

std::vector<QRgb> KLabel::m_veColorMap;
uint8_t KLabel::m_iColorLevel = 25;

KLabel::KLabel(QWidget * pWidget)
    : QLabel(pWidget)
    , m_uImage(nullptr)
    , m_iWidth(0)
    , m_iHeight(0)
    , m_beColored(false)
    , m_bStartBlue(false)
    , m_sText("KLabel")
{
    if(m_beColored) creat_color_tabel(m_iColorLevel, m_bStartBlue);
}

KLabel::~KLabel(){
    if(nullptr != m_uImage){
        delete [] m_uImage;
        m_uImage = nullptr;
    }
}

void KLabel::paintEvent(QPaintEvent *)
{
    this->setWordWrap(true);
    QPainter *paint = new QPainter();
    paint->begin(this);
    if(m_qImage) {
        this->setPixmap(QPixmap::fromImage(*m_qImage));
        //this->resize(this->pixmap()->size());
        paint->drawPixmap(QPoint(0, 0),*(this->pixmap()));
    }
    else paint->drawText(0,0,50,50,Qt::AlignCenter, this->text());
    paint->end();
}
