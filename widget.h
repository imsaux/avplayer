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
#include <QMessageBox>
#include <QDesktopWidget>
#include <QGuiApplication>

#include <plabel.h>
#include <controller.h>
#include <global.h>

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

public slots:
    void dragH1();
    void dragH2();
    void dragV1();
    void dragV2();
    void updateSlider();
private:
    Ui::Widget *ui;
    int imgMode, displayMode;
    float winwidth, winheight, funcRange, funcWidth, funcStart;
    QTextCodec *codec;
    QSlider *slider;
    QLabel *lbSliderInfo;
    pLabel *plb;
    Controller *controller;

private slots:
    void updateFrame(const QImage &frame);

};
#endif // WIDGET_H
