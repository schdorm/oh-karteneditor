SOURCES = main.cpp \
	mainwindow.cpp\
	mapframe.cpp \
	sidebar.cpp \
	settings.cpp \
	settingswidget.cpp \
	shared/visual_data/map.cpp \
	mapsettingsdialog.cpp \
	shared/data/abstractcity.cpp \
	shared/data/goods.cpp \
	gameparameter.cpp \
	shared/libs/qstring.cpp \
	mapobjectdialog.cpp
	
HEADERS += mainwindow.h\
	mapframe.h \
	sidebar.h \
	settings.h \
	settingswidget.h \
	mapsettingsdialog.h \
	shared/visual_data/map.h \
	shared/visual_data/mapobject.h \
	shared/data/abstractcity.h \
	shared/libs/qstring.h \
	shared/data/goods.h \
	gameparameter.h \
	mapobjectdialog.h
	
FORMS       = mapsettingsdialog.ui \
	mapobjectdialog.ui

	
TEMPLATE = app
TARGET = KE
CONFIG += warn_on \
	  thread \
          qt

 QT += svg \
	xml

RESOURCES += icons.qrc

DESTDIR = ../bin/

