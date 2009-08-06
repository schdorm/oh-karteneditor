/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel and Joerg Thalheim  *
 *   oh.devs@googlemail.com   *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License, or     *
 *   (at your option) any later version.                                   *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/
 
 #ifndef _MAPFRAME_H
 #define _MAPFRAME_H
 
 #define GI2GPMI(x) dynamic_cast<QGraphicsPixmapItem *>(x)

 
 #include <QtCore/QMap>
 
 #include <QtGui/QGraphicsView>
 #include <QtGui/QGraphicsScene>
 #include <QtGui/QGraphicsItem>
 
#include <QtSvg/QGraphicsSvgItem>
 
 #include <QtGui/QMouseEvent>
 #include <QtGui/QDialog>
 #include <QtGui/QFileDialog>
 

namespace MapType
	{
	enum mtyp
		{
		 sea,			// 000
		 coast,			// 001 || -> | -> 110
		 land,			// 010 || -> | -> 111
		 coast_city,
		 land_city			// 100
		};
	}

 namespace NameFilters{ enum NFs{Img, Map, Save};}
 
 class MapFrame : public QGraphicsView
 {
 Q_OBJECT

 public:
 
 void initMap();
 void newObjectDialog(QPoint);

  MapType::mtyp maptyp;
//   int maptyp;
//   QString maptypename;
  
 QString bgi_filename; // Backgroundimage-FN
//  bool isCity;
 QString cityname;
 QString mapnorth, mapwest, mapsouth, mapeast;
 QMap <QGraphicsItem *, QGraphicsPixmapItem *> pixmapItemList;
 QList <QGraphicsItem *> qgilist;
//  QMap <QVariant , QGraphicsItem*> itemMapList; 
QMap <QString , QGraphicsItem*> itemMapList; 

//  QStringList itemfktList;
 
 QStringList MapTypeEntries;
 QString mt_sea, mt_coast, mt_land, mt_coast_city, mt_land_city;

 QStringList ObjectTypeEntries;
 QString ot_market, ot_townhall, ot_church, ot_port, ot_office, ot_bank, ot_tavern, ot_land, ot_land2, ot_mapdecoration;

 QSize mapSize;
 QPoint curser;
 
 
 QGraphicsScene *szene;

enum object_types_def
{
object_type_townhall,
object_type_market,
object_type_church,
object_type_port,
object_type_office,
object_type_bank,
object_type_tavern,
object_type_namespacexyz, 
object_type_land


};


 QGraphicsPixmapItem *activePixmapItem;
 QGraphicsItem *activeItem;
 bool itemSelected;
 bool itemGrabbed;
 int object_typ;
 int object_ZValue;
 QString object_filename;
 QString object_tooltip;
 int x, y;
 QPoint ziel;

 QFileDialog *fd;
 QDialog *createObjectDialog;
 QString fd_filename;

QString objectName;

 public slots:
 void newMap();
 void saveMap(QString);
 void loadMap(QString);


 void newObjectDialog_ext();
 void newObject();
 void createObject();
 void fileDialog(int);
// void fileDialog(NameFilters::NFs);

 void setMapType(QString);
 void setObjectType(QString);
 void setToolTipString(QString);
 void setFileString(QString);
 
 void getObjectID(QString);
 void selectObject();
 
 void setXPos(int);
 void setYPos(int);
 
 protected:
 void mousePressEvent(QMouseEvent*);
 void mouseMoveEvent(QMouseEvent*);
 void mouseReleaseEvent (QMouseEvent *);
 
 void keyPressEvent(QKeyEvent *);
 
 
 signals:
 void newObjectCreated(QGraphicsItem *);
 void fileStringChanged(QString);
 void objectSelected(QGraphicsItem *);
 void objectMoved();
 
 void SIG_deleteObject();
 };
 #endif
