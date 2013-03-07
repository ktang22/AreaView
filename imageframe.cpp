#include <QPainter>
#include "imageframe.h"

ImageFrame::ImageFrame(QWidget *parent) :
    QLabel(parent)
{
    m_pos.setX(0);
    m_pos.setY(0);
    m_size = 50;
    drawCircle = false;
}

void ImageFrame::paintEvent(QPaintEvent *ev)
{
    QLabel::paintEvent(ev);
    QPainter painter(this);

    if (drawCircle)
    {
        //draw the circle
        painter.setPen(Qt::yellow);
        painter.drawEllipse(m_pos, m_size, m_size);
    }
}

void ImageFrame::setRadius(int rad)
{
    m_size = rad;
}

void ImageFrame::setCenterX(int x)
{
    m_pos.setX(x);
}

void ImageFrame::setCenterY(int y)
{
    m_pos.setY(y);
}
