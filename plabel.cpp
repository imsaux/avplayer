
#include <QDebug>
#include <QLabel>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QPen>
#include <QDesktopWidget>
#include <QGuiApplication>
#include <QScreen>
#include <QTime>
#include "plabel.h"

pLabel::pLabel(QWidget *parent=0, int w=0, int h=0): QLabel(parent)
{
    this->setMouseTracking(true);
    this->setGeometry(0,0,w,h);
    width=w;
    height=h;
    displayMode = DISPLAY_RTSP;
    lineMode = LINE_NONE;
    isPressed = false;
    opMode = OP_MODE_CREATE;
    dragMode = LINE_NONE;
}


void pLabel::setShowImage(QImage _img)
{
    img = QImage(_img);
    update();
}

void pLabel::paintEvent(QPaintEvent *e)
{
    if(displayMode==DISPLAY_RTSP)
    {
//        qDebug() << "paintEvent";
//        qDebug() << width;
//        qDebug() << height;

        this->setPixmap(QPixmap::fromImage(img.scaled(width,height)));
    }
    QLabel::paintEvent(e);
    QPainter p(this);
    QPen pen(Qt::yellow, 2, Qt::DotLine, Qt::RoundCap, Qt::RoundJoin);
    p.setPen(pen);
    p.drawLine(QPoint(0, current.y()), QPoint(this->geometry().width(), current.y()));
    p.drawLine(QPoint(current.x(), 0), QPoint(current.x(), this->geometry().height()));
    if (lastClick.isNull() == false)
    {
        p.drawPoint(lastClick);
    }
    drawH1(&p);
    drawH2(&p);
    drawV1(&p);
    drawV2(&p);

    if (opMode==OP_MODE_DRAG)
    {
        p.setPen(Qt::cyan);
        if (dragMode==LINE__H1 && p_h1.isEmpty()==false)
        {
            p_h1.translate(endPoint.x()-lastPoint.x(),endPoint.y()-lastPoint.y());//移动(可理解为向量移动)
            p.drawPath(p_h1);
            p.drawPath(p_h2);
            p.drawPath(p_v1);
            p.drawPath(p_v2);
        }
        if (dragMode==LINE__H2 && p_h1.isEmpty()==false)
        {
            p_h2.translate(endPoint.x()-lastPoint.x(),endPoint.y()-lastPoint.y());//移动(可理解为向量移动)
            p.drawPath(p_h1);
            p.drawPath(p_h2);
            p.drawPath(p_v1);
            p.drawPath(p_v2);
        }
        if (dragMode==LINE__V1 && p_h1.isEmpty()==false)
         {
            p_v1.translate(endPoint.x()-lastPoint.x(),endPoint.y()-lastPoint.y());//移动(可理解为向量移动)
            p.drawPath(p_h1);
            p.drawPath(p_h2);
            p.drawPath(p_v1);
            p.drawPath(p_v2);
        }
        if (dragMode==LINE__V2 && p_h1.isEmpty()==false)
        {
            p_v2.translate(endPoint.x()-lastPoint.x(),endPoint.y()-lastPoint.y());//移动(可理解为向量移动)
            p.drawPath(p_h1);
            p.drawPath(p_h2);
            p.drawPath(p_v1);
            p.drawPath(p_v2);
        }

        lastPoint = endPoint;
    }
}

void pLabel::drawH1(QPainter* p)
{
    QPen pen(Qt::red, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p->setPen(pen);

    if(h1.count()==2)
    {
        p->drawLine(h1[0], h1[1]);
    }
    if(_h1.count()==2)
    {
        p->drawLine(_h1[0], _h1[1]);
    }
}
void pLabel::drawH2(QPainter* p)
{
    QPen pen(Qt::green, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p->setPen(pen);

    if(h2.count()==2)
    {
        p->drawLine(h2[0], h2[1]);
    }
    if(_h2.count()==2)
    {
        p->drawLine(_h2[0], _h2[1]);
    }
}
void pLabel::drawV1(QPainter* p)
{
    QPen pen(Qt::blue, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p->setPen(pen);

    if(v1.count()==2)
    {
        p->drawLine(v1[0], v1[1]);
    }
    if(_v1.count()==2)
    {
        p->drawLine(_v1[0], _v1[1]);
    }
}
void pLabel::drawV2(QPainter* p)
{
    QPen pen(Qt::magenta, 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
    p->setPen(pen);

    if(v2.count()==2)
    {
        p->drawLine(v2[0], v2[1]);
    }
    if(_v2.count()==2)
    {
        p->drawLine(_v2[0], _v2[1]);
    }
}

void pLabel::dragLine(int mode)
{
    dragMode = mode;
    opMode = OP_MODE_DRAG;
    lineMode = LINE_NONE;
    switch(mode)
    {
        case LINE_H1:
        if (h1.count()!=2) break;
        if (ph1.isEmpty()==true)
        {
            ph1.moveTo(h1[1]);
            ph1.lineTo(h1[0]);
        }
        break;
        case LINE_H2:
        if (h2.count()!=2) break;
        if (ph2.isEmpty()==true)
        {
            ph2.moveTo(h2[1]);
            ph2.lineTo(h2[0]);
        }
        break;
        case LINE_V1:
        if (v1.count()!=2) break;
        if (pv1.isEmpty()==true)
        {
            pv1.moveTo(v1[1]);
            pv1.lineTo(v1[0]);
        }
        break;
        case LINE_V2:
        if (v2.count()!=2) break;
        if (pv2.isEmpty()==true)
        {
            pv2.moveTo(v2[1]);
            pv2.lineTo(v2[0]);
        }
        break;
        case LINE__H1:
        if (_h1.count()!=2) break;
        if (p_h1.isEmpty()==true)
        {
            p_h1.moveTo(_h1[1]);
            p_h1.lineTo(_h1[0]);
        }
        break;
        case LINE__H2:
        if (_h2.count()!=2) break;
        if (p_h2.isEmpty()==true)
        {
            p_h2.moveTo(_h2[1]);
            p_h2.lineTo(_h2[0]);
        }
        break;
        case LINE__V1:
        if (_v1.count()!=2) break;
        if (p_v1.isEmpty()==true)
        {
            p_v1.moveTo(_v1[1]);
            p_v1.lineTo(_v1[0]);
        }
        break;
        case LINE__V2:
        if (_v2.count()!=2) break;
        if (p_v2.isEmpty()==true)
        {
            p_v2.moveTo(_v2[1]);
            p_v2.lineTo(_v2[0]);
        }
        break;
    }
}

void pLabel::screenShot()
{
    img.save(QTime::currentTime().toString("hhmmsszzz")+".jpg");
}

void pLabel::mouseMoveEvent(QMouseEvent *e)
{
    tmp = current;
    current = e->pos();
    x_offset = current.x() - tmp.x();
    y_offset = current.y() - tmp.y();
    if (isPressed)
    {
        //按住鼠标左键
        endPoint = e->pos();
    }
    update();
}

void pLabel::mouseReleaseEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        endPoint = e->pos();
        update();
    }
    isPressed = false;
}

void pLabel::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton)
    {
        isPressed = true;
        lastClick = e->pos();
        lastPoint = e->pos();
        if (opMode==OP_MODE_CREATE)
        {
            updateQPointGroup(e->pos());
        }
    }
}

void pLabel::updateQPointGroup(QPoint p)
{
    if (displayMode == DISPLAY_RTSP)
    {
        if (lineMode == LINE_H1)
        {
            if (h1.count()==2)
            {
                h1.clear();
            }
            h1.append(p);
        }
        if (lineMode == LINE_H2)
        {
            if (h2.count()==2)
            {
                h2.clear();
            }
            h2.append(p);
        }
        if (lineMode == LINE_V1)
        {
            if (v1.count()==2)
            {
                v1.clear();
            }
            v1.append(p);
        }
        if (lineMode == LINE_V2)
        {
            if (v2.count()==2)
            {
                v2.clear();
            }
            v2.append(p);
        }
    }
    else if (displayMode == DISPLAY_IMAGE)
    {
        if (lineMode == LINE_H1)
        {
            if (_h1.count()==2)
            {
                _h1.clear();
            }
            _h1.append(p);
        }
        if (lineMode == LINE_H2)
        {
            if (_h2.count()==2)
            {
                _h2.clear();
            }
            _h2.append(p);
        }
        if (lineMode == LINE_V1)
        {
            if (_v1.count()==2)
            {
                _v1.clear();
            }
            _v1.append(p);
        }
        if (lineMode == LINE_V2)
        {
            if (_v2.count()==2)
            {
                _v2.clear();
            }
            _v2.append(p);
        }
    }
}

void pLabel::setDisplayMode(int mode)
{
    displayMode = mode;
}

void pLabel::setLineMode(int mode)
{
    lineMode = mode;
}

void pLabel::setOpMode(int mode)
{
    opMode = mode;
}
