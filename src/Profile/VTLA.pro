RC_ICONS = myico.ico
QT       += core gui webenginewidgets webchannel multimedia

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    boundingbox.cpp \
    dockbar.cpp \
    easyplt.cpp \
    eiginf.cpp \
    fastreader.cpp \
    fixinf.cpp \
    forinf.cpp \
    fstfinf.cpp \
    jscontext.cpp \
    lnkbutton.cpp \
    main.cpp \
    mif.cpp \
    optdock.cpp \
    optplatform.cpp \
    oscgraph.cpp \
    oscplatform.cpp \
    ourteam.cpp \
    rstartree.cpp \
    timeopt.cpp \
    trdfinf.cpp \
    worker.cpp

HEADERS += \
    boundingbox.h \
    dockbar.h \
    easyplt.h \
    eiginf.h \
    fastreader.h \
    fixinf.h \
    forinf.h \
    fstfinf.h \
    jscontext.h \
    lnkbutton.h \
    mif.h \
    optdock.h \
    optplatform.h \
    oscgraph.h \
    oscplatform.h \
    ourteam.h \
    rstartree.h \
    timeopt.h \
    trdfinf.h \
    worker.h

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    image.qrc
