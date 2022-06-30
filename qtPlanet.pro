QT += core gui widgets opengl openglwidgets

TARGET = qtPlanet
TEMPLATE = app


QMAKE_CXXFLAGS += -openmp
QMAKE_LFLAGS += -openmp
LIBS+=-openmp

SOURCES += \
    source/main.cpp \
    source/mainwidget.cpp \
    source/misc/SimplexNoise.cpp \
    source/misc/entity.cpp \
    source/planet.cpp \
    source/planetdata.cpp \
    source/qnoise.cpp \
    source/quadplane.cpp \
    source/world.cpp

HEADERS += \
    source/mainwidget.h \
    source/misc/entity.h \
    source/planet.h \
    source/mainwidget.h \
    source/misc/SimplexNoise.h \
    source/planet.h \
    source/planetdata.h \
    source/qnoise.h \
    source/quadplane.h \
    source/world.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/opengl/p
#INSTALLS += target
