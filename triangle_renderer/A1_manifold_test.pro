QT+=opengl
TEMPLATE = app
TARGET = ManifoldTest

DEFINES += QT_DEPRECATED_WARNINGS

QMAKE_CXXFLAGS+= -Wall

# Input
HEADERS += ./Ball.h \
           ./BallAux.h \
           ./BallMath.h \
           ./Cartesian3.h \
           ./GeometricSurfaceFaceDS.h \
           ./GeometricWidget.h

SOURCES += ./main.cpp \
           ./Ball.cpp \
           ./BallAux.cpp \
           ./BallMath.cpp \
           ./Cartesian3.cpp \
           ./GeometricSurfaceFaceDS.cpp \
           ./GeometricWidget.cpp