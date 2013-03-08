#include <QGraphicsScene>
#include <QGraphicsTextItem>
#include <QTextStream>
#include <QScrollBar>
#include <QMouseEvent>
#include <QWheelEvent>
#include <QDebug>

#include "maskitem.h"
#include "sceneview.h"

using namespace cv;

SceneView::SceneView(QWidget *parent) :
    QGraphicsView(parent)
{
    cap.open(0);
    if(!cap.isOpened())  // check if we succeeded
    {
        qDebug() << "Fail to open camera";
    }

    Mat frame;

    cap >> frame;
    QImage img = MatToQImage(frame);

    timer.setInterval(16);

    setRenderHints(QPainter::Antialiasing | QPainter::SmoothPixmapTransform);

    //Set-up the scene
    Scene = new QGraphicsScene(this);
    setScene(Scene);

    //Populate the scene
    pix = new QGraphicsPixmapItem();
    Scene->addItem(pix);
    pix->setPixmap(QPixmap::fromImage(img));

    mi = new MaskItem();
    added = false;
    crop = false;

    //Set-up the view
    setSceneRect(0, 0, 640, 480);
    SetCenter(QPointF(320.0, 240.0)); //A modified version of centerOn(), handles special cases
    setCursor(Qt::OpenHandCursor);

    connect(&timer, SIGNAL(timeout()), SLOT(updateImage()));
    timer.start();
}

SceneView::~SceneView()
{
    cap.release();

    delete pix;
    delete mi;
//    delete cir;
}

/**
  * Sets the current centerpoint.  Also updates the scene's center point.
  * Unlike centerOn, which has no way of getting the floating point center
  * back, SetCenter() stores the center point.  It also handles the special
  * sidebar case.  This function will claim the centerPoint to sceneRec ie.
  * the centerPoint must be within the sceneRec.
  */
//Set the current centerpoint in the
void SceneView::SetCenter(const QPointF& centerPoint) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();

    //Get the scene area
    QRectF sceneBounds = sceneRect();

    double boundX = visibleArea.width() / 2.0;
    double boundY = visibleArea.height() / 2.0;
    double boundWidth = sceneBounds.width() - 2.0 * boundX;
    double boundHeight = sceneBounds.height() - 2.0 * boundY;

    //The max boundary that the centerPoint can be to
    QRectF bounds(boundX, boundY, boundWidth, boundHeight);

    if(bounds.contains(centerPoint)) {
        //We are within the bounds
        CurrentCenterPoint = centerPoint;
    } else {
        //We need to clamp or use the center of the screen
        if(visibleArea.contains(sceneBounds)) {
            //Use the center of scene ie. we can see the whole scene
            CurrentCenterPoint = sceneBounds.center();
        } else {

            CurrentCenterPoint = centerPoint;

            //We need to clamp the center. The centerPoint is too large
            if(centerPoint.x() > bounds.x() + bounds.width()) {
                CurrentCenterPoint.setX(bounds.x() + bounds.width());
            } else if(centerPoint.x() < bounds.x()) {
                CurrentCenterPoint.setX(bounds.x());
            }

            if(centerPoint.y() > bounds.y() + bounds.height()) {
                CurrentCenterPoint.setY(bounds.y() + bounds.height());
            } else if(centerPoint.y() < bounds.y()) {
                CurrentCenterPoint.setY(bounds.y());
            }

        }
    }

    //Update the scrollbars
    centerOn(CurrentCenterPoint);
}

/**
  * Handles when the mouse button is pressed
  */
void SceneView::mousePressEvent(QMouseEvent* event) {
    //For panning the view
    LastPanPoint = event->pos();
    setCursor(Qt::ClosedHandCursor);

    QGraphicsView::mousePressEvent(event);
}

/**
  * Handles when the mouse button is released
  */
void SceneView::mouseReleaseEvent(QMouseEvent* event) {
    setCursor(Qt::OpenHandCursor);
    LastPanPoint = QPoint();

    if (event->button() == Qt::RightButton)
    {
        if (added)
        {
            clearMask();
            added = !added;
        }
        if (crop)
        {
            crop = !crop;
        }
    }

    QGraphicsView::mouseReleaseEvent(event);
}

void SceneView::mouseDoubleClickEvent(QMouseEvent *event)
{
    startVidMask();

    QGraphicsView::mouseDoubleClickEvent(event);
}

/**
*Handles the mouse move event
*/
void SceneView::mouseMoveEvent(QMouseEvent* event) {
    if(!LastPanPoint.isNull()) {
        //Get how much we panned
        QPointF delta = mapToScene(LastPanPoint) - mapToScene(event->pos());
        LastPanPoint = event->pos();

        //Update the center ie. do the pan
        SetCenter(GetCenter() + delta);
    }

    QGraphicsView::mouseMoveEvent(event);
}

/**
  * Zoom the view in and out.
  */
void SceneView::wheelEvent(QWheelEvent* event) {

    //Get the position of the mouse before scaling, in scene coords
    QPointF pointBeforeScale(mapToScene(event->pos()));

    //Get the original screen centerpoint
    QPointF screenCenter = GetCenter(); //CurrentCenterPoint; //(visRect.center());

    //Scale the view ie. do the zoom
    double scaleFactor = 1.15; //How fast we zoom
    if(event->delta() > 0) {
        //Zoom in
        scale(scaleFactor, scaleFactor);
    } else {
        //Zooming out
        scale(1.0 / scaleFactor, 1.0 / scaleFactor);
    }

    //Get the position after scaling, in scene coords
    QPointF pointAfterScale(mapToScene(event->pos()));

    //Get the offset of how the screen moved
    QPointF offset = pointBeforeScale - pointAfterScale;

    //Adjust to the new center for correct zooming
    QPointF newCenter = screenCenter + offset;
    SetCenter(newCenter);
}

/**
  * Need to update the center so there is no jolt in the
  * interaction after resizing the widget.
  */
void SceneView::resizeEvent(QResizeEvent* event) {
    //Get the rectangle of the visible area in scene coords
    QRectF visibleArea = mapToScene(rect()).boundingRect();
    SetCenter(visibleArea.center());

    //Call the subclass resize so the scrollbars are updated correctly
    QGraphicsView::resizeEvent(event);
}

QImage SceneView::MatToQImage(const Mat &mat)
{
    // 8-bits unsigned, NO. OF CHANNELS=1
    if(mat.type()==CV_8UC1)
    {
        // Set the color table (used to translate colour indexes to qRgb values)
        QVector<QRgb> colorTable;
        for (int i=0; i<256; i++)
            colorTable.push_back(qRgb(i,i,i));
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_Indexed8);
        img.setColorTable(colorTable);
        return img;
    }
    // 8-bits unsigned, NO. OF CHANNELS=3
    if(mat.type()==CV_8UC3)
    {
        // Copy input Mat
        const uchar *qImageBuffer = (const uchar*)mat.data;
        // Create QImage with same dimensions as input Mat
        QImage img(qImageBuffer, mat.cols, mat.rows, mat.step, QImage::Format_RGB888);
        return img.rgbSwapped();
    }
    else
    {
        qDebug() << "ERROR: Mat could not be converted to QImage.";
        return QImage();
    }
}

void SceneView::updateImage()
{
    Mat frame;
    Mat final;
    Mat mask(Size(640, 480), CV_8UC3);
    cap >> frame;

    if (crop)
    {
        circle(mask, Point(x,y), s, Scalar(255.,255.,255.), -1);
        frame.copyTo(final, mask);
    }
    else
    {
        frame.copyTo(final);
    }

    QImage img = MatToQImage(final);
    pix->setPixmap(QPixmap::fromImage(img));
}

void SceneView::clearMask()
{
    Scene->removeItem(mi);
}

void SceneView::setMaskSize(int a)
{
    mi->setSize(a);
}

void SceneView::setMask()
{
    if (!added)
    {
        Scene->addItem(mi);
        added = !added;
    }
}

void SceneView::closing()
{
    mi->closing();
}

void SceneView::startVidMask()
{
    if (added)
    {
        clearMask();
        added = !added;

        crop = true;

        x = mi->getCX();
        y = mi->getCY();
        s = mi->getSize();
    }
}
