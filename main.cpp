#include <QApplication>
#include <QIcon>
#include "viewdialog.h"

int main(int argc, char* argv[])
{
    QApplication app(argc, argv);

    app.setWindowIcon(QIcon("icon.ico"));

    ViewDialog vd;
    vd.show();

    QObject::connect(qApp, SIGNAL(aboutToQuit()), &vd, SLOT(closing()));

    return app.exec();
}
