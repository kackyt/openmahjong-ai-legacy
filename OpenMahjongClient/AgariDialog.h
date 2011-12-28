#ifndef AGARIDIALOG_H
#define AGARIDIALOG_H

#include <QDialog>

namespace Ui {
class AgariDialog;
}

class AgariDialog : public QDialog
{
    Q_OBJECT
    
public:
    explicit AgariDialog(QWidget *parent = 0);
    ~AgariDialog();
    
private:
    Ui::AgariDialog *ui;
};

#endif // AGARIDIALOG_H
