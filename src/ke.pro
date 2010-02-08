SOURCES = main.cpp \
	mainwindow.cpp\
	mapframe.cpp \
	sidebar.cpp \
	settings.cpp \
	settingswidget.cpp \
	map.cpp \
	mapsettingsdialog.cpp \
	city.cpp \
	goods.cpp \
	gameparameter.cpp
	
HEADERS += mainwindow.h\
	mapframe.h \
	sidebar.h \
	settings.h \
	settingswidget.h \
	mapsettingsdialog.h \
	map.h \
	mapobject.h \
	city.h \
	qstring.h \
	goods.h \
	gameparameter.h
	
FORMS       = mapsettingsdialog.ui

	
TEMPLATE = app
TARGET = KE
CONFIG += warn_on \
	  thread \
          qt

 QT += svg \
	xml

RESOURCES += icons.qrc

DESTDIR = ../bin/

