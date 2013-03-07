#ifndef VIEWDIALOG_H
#define VIEWDIALOG_H

#include <QDialog>

class SceneView;
class QPushButton;
class QSlider;

class ViewDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ViewDialog(QWidget *parent = 0);
    ~ViewDialog();

public slots:
    void closing();

private:
    SceneView *sv;
    QSlider *sizeSlider;
    QPushButton *setButton;
};

#endif // VIEWDIALOG_H
