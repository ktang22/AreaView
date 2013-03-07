TEMPLATE = app
QT += core gui

#CONFIG += console
WORKSPACE = $$quote($$(SISWORKSPACE))

INCLUDEPATH += $$quote(C:\opencv\build\include)

LIBS += -L$$quote(C:\opencv\build\x86\vc10\lib) -lopencv_core -lopencv_imgproc -lopencv_highgui -lopencv_ml -lopencv_video -lopencv_features2d -lopencv_calib3d -lopencv_objdetect -lopencv_contrib -lopencv_legacy -lopencv_flann

SOURCES += \
    main.cpp \
    viewdialog.cpp \
    imageframe.cpp \
    sceneview.cpp \
    maskitem.cpp

HEADERS += \
    viewdialog.h \
    imageframe.h \
    sceneview.h \
    maskitem.h
