# makefile of swooz-teleoperation project using Visual C++
# Guillaume Gibert / Florian Lance
# lyon sept 2013

!IF "$(CFG)" != "Release" &&  "$(CFG)" != "Debug"
!ERROR Invalid CFG "($CFG)
!ENDIF

!IF "$(ARCH)" != "x86" &&  "$(ARCH)" != "amd64"
!ERROR Invalid ARCH "($ARCH)
!ENDIF

CC=cl
LINK=link

!INCLUDE <./makefile-include>


############################################################################## OBJ LISTS

CONTOUR_LINK_OBJ=\
    $(LIBDIR)/fenprincipale.obj\
    $(LIBDIR)/gradient.obj\
    $(LIBDIR)/hough.obj\
    $(LIBDIR)/matrix.obj\
    $(LIBDIR)/progressionAlgo.obj\
    $(LIBDIR)/seuillage.obj\

CONTOUR_LINK_MAIN_CV_OBJ=\
    $(LIBDIR)/mainCV.obj

CONTOUR_LINK_MAIN_QT_OBJ=\
    $(LIBDIR)/main.obj

############################################################################## Makefile commands

!if "$(ARCH)" == "x86"
all: $(MOCDIR)/moc_fenprincipale.cpp $(MOCDIR)/moc_progressionAlgo.cpp $(QTGENW)/fenprincipale_ui.h $(BINDIR)/Contour.exe $(BINDIR)/Contour-cv.exe
!endif

!if "$(ARCH)" == "amd64"
all: $(MOCDIR)/moc_fenprincipale.cpp $(MOCDIR)/moc_progressionAlgo.cpp $(QTGENW)/fenprincipale_ui.h $(BINDIR)/Contour-x64.exe $(BINDIR)/Contour-cv-x64.exe
!endif

############################################################################## exe files

WIN_CONFIG = $(SETARGV) $(BINMODE) $(WINLIBS)

# exec files to be generated
$(BINDIR)/Contour.exe: $(CONTOUR_LINK_OBJ) $(CONTOUR_LINK_MAIN_QT_OBJ) $(LIBS_CONTOUR)
        $(LINK) /OUT:$(BINDIR)/Contour.exe $(LFLAGS_CONTOUR) $(CONTOUR_LINK_OBJ) $(CONTOUR_LINK_MAIN_QT_OBJ) $(LIBS_CONTOUR) $(WIN_CONFIG)

$(BINDIR)/Contour-cv.exe: $(CONTOUR_LINK_OBJ) $(CONTOUR_LINK_MAIN_CV_OBJ) $(LIBS_CONTOUR)
        $(LINK) /OUT:$(BINDIR)/Contour-cv.exe $(LFLAGS_CONTOUR) $(CONTOUR_LINK_OBJ) $(CONTOUR_LINK_MAIN_CV_OBJ) $(LIBS_CONTOUR) $(WIN_CONFIG)

$(BINDIR)/Contour-x64.exe: $(CONTOUR_LINK_OBJ) $(CONTOUR_LINK_MAIN_QT_OBJ) $(LIBS_CONTOUR)
        $(LINK) /OUT:$(BINDIR)/Contour-x64.exe $(LFLAGS_CONTOUR) $(CONTOUR_LINK_OBJ) $(CONTOUR_LINK_MAIN_QT_OBJ) $(LIBS_CONTOUR) $(WIN_CONFIG)

$(BINDIR)/Contour-cv-x64.exe: $(CONTOUR_LINK_OBJ) $(CONTOUR_LINK_MAIN_CV_OBJ) $(LIBS_CONTOUR)
        $(LINK) /OUT:$(BINDIR)/Contour-cv-x64.exe $(LFLAGS_CONTOUR) $(CONTOUR_LINK_OBJ) $(CONTOUR_LINK_MAIN_CV_OBJ) $(LIBS_CONTOUR) $(WIN_CONFIG)

##################################################### devices

$(LIBDIR)/fenprincipale.obj: ./src/fenprincipale.cpp
        $(CC) -c ./src/fenprincipale.cpp $(CFLAGS_DYN) $(CONTOUR_INC) -Fo"$(LIBDIR)/fenprincipale.obj"

$(LIBDIR)/gradient.obj: ./src/gradient.cpp
        $(CC) -c ./src/gradient.cpp $(CFLAGS_DYN) $(CONTOUR_INC) -Fo"$(LIBDIR)/gradient.obj"

$(LIBDIR)/hough.obj: ./src/hough.cpp
        $(CC) -c ./src/hough.cpp $(CFLAGS_DYN) $(CONTOUR_INC) -Fo"$(LIBDIR)/hough.obj"

$(LIBDIR)/main.obj: ./src/main.cpp
        $(CC) -c ./src/main.cpp $(CFLAGS_DYN) $(CONTOUR_INC) -Fo"$(LIBDIR)/main.obj"

$(LIBDIR)/mainCV.obj: ./src/mainCV.cpp
        $(CC) -c ./src/mainCV.cpp $(CFLAGS_DYN) $(CONTOUR_INC) -Fo"$(LIBDIR)/mainCV.obj"

$(LIBDIR)/matrix.obj: ./src/matrix.cpp
        $(CC) -c ./src/matrix.cpp $(CFLAGS_DYN) $(CONTOUR_INC) -Fo"$(LIBDIR)/matrix.obj"

$(LIBDIR)/progressionAlgo.obj: ./src/progressionAlgo.cpp
        $(CC) -c ./src/progressionAlgo.cpp $(CFLAGS_DYN) $(CONTOUR_INC) -Fo"$(LIBDIR)/progressionAlgo.obj"

$(LIBDIR)/seuillage.obj: ./src/seuillage.cpp
        $(CC) -c ./src/seuillage.cpp $(CFLAGS_DYN) $(CONTOUR_INC) -Fo"$(LIBDIR)/seuillage.obj"

############################################################################## Qt moc files

$(MOCDIR)/moc_fenprincipale.cpp: ./include/fenprincipale.h
         moc  ./include/fenprincipale.h -o $@

$(MOCDIR)/moc_progressionAlgo.cpp: ./include/progressionAlgo.h
         moc  ./include/progressionAlgo.h -o $@

##################################################### ui

$(QTGENW)/fenprincipale_ui.h: $(FORMDIR)/fenprincipale.ui
        uic $(FORMDIR)/fenprincipale.ui -o $@

