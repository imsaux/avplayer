#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <plabel.h>
#define LINE_H1 4
#define LINE_H2 5
#define LINE_V1 6
#define LINE_V2 7

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();
    void start();
    void keyPressEvent(QKeyEvent* e);
    QString itOpenFile();
    int itReadFile(QString filepath, int mode=-1);
    void initWidget();
    void readConfig();
public slots:
    void dragH1();
    void dragH2();
    void dragV1();
    void dragV2();
private:
    qint16 nChannelID;
    Ui::Widget *ui;
    pLabel *plb;
    int imgMode, displayMode;
    float width, height, funcRange, funcWidth, funcStart;
    QString ip, user, pwd;
    QTextCodec *codec;
};

#endif // WIDGET_H
