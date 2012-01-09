#ifndef MENTSUWIDGET_H
#define MENTSUWIDGET_H

#include <QWidget>
#include "common/NakiMentsu.h"

class OMMentsuWidget : public QWidget
{
    Q_OBJECT
public:
    explicit OMMentsuWidget(QWidget *parent = 0);
    void setMentsu(OMNakiMentsu &mentsu,int rotate,int distance);
    OMRule m_rule;
    
signals:
    
public slots:

protected:
    virtual void paintEvent(QPaintEvent *);

private:
    OMNakiMentsu m_mentsu;
    QImage m_image[4];
    int m_rotate;
};

#endif // MENTSUWIDGET_H
