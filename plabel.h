#ifndef PLABEL_H
#define PLABEL_H

#include <QWidget>
#include <QLabel>

#define FUNCTION_FAIL 1
#define FUNCTION_OK 0
#define DISPLAY_RTSP 2
#define DISPLAY_IMAGE 3
#define LINE_H1 4
#define LINE_H2 5
#define LINE_V1 6
#define LINE_V2 7
#define LINE__H1 14
#define LINE__H2 15
#define LINE__V1 16
#define LINE__V2 17
#define LINE_NONE 8
#define OP_MODE_DRAG 9
#define OP_MODE_CREATE 10

class pLabel: public QLabel
{
public:
    pLabel(QWidget *p, int w, int h);
    void paintEvent(QPaintEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void setShowImage(QImage _img);
    void setDisplayMode(int mode);
    void setLineMode(int mode);
    void setOpMode(int mode);
    void drawH1(QPainter* p);
    void drawH2(QPainter* p);
    void drawV1(QPainter* p);
    void drawV2(QPainter* p);
    void updateQPointGroup(QPoint p);
    void screenShot();
    void dragLine(int mode);

private:
    QList<QPoint> h1, h2, v1, v2, _h1, _h2, _v1, _v2;
    QPoint current, tmp, lastClick, endPoint, lastPoint;
    QImage img;
    int lineMode, displayMode, dragMode, opMode, x_offset, y_offset, width, height;
    bool isPressed;
    qint32 nChannelID;
    QPainterPath p_h1, p_h2, p_v1, p_v2, ph1, ph2, pv1, pv2;

};

#endif // PLABEL_H
