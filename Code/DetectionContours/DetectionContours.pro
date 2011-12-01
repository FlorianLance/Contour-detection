# -------------------------------------------------
# Project created by QtCreator 2011-11-11T18:34:44
# -------------------------------------------------
TARGET = DetectionContours
TEMPLATE = app
SOURCES += main.cpp \
    fenprincipale.cpp \
    ../src/gradient.cpp \
    ../src/matrix.cpp \
    ../src/seuillage.cpp \
    ../src/progressionAlgo.cpp \
    ../src/hough.cpp
INCLUDEPATH += /include/opencv
LIBS += -lm \
    -lcv \
    -lhighgui \
    -lcvaux
HEADERS += fenprincipale.h \
    ../include/gradient.h \
    ../include/seuillage.h \
    ../include/matrix.h \
    ../include/progressionAlgo.h \
    ../include/hough.h
FORMS += fenprincipale.ui
