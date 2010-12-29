QT += svg
HEADERS += core/svgresizingview.h \
    core/resizingview.h \
    gamewindow.h \
    mazeview.h \
    core/clickablesvgitem.h \
    core/svgtheme.h \
    campaign.h \
    startupdialog.h \
    mazeeditormodel.h \
    mazegameview.h \
    mazeeditorview.h \
    mazemodeldata.h \
    mazegamemodel.h
SOURCES += core/svgresizingview.cpp \
    core/resizingview.cpp \
    main.cpp \
    gamewindow.cpp \
    mazeview.cpp \
    core/clickablesvgitem.cpp \
    startupdialog.cpp \
    mazeeditormodel.cpp \
    mazegameview.cpp \
    mazeeditorview.cpp \
    mazemodeldata.cpp \
    mazegamemodel.cpp
RESOURCES += resources.qrc
FORMS += gamewindow.ui \
    startupdialog.ui
