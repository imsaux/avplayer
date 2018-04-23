#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QLabel>
#include <QSlider>
#include <QDebug>
#include <QPainter>
#include <QPaintEvent>
#include <QPushButton>
#include <QFileDialog>
#include <QTextCodec>
#include <QCheckBox>
#include <QMessageBox>
#include <QDesktopWidget>
#include <QGuiApplication>

#include <plabel.h>
#include <controller.h>
#include <global.h>
#include <distortionwidget.h>

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
    void startMonitor();
    void keyPressEvent(QKeyEvent* e);
    QString itOpenFile();
    int itReadFile(QString filepath, int mode);
    void initWidget();
    void saveConfig();
    void readConfig();

public slots:
    void dragH1();
    void dragH2();
    void dragV1();
    void dragV2();
//    void updateSlider();
    void showDistorationDialog();
    void cbChange(int);
private:
    Ui::Widget *ui;
    int imgMode, displayMode;
    float winwidth, winheight, funcRange, funcWidth, funcStart;
    QImage img;
    QTextCodec *codec;
    pLabel *plb;
    Controller *controller;
    distortionWidget *dw;

private slots:
    void updateFrame1(char* p);
//    void updateFrame2(char* p);

};
#endif // WIDGET_H
