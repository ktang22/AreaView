#ifndef IMAGEFRAME_H
#define IMAGEFRAME_H

#include <QLabel>

class ImageFrame : public QLabel
{
    Q_OBJECT
public:
    explicit ImageFrame(QWidget *parent = 0);
    inline void setDrawCircle(bool draw) {drawCircle = draw;}
    inline QPoint getCenter() const {return m_pos;}
    inline int getSize() const {return m_size;}

private:
    bool drawCircle;
    QPoint m_pos;
    int m_size;
    
signals:
    
public slots:
    void setCenterX(int x);
    void setCenterY(int y);
    void setRadius(int rad);

protected:
    void paintEvent(QPaintEvent *ev);
};

#endif // IMAGEFRAME_H
