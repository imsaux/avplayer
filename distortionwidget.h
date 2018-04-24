#ifndef DISTORTIONWIDGET_H
#define DISTORTIONWIDGET_H

#include <QWidget>
#include <QLabel>
#include <QPushButton>
#include <QSlider>
#include <QTime>
#include <QTextCodec>
#include <QLineEdit>
#include <QMouseEvent>
#include <global.h>

namespace Ui {
class distortionWidget;
}

class distortionWidget : public QWidget
{
    Q_OBJECT

public:
    explicit distortionWidget(QWidget *parent = 0);
    ~distortionWidget();

public slots:
    void initWidget();
    void updateImg();
    void showImg();
    void updateSlider(int);
    void distoration_refresh();
    void distoration_tofile();
    void updateSlider(QString);

protected:
    void mouseReleaseEvent(QMouseEvent *event);

private:
    Ui::distortionWidget *ui;
    QLabel *lpic, *lRatioInfo;
    QPushButton *pbRefresh, *pbToFile;
    QSlider *slider;
    QTextCodec *distorationCodec;
    QLineEdit *leRatio;
    QImage img;
};

#endif // DISTORTIONWIDGET_H
