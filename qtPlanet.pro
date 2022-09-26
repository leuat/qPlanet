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
    source/engine/entity.cpp \
    source/engine/eriswidget.cpp \
    source/engine/materials/material.cpp \
    source/engine/materials/materialfactory.cpp \
    source/engine/meshes/mesh.cpp \
    source/engine/meshes/meshfactory.cpp \
    source/engine/meshes/meshinstance.cpp \
    source/engine/meshes/meshobject.cpp \
    source/engine/meshes/planet.cpp \
    source/engine/meshes/planetdata.cpp \
    source/engine/meshes/quadplane.cpp \
    source/engine/misc/SimplexNoise.cpp \
    source/engine/misc/objloader.cpp \
    source/engine/misc/qnoise.cpp \
    source/engine/misc/util.cpp \
    source/engine/output/postprocess.cpp \
    source/engine/sdata.cpp \
    source/engine/widget/glwidget.cpp \
    source/engine/world.cpp \
    source/main.cpp \
    source/mainwidget.cpp \
    source/misc/cinifile.cpp \
    source/mymeshes/rotboxtest.cpp

HEADERS += \
    source/engine/camera.h \
    source/engine/entity.h \
    source/engine/eriswidget.h \
    source/engine/materials/material.h \
    source/engine/materials/materialfactory.h \
    source/engine/mesh.h \
    source/engine/meshes/mesh.h \
    source/engine/meshes/meshfactory.h \
    source/engine/meshes/meshinstance.h \
    source/engine/meshes/meshobject.h \
    source/engine/meshes/planet.h \
    source/engine/meshes/planetdata.h \
    source/engine/meshes/quadplane.h \
    source/engine/meshfactory.h \
    source/engine/meshinstance.h \
    source/engine/misc/SimplexNoise.h \
    source/engine/misc/objloader.h \
    source/engine/misc/qnoise.h \
    source/engine/misc/random.h \
    source/engine/misc/util.h \
    source/engine/output/postprocess.h \
    source/engine/sdata.h \
    source/engine/widget/glwidget.h \
    source/engine/world.h \
    source/mainwidget.h \
    source/misc/cinifile.h \
    source/mymeshes/rotboxtest.h

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/opengl/p
#INSTALLS += target


HEADERS += \

RESOURCES += \
    shaders.qrc \
    textures.qrc

# install
#target.path = $$[QT_INSTALL_EXAMPLES]/opengl/p
#INSTALLS += target
