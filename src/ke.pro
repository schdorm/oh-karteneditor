SOURCES = main.cpp \
	mainwindow.cpp\
	mapframe.cpp \
	sidebar.cpp
	
HEADERS += mainwindow.h\
	mapframe.h \
	sidebar.h
	
TEMPLATE = app
TARGET = KE
CONFIG += warn_on \
	  thread \
          qt

 QT += svg

RESOURCES += icons.qrc

DESTDIR = ../bin/

