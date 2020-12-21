TEMPLATE = app
CONFIG += console c++11
CONFIG -= app_bundle
CONFIG -= qt

macx:DEFINES+=OSMac_

SOURCES += \
    main.cpp \
    mayaSceneRender.cpp \
    nprnode.cpp \
    renderersettings.cpp \
    utils.cpp \
    nprRenderOverride.cpp \
    quadRender.cpp \
    sceneRender.cpp \
    userRenderOperation.cpp \

macx:TARGET= WatercolorRenderer.bundle

# $$ symbol reads contents of variable, -L specifies a library
macx:MAYALOCATION=/Applications/Autodesk/maya2019
macx:INCLUDEPATH+=$$MAYALOCATION/include
MAYALIBS=-lOpenMaya

LIBS+= -L$$MAYALOCATION/Maya.app/Contents/MacOS \
            -lOpenMaya \
            -lOpenMayaUI \
            -lOpenMayaRender\
            -lOpenMayaAnim\
            -lFoundation


HEADERS += \
    mayaSceneRender.h \
    nprnode.h \
    renderersettings.h \
    utils.h \
    nprRenderOverride.h \
    quadRender.h \
    sceneRender.h \
    userRenderOperation.h \

SUBDIRS += \
    WatercolorRenderer.pro

RESOURCES += \
    ogsfx.qrc
