QT += core gui opengl quick

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/glbody.cpp \
        src/glbodygroup.cpp \
        src/glcolorrgba.cpp \
        src/glcourt.cpp \
        src/glesrenderer.cpp \
        src/glhalma.cpp \
        src/glitem.cpp \
        src/glmouseray.cpp \
        src/glmultiplebody.cpp \
        src/glpoint.cpp \
        src/gltableplate.cpp \
        src/gltoken.cpp \
        src/gltokengreen.cpp \
        src/gltokenred.cpp \
        src/main.cpp \
        src/myglitem.cpp \
        src/shaderdebugger.cpp

HEADERS += \
        src/glbody.h \
        src/glbodygroup.h \
        src/glcolorrgba.h \
        src/glcourt.h \
        src/glesrenderer.h \
        src/glhalma.h \
        src/glitem.h \
        src/glmouseray.h \
        src/glmultiplebody.h \
        src/glpoint.h \
        src/gltableplate.h \
        src/gltoken.h \
        src/gltokengreen.h \
        src/gltokenred.h \
        src/myglitem.h \
        src/shaderdebugger.h

RESOURCES += \
    models.qrc \
    qml.qrc \
    shaders.qrc \
    textures.qrc \
    translations.qrc

LIBS +=-lopengl32

TRANSLATIONS += \
    translations/VierGewinnt_de_DE.ts

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
