#ifndef HELPWIDGET_H
#define HELPWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QTextCodec>

namespace Ui {
class helpWidget;
}

class helpWidget : public QWidget
{
    Q_OBJECT

public:
    explicit helpWidget(QWidget *parent = 0);
    ~helpWidget();

private:
    Ui::helpWidget *ui;
    QLabel *lbLineV1, *lbLineV2, *lbLineH1, *lbLineH2, *lbSave, *lbQuit, *lbESC, *lbLoadPic;
    int base, diff, label_width, label_height;
    QTextCodec *helpCodec;

};

#endif // HELPWIDGET_H
