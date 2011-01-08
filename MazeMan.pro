QMAKE_CXXFLAGS += -g -O0 -ansi -pedantic -Wall -Wextra
QT += svg
HEADERS += dialogs/startupdialog.h \
    dialogs/mapeditordialog.h \
    dialogs/createmapdialog.h \
    core/svgtheme.h \
    core/svgresizingview.h \
    core/resizingview.h \
    core/clickablesvgitem.h \
    mazemodeldata.h \
    mazeview.h \
    mazegameview.h \
    mazegamemodel.h \
    mazeeditorview.h \
    mazeeditormodel.h \
    gamewindow.h \
    campaign.h \
    dialogs/textbuttonsdialog.h
SOURCES += dialogs/startupdialog.cpp \
    dialogs/mapeditordialog.cpp \
    dialogs/createmapdialog.cpp \
    core/svgresizingview.cpp \
    core/resizingview.cpp \
    core/clickablesvgitem.cpp \
    mazemodeldata.cpp \
    mazeview.cpp \
    mazegameview.cpp \
    mazegamemodel.cpp \
    mazeeditorview.cpp \
    mazeeditormodel.cpp \
    main.cpp \
    gamewindow.cpp \
    dialogs/textbuttonsdialog.cpp
FORMS += dialogs/startupdialog.ui \
    dialogs/mapeditordialog.ui \
    dialogs/createmapdialog.ui \
    gamewindow.ui \
    dialogs/textbuttonsdialog.ui
RESOURCES += resources.qrc
