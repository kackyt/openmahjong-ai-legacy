#include <QPainter>
#include "MentsuWidget.h"

OMMentsuWidget::OMMentsuWidget(QWidget *parent) :
    QWidget(parent)
{
    /* ƒfƒoƒbƒO—p */
    OMNakiMentsu mentsu;
    OMPai pai;

    mentsu.m_iAite = 3;
    mentsu.m_iCategory = MENTSU_ANKAN;
    pai.m_iCategory = PAI_MANZU;
    pai.m_iNo = 1;

    mentsu.m_nakihai = pai;
    mentsu.m_aPaiList.append(pai);
    pai.m_iNo = 2;
    mentsu.m_aPaiList.append(pai);
    pai.m_iNo = 3;
    mentsu.m_aPaiList.append(pai);
    mentsu.m_aPaiList.append(pai);

    setMentsu(mentsu,2,0);

}

void OMMentsuWidget::setMentsu(OMNakiMentsu &mentsu,int rotate,int distance)
{
    int i;
    int w=0,h=0;
    OMString filepath,hainame;
    QImage image;
    m_rotate = rotate;
    m_mentsu = mentsu;

    if(mentsu.m_iCategory == MENTSU_ANKAN){
        OMPai pai;
        pai.m_iCategory = 0;
        pai.m_iNo = 0;

        pai.toPaiString(hainame,m_rule);

        /* ‰ñ“]‚µ‚½‰æ‘œ‚ðŽæ“¾‚·‚é(— ) */
        switch(rotate){
        case 0:
            filepath = QString(":/haiga/%1.gif").arg(hainame);
            break;
        case 1:
            filepath = QString(":/haiga/y%1.gif").arg(hainame);
            break;
        case 2:
            filepath = QString(":/haiga/t%1.gif").arg(hainame);
            break;
        case 3:
        default:
            filepath = QString(":/haiga/ty%1.gif").arg(hainame);
            break;
        }

        image = QImage(filepath);

        m_image[0] = image;
        m_image[3] = image;

        pai = m_mentsu.m_aPaiList[0];

        pai.toPaiString(hainame,m_rule);

        switch(rotate){
        case 0:
        case 2:
            w += image.width() * 2;
            h = image.height();
            break;
        case 1:
        case 3:
        default:
            w = image.width();
            h += image.height() * 2;
            break;
        }

        /* ‰ñ“]‚µ‚½‰æ‘œ‚ðŽæ“¾‚·‚é(•\) */
        switch(rotate){
        case 0:
            filepath = QString(":/haiga/%1.gif").arg(hainame);
            break;
        case 1:
            filepath = QString(":/haiga/y%1.gif").arg(hainame);
            break;
        case 2:
            filepath = QString(":/haiga/t%1.gif").arg(hainame);
            break;
        case 3:
        default:
            filepath = QString(":/haiga/ty%1.gif").arg(hainame);
            break;
        }

        image = QImage(filepath);
        m_image[1] = image;
        m_image[2] = image;

        switch(rotate){
        case 0:
        case 2:
            w += image.width() * 2;
            break;
        case 1:
        case 3:
        default:
            h += image.height() * 2;
            break;
        }
    }else{
        int pos=0;
        int rotate_pos;

        switch(distance){
        case 1:
            rotate_pos = mentsu.m_aPaiList.size() - 1;
            break;
        case 2:
            rotate_pos = 1;
            break;
        case 3:
        default:
            rotate_pos = 0;
            break;
        }

        for(i=0;i<mentsu.m_aPaiList.size();i++){
            OMPai pai;

            if(i==rotate_pos){
                pai = mentsu.m_aPaiList[m_mentsu.getNakiPos()];
            }else{
                if(pos == (int)mentsu.getNakiPos()){
                    pos++;
                }
                pai = mentsu.m_aPaiList[pos++];
            }

            pai.toPaiString(hainame,m_rule);
            /* ‰ñ“]‚µ‚½‰æ‘œ‚ðŽæ“¾‚·‚é */

            switch(i == rotate_pos ? (rotate + 1) : rotate){
            case 0:
            default:
                filepath = QString(":/haiga/%1.gif").arg(hainame);
                break;
            case 1:
                filepath = QString(":/haiga/y%1.gif").arg(hainame);
                break;
            case 2:
                filepath = QString(":/haiga/t%1.gif").arg(hainame);
                break;
            case 3:
                filepath = QString(":/haiga/ty%1.gif").arg(hainame);
                break;
            }

            image = QImage(filepath);

            m_image[i] = image;


            switch(rotate){
            case 0:
            case 2:
                w += image.width();
                if(image.height() > h){
                    h = image.height();
                }
                break;
            case 1:
            case 3:
            default:
                if(image.width() > w){
                    w = image.width();
                }
                h += image.height();
                break;
            }
        }

    }

    setMaximumSize(w,h);
    setMinimumSize(w,h);
    resize(w,h);

}

void OMMentsuWidget::paintEvent(QPaintEvent *)
{
    QPainter painter(this);
    int i,x,y;

    switch(m_rotate){
    case 0:
    case 3:
        x = 0;
        y = 0;
        break;
    case 1:
        x = 0;
        y = height();
        break;
    case 2:
    default:
        x = width();
        y = 0;
        break;
    }

    for(i=0;i<m_mentsu.m_aPaiList.size();i++){
        if(m_rule.m_iGlasshai != FALSE && (UINT)m_mentsu.m_aPaiList[i] != PAI_NIL && m_mentsu.m_aPaiList[i].m_iId != 4) {
            painter.setOpacity(0.5);
        }else{
            painter.setOpacity(1.0);
        }

        switch(m_rotate){
        case 0:
            if(m_image[i].width() > m_image[i].height()){
                y = m_image[i].width() - m_image[i].height() + 1;
            }else{
                y = 0;
            }
            break;
        case 1:
            if(m_image[i].height() > m_image[i].width()){
                x = m_image[i].height() - m_image[i].width() - 1;
            }else{
                x = 0;
            }
            y -= m_image[i].height();
            break;
        case 2:
            x -= m_image[i].width();
            break;
        case 3:
        default:
            break;
        }

        painter.drawImage(x,y,m_image[i]);

        switch(m_rotate){
        case 0:
            x += m_image[i].width();
            break;
        case 3:
            y += m_image[i].height();
            break;
        default:
            break;
        }
    }

}
