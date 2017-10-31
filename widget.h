#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <plabel.h>
#include <QSlider>
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
    int itReadFile(QString filepath, int mode);
    void initWidget();
    void readConfig();
    void saveConfig();
    void setWH(int w, int h);
    bool isInit;
    pLabel *plb;
    qint16 nChannelID;
public slots:
    void dragH1();
    void dragH2();
    void dragV1();
    void dragV2();
    void updateSlider();
private:
    Ui::Widget *ui;
    int imgMode, displayMode;
    float width, height, funcRange, funcWidth, funcStart;
    QString ip, user, pwd;
    QTextCodec *codec;
    QSlider *slider;
    QLabel *lbSliderInfo;
    int ratio, imgWidth, imgHeight;
};

#endif // WIDGET_H
