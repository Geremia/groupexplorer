
LANGUAGE	= C++

CONFIG	+= qt warn_on release opengl uic3 x86 ppc
QT += xml  opengl qt3support 

VPATH += src
DEPENDPATH += src
INCLUDEPATH += src

unix {
  UI_DIR = src/.ui
  MOC_DIR = src/.moc
  OBJECTS_DIR = src/.obj
}

HEADERS	+= src/group.h \
	src/idealist.h \
        src/groupfile.h \
        src/ge.h \
        src/geml.h \
        src/mathtableitem.h \
        src/idea.h \
        src/mathtext.h \
        src/grouptable.h \
        src/gepreferences.h \
        src/gehelpbrowser.h \
        src/linkcommand.h \
        src/linkcommandlist.h \
        src/mainwindowcmds.h \
        src/gewhatsthis.h \
        src/gevisualizer.h \
        src/gevisualizerlist.h \
        src/multiplicationtable.h \
        src/cyclegraph.h \
        src/utilpieces.h \
        src/sympanewidgets.h \
        src/subseteditor.h \
        src/subsetcomputations.h \
        src/symmetryobject.h \
        src/modelviewwidgets.h \
        src/gesheet.h \
        src/gesheetwindowcmds.h \
        src/insvisdialog.h \
        src/cayleydiagram.h \
        src/geltgrid.h \
        src/cdwidgets.h \
        src/actionforwarder.h

SOURCES	+= src/group.cpp \
	src/idealist.cpp \
        src/groupfile.cpp \
        src/main.cpp \
        src/geml.cpp \
        src/mathtableitem.cpp \
        src/mathtext.cpp \
        src/grouptable.cpp \
        src/gepreferences.cpp \
        src/gehelpbrowser.cpp \
        src/linkcommand.cpp \
        src/linkcommandlist.cpp \
        src/mainwindowcmds.cpp \
        src/gewhatsthis.cpp \
        src/gevisualizer.cpp \
        src/gevisualizerlist.cpp \
        src/multiplicationtable.cpp \
        src/cyclegraph.cpp \
        src/utilpieces.cpp \
        src/sympanewidgets.cpp \
        src/subseteditor.cpp \
        src/subsetcomputations.cpp \
        src/symmetryobject.cpp \
        src/modelviewwidgets.cpp \
        src/gesheet.cpp \
        src/gesheetwindowcmds.cpp \
        src/insvisdialog.cpp \
        src/cayleydiagram.cpp \
        src/geltgrid.cpp \
        src/cdwidgets.cpp \
        src/actionforwarder.cpp

# !contains( DEFINES, NO_QT_UNDO ) {
#     include(qtundo.pri)
# } else {
#     HEADERS += src/qtundo.h
#     SOURCES += src/qtundo.cpp
# }

#The following line was changed from FORMS to FORMS3 by qt3to4
FORMS3	= src/mainwindow.ui \
	src/preferencesdialog.ui \
	src/textinputdialog.ui \
	src/nameschemedialog.ui \
	src/gehelpwindow.ui \
	src/searchdialog.ui \
	src/gevlarge.ui \
	src/gesheetwindow.ui \
	src/sheetsizedialog.ui \
	src/morphismedit.ui

IMAGES	= images/filenew.png \
        images/fileopen.png \
        images/filesave.png \
        images/print.png \
        images/undo \
        images/redo \
        images/editcut.png \
        images/editcopy.png \
        images/editpaste.png \
        images/searchfind.png \
        images/options.png \
        images/helpdocs.png \
        images/ge2circle.png \
        images/biglogo.png \
        images/previous.png \
        images/next.png \
        images/decrease_font.png \
        images/increase_font.png \
        images/home.png \
        images/tutorials.png \
        images/searchdocs.png \
        images/zoomin.png \
        images/zoomout.png \
        images/insert_text.png \
        images/adjustsize.png \
        images/insert_rectangle.png \
        images/bringforward.png \
        images/pushbackward.png \
        images/insert_line.png \
        images/insert_visualizer.png \
        images/insert_morphism.png \
        images/filesaveimage.png \
        images/idea.png \
        images/delete.png \
        images/helpsheets.png

include(useful.pri)
include(GLonly.pri)

macx:QMAKE_POST_LINK = ./finalize.sh

macx:RC_FILE = Resources/GroupExplorer.icns
win32:RC_FILE = Resources/GroupExplorer.rc

