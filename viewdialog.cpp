#include <QSlider>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QSettings>
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

    setButton = new QPushButton("&Set");

    QHBoxLayout *bottomLayout = new QHBoxLayout();
    bottomLayout->addWidget(sizeSlider);
    bottomLayout->addStretch();
    bottomLayout->addWidget(setButton);

    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->addWidget(sv);
    mainLayout->addLayout(bottomLayout);

    setLayout(mainLayout);
    setFixedSize(1024, 768);
    setWindowTitle("Area Viewer by Sunny Instruments Singapore");

    connect(sizeSlider, SIGNAL(valueChanged(int)), sv, SLOT(setMaskSize(int)));
    connect(setButton, SIGNAL(clicked()), sv, SLOT(setMask()));
    connect(sv, SIGNAL(slideChanged(int)), SLOT(sizeSlot(int)));
}

ViewDialog::~ViewDialog()
{
    delete sv;
    delete sizeSlider;
    delete setButton;
}

void ViewDialog::closing()
{
    sv->closing();
}
