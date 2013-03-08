#include <QSlider>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
#include <QKeyEvent>
#include <QKeySequence>
#include <QDebug>
#include "viewdialog.h"
#include "sceneview.h"

using namespace cv;

ViewDialog::ViewDialog(QWidget *parent) :
    QDialog(parent)
{
    sv = new SceneView();

    QSettings settings("SIS", "AreaView");
    settings.beginGroup("circle");
    int d_size = settings.value("radius", 50).toInt();
    settings.endGroup();

    sizeSlider = new QSlider();
    sizeSlider->setMinimum(10);
    sizeSlider->setMaximum(479);
    sizeSlider->setValue(2 * d_size);
    sizeSlider->setOrientation(Qt::Horizontal);

//    setButton = new QPushButton("&Set");

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(sizeSlider);
    bottomLayout->addStretch();
//    bottomLayout->addWidget(setButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(sv);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
    setMinimumSize(800, 600);
//    setFixedSize(800, 600);
    setWindowTitle("Area Viewer by Sunny Instruments Singapore");

    connect(sizeSlider, SIGNAL(valueChanged(int)), sv, SLOT(setMaskSize(int)));
//    connect(setButton, SIGNAL(clicked()), sv, SLOT(setMask()));

    sv->forceVidMaskCriteria();
    sv->startVidMask();
}

ViewDialog::~ViewDialog()
{
    delete sv;
    delete sizeSlider;
//    delete setButton;
}

void ViewDialog::closing()
{
    sv->closing();
}

void ViewDialog::keyPressEvent(QKeyEvent *event)
{
    if ((event->key() == Qt::Key_A) && (event->modifiers() & Qt::AltModifier))
    {
        sv->forceVidMaskCriteria();
        sv->startVidMask();
    }
    else if ((event->key() == Qt::Key_C) && (event->modifiers() & Qt::AltModifier))
    {
        sv->reset();
    }
    else if ((event->key() == Qt::Key_S) && (event->modifiers() & Qt::AltModifier))
    {
        sv->setMask();
    }
//    qDebug() << event->key();
    QDialog::keyPressEvent(event);
}
