#ifndef MASKITEM_H
#define MASKITEM_H

#include <QGraphicsEllipseItem>

class MaskItem : public QGraphicsEllipseItem
{
public:
    MaskItem(QGraphicsItem *parent = 0);
    void setSize(int size);

    inline int getSize() {return m_size / 2;}
    inline int getCX() {return (cx + (m_size / 2));}
    inline int getCY() {return (cy + (m_size / 2));}

    void closing();

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    void mouseMoveEvent(QGraphicsSceneMouseEvent *event);


private:
    int m_size;
    int px, py;
    int cx, cy;
    QPointF startPt;

    void updatePos();
};

#endif // MASKITEM_H
