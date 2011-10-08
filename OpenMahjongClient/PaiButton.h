#ifndef PAIBUTTON_H
#define PAIBUTTON_H

#include <QPushButton>
#include "common/OpenMahjongDef.h"
#include "common/Pai.h"
#include "common/Rule.h"

class OMPaiButton : public QPushButton
{
    Q_OBJECT
public:
    explicit OMPaiButton(QWidget *parent = 0);
    void setPai(OMPai&,int rotate = 0);
    void setRule(OMRule&);

signals:
    void selectPai(OMPai *,bool);

private slots:
    void togglePai(bool enable);

public slots:

private:
    OMRule m_rule;
    OMPai m_pai;
    int m_rotate;
    QImage m_image;
    void toPaiString(OMPai &,QString &);
protected:
    virtual void paintEvent(QPaintEvent *);


};

#endif // PAIBUTTON_H
