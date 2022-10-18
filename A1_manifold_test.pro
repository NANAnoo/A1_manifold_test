QT+=opengl
TEMPLATE = app
TARGET = ManifoldTest
INCLUDEPATH += ./triangle_renderer

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS+= -Wall

# Input
HEADERS += ./triangle_renderer/Ball.h \
           ./triangle_renderer/BallAux.h \
           ./triangle_renderer/BallMath.h \
           ./triangle_renderer/Cartesian3.h \
           ./triangle_renderer/GeometricSurfaceFaceDS.h \
           ./triangle_renderer/GeometricWidget.h

SOURCES += ./triangle_renderer/main.cpp \
           ./triangle_renderer/Ball.cpp \
           ./triangle_renderer/BallAux.cpp \
           ./triangle_renderer/BallMath.cpp \
           ./triangle_renderer/Cartesian3.cpp \
           ./triangle_renderer/GeometricSurfaceFaceDS.cpp \
           ./triangle_renderer/GeometricWidget.cpp