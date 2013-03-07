#include <QGraphicsSceneMouseEvent>
#include <QPen>
#include <QDebug>
#include <QSettings>
#include "maskitem.h"

MaskItem::MaskItem(QGraphicsItem *parent) : QGraphicsEllipseItem(parent)
{
    setFlag(QGraphicsItem::ItemIsSelectable, true);
    setFlag(QGraphicsItem::ItemIsMovable, true);
//    setFlag(QGraphicsItem::ItemSendsGeometryChanges, true);

    QSettings settings("SIS", "AreaView");
    settings.beginGroup("circle");
    int d_size = settings.value("radius", 50).toInt();
    int x = settings.value("x", 0).toInt();
    int y = settings.value("y", 0).toInt();
    settings.endGroup();

    m_size = 2 * d_size;
    px = x - d_size;
    py = y - d_size;

    setPen(QPen(Qt::yellow));

    updatePos();
}

void MaskItem::setSize(int size)
{
    m_size = size;
    updatePos();
}

void MaskItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsItem::mousePressEvent(event);
}

void MaskItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
    updatePos();
    QGraphicsEllipseItem::mouseMoveEvent(event);
}

void MaskItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QGraphicsEllipseItem::mouseReleaseEvent(event);
}

void MaskItem::updatePos()
{
    setRect(px, py, m_size, m_size);
    QRectF sth = mapRectToParent(boundingRect());
    cx = sth.x();
    cy = sth.y();
}

void MaskItem::closing()
{
    qDebug() << "closing...";
    QSettings settings("SIS", "AreaView");
    settings.beginGroup("circle");
    settings.setValue("radius", (m_size / 2));
    settings.setValue("x", getCX());
    settings.setValue("y", getCY());
    settings.endGroup();
}
