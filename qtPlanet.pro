QT += core gui widgets opengl openglwidgets

TARGET = qtPlanet
TEMPLATE = app

win32-msvc*{
    QMAKE_CXXFLAGS += -openmp
    QMAKE_LFLAGS += -openmp
    LIBS+=-openmp
}
macx{
    CONFIG += arm64
    QMAKE_CXXFLAGS += -Xpreprocessor -fopenmp -I/usr/local/include
    LIBS += -L/usr/local/lib /usr/local/lib/libomp.dylib
}

SOURCES += \
    source/engine/camera.cpp \
    source/main.cpp \
    source/mainwidget.cpp \
    source/materials/materialfactory.cpp \
    source/mesh/meshfactory.cpp \
    source/misc/SimplexNoise.cpp \
    source/misc/entity.cpp \
    source/materials/material.cpp \
    source/misc/mesh.cpp \
    source/misc/meshinstance.cpp \
    source/misc/postprocess.cpp \
    source/misc/sdata.cpp \
    source/planet.cpp \
    source/planetdata.cpp \
    source/qnoise.cpp \
    source/quadplane.cpp \
    source/world.cpp
    source/engine/camera.cpp

HEADERS += \
    source/engine/camera.h \
    source/mainwidget.h \
    source/materials/materialfactory.h \
    source/mesh/meshfactory.h \
    source/misc/entity.h \
    source/materials/material.h \
    source/misc/mesh.h \
    source/misc/meshinstance.h \
    source/misc/postprocess.h \
    source/misc/sdata.h \
    source/planet.h \
    source/mainwidget.h \
    source/misc/SimplexNoise.h \
    source/planet.h \
    source/planetdata.h \
    source/qnoise.h \
    source/quadplane.h \
    source/world.h
    source/engine/camera.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/opengl/p
#INSTALLS += target
