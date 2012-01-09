#include <QPainter>
#include "PaiButton.h"

OMPaiButton::OMPaiButton(QWidget *parent) :
    QPushButton(parent)
{
    setPai(m_pai,1);
    //setCheckable(true);
    setSizePolicy(QSizePolicy::Fixed,QSizePolicy::Fixed);
    QObject::connect(this,SIGNAL(clicked(bool)),this,SLOT(togglePai(bool)));
}



void OMPaiButton::setPai(OMPai &pai, int rotate)
{
    QString filepath;
    OMString hainame;
    m_pai = pai;
    m_rotate = rotate;

    pai.toPaiString(hainame,m_rule);

    /* 回転した画像を取得する */
    switch(rotate){
    case 0:
        filepath = QString(":/haiga/%1.gif").arg(hainame);
        setMaximumSize(24,34);
        setMinimumSize(24,34);
        resize(24,34);
        break;
    case 1:
        filepath = QString(":/haiga/y%1.gif").arg(hainame);
        if(pai != PAI_NIL){
            setMaximumSize(32,24);
            setMinimumSize(32,24);
            resize(32,24);
        }else{
            setMaximumSize(34,24);
            resize(34,24);
        }
        break;
    case 2:
        filepath = QString(":/haiga/t%1.gif").arg(hainame);
        setMaximumSize(24,34);
        setMinimumSize(24,34);
        resize(24,34);
        break;
    case 3:
    default:
        filepath = QString(":/haiga/ty%1.gif").arg(hainame);
        if(pai != PAI_NIL){
            setMaximumSize(32,24);
            setMinimumSize(32,24);
            resize(32,24);
        }else{
            setMaximumSize(34,24);
            setMinimumSize(34,24);
            resize(34,24);
        }
        break;
    }

    m_image = QImage(filepath);

}

void OMPaiButton::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    QPen pen;

    if(m_rule.m_iGlasshai != FALSE && (UINT)m_pai != PAI_NIL && m_pai.m_iId != 4) {
        painter.setOpacity(0.5);
    }

    painter.drawImage(0,0,m_image);

    pen.setWidth(5);
    pen.setBrush(Qt::red);
    painter.setOpacity(1.0);

    if(isChecked()) {
        /* チェックされていた場合は赤枠で囲む */
        painter.setPen(pen);
        painter.drawRect(0,0,width(),height());
    }

}

void OMPaiButton::togglePai(bool enable)
{
    emit selectPai(&m_pai,enable);
}
