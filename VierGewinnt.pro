QT += core gui opengl quick multimedia

CONFIG += c++11

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
        src/game/viergewinnt.cpp \
        src/game/viergewinntscene.cpp \
        src/gl/glbody.cpp \
        src/gl/glbodygroup.cpp \
        src/gl/glcolorrgba.cpp \
        src/gl/glcourt.cpp \
        src/gl/glesrenderer.cpp \
        src/gl/glitem.cpp \
        src/gl/glmouseray.cpp \
        src/gl/glmultiplebody.cpp \
        src/gl/glpoint.cpp \
        src/gl/gltableplate.cpp \
        src/gl/gltoken.cpp \
        src/gl/gltokengreen.cpp \
        src/gl/gltokenred.cpp \
        src/gl/shaderdebugger.cpp \
        src/sound/soundengine.cpp \
        src/main.cpp

HEADERS += \
        src/game/player.h \
        src/game/viergewinnt.h \
        src/game/viergewinntscene.h \
        src/gl/glbody.h \
        src/gl/glbodygroup.h \
        src/gl/glcolorrgba.h \
        src/gl/glcourt.h \
        src/gl/gldefines.h \
        src/gl/glesrenderer.h \
        src/gl/glitem.h \
        src/gl/glmouseray.h \
        src/gl/glmultiplebody.h \
        src/gl/glpoint.h \
        src/gl/gltableplate.h \
        src/gl/gltoken.h \
        src/gl/gltokengreen.h \
        src/gl/gltokenred.h \
        src/gl/shaderdebugger.h \
        src/sound/soundengine.h

RESOURCES += \
    models.qrc \
    qml.qrc \
    shaders.qrc \
    sounds.qrc \
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
