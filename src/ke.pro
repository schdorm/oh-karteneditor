SOURCES = main.cpp \
	mainwidgets/mainwindow.cpp \
	mainwidgets/mapframe.cpp \
	mainwidgets/sidebar.cpp \
	data/settings.cpp \
	dialogs/settingsdialog.cpp \
	dialogs/mapsettingsdialog.cpp \
	dialogs/mapobjectdialog.cpp \
	shared/data/abstractcity.cpp \
	shared/data/goods.cpp \
	shared/data/gameparameter.cpp \
	shared/visual_data/map.cpp \
	shared/libs/qstring.cpp \
	
HEADERS += mainwidgets/mainwindow.h \
	mainwidgets/mapframe.h \
	mainwidgets/sidebar.h \
	data/settings.h \
	dialogs/settingsdialog.h \
	dialogs/mapsettingsdialog.h \
	dialogs/mapobjectdialog.h \
	shared/data/abstractcity.h \
	shared/data/goods.h \
	shared/data/gameparameter.h \
	shared/visual_data/map.h \
	shared/visual_data/mapobject.h \
	shared/libs/qstring.h
	
FORMS = dialogs/mapsettingsdialog.ui \
	dialogs/mapobjectdialog.ui

	
TEMPLATE = app
TARGET = KE
CONFIG += warn_on \
	  thread \
          qt

 QT += svg \
	xml

RESOURCES += icons.qrc

DESTDIR = ../bin/

