#ifndef SCENEVIEW_H
#define SCENEVIEW_H

#include <QGraphicsView>
#include <QGraphicsPixmapItem>
#include <QTimer>
#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>

using namespace cv;

class MaskItem;
class QGraphicsScene;

class SceneView : public QGraphicsView
{
    Q_OBJECT
public:
    explicit SceneView(QWidget *parent = 0);
    ~SceneView();
    QImage MatToQImage(const Mat& mat);
    void closing();
    void startVidMask();
    inline void forceVidMaskCriteria() {added = true;}
    void reset();

public slots:
    void updateImage();
    void clearMask();
    void setMaskSize(int a);
    void setMask();

protected:
    //Holds the current centerpoint for the view, used for panning and zooming
    QPointF CurrentCenterPoint;

    //From panning the view
    QPoint LastPanPoint;

    //Set the current centerpoint in the
    void SetCenter(const QPointF& centerPoint);
    QPointF GetCenter() { return CurrentCenterPoint; }

    //Take over the interaction
    virtual void mousePressEvent(QMouseEvent* event);
    virtual void mouseReleaseEvent(QMouseEvent* event);
    virtual void mouseMoveEvent(QMouseEvent* event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent* event);
    virtual void resizeEvent(QResizeEvent* event);

private:
    QTimer timer;
    VideoCapture cap;
    QGraphicsPixmapItem *pix;
    MaskItem *mi;
    QGraphicsScene* Scene;
    bool added, crop;

    int x,y,s;
//    QGraphicsEllipseItem *cir;
};

#endif // SCENEVIEW_H
