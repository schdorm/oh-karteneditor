/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel and Joerg Thalheim           *
 *   Copyright (C) 2009 - 2010 by Christian Doerffel                       *
 *   oh.devs@googlemail.com                                                *
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

 
 #include "map.h"
 
 #include <QtGui/QGraphicsView>
//  #include <QtGui/QGraphicsScene>
 #include <QtGui/QGraphicsItem>
 
// #include <QtSvg/QGraphicsSvgItem>
 
 #include <QtGui/QMouseEvent>
//  #include <QtGui/QDialog>
//  #include <QtGui/QFileDialog>
 class QWheelEvent;
 
 
 namespace NameFilters{ enum NFs{Img, Map, Save};}
 
 
 class MainWindow;
 class QComboBox;
 
 
class MapFrame : public QGraphicsView
{
  Q_OBJECT
  public:
    
 
    enum
    {
      ID,
      Name,
      Filename,
      Function,
      Tooltip,
    };
 

    MapFrame(const MainWindow *);
    ~MapFrame();
    //  void initMap();
    void newObjectDialog(QPoint);

    void setGraphicsItemProperties(QGraphicsItem *);
    
     Map &mapref();
     Map map() const;
/*    Map *map() { return m_map; }*/
    Map *map();
//     void setMap(const Map *a_map = 0);
    void setMap(const Map &map);

//     void setSMap(const Map &map);
//     Map smap() const {return s_map; }

    int maptyp;
    //   int maptyp;
    //   QString maptypename;
    
    QString bgi_filename; // Backgroundimage-FN
    //  bool isCity;
    QString cityname;
    
    QString mapname;
    
    QString mapnorth, mapwest, mapsouth, mapeast;
    //  QList <ObjectGraphicsItem *> ogilist;
 
 
    //  QStringList MapTypeEntries;
    //  QString mt_sea, mt_coast, mt_land, mt_coast_city, mt_land_city;
    // 
    //  QStringList ObjectTypeEntries;
    //  QString ot_market, ot_townhall, ot_church, ot_port, ot_office, ot_bank, ot_tavern, ot_land, ot_land2, ot_mapdecoration;
    
    QSize mapSize;
    QPoint curser;
 
    //  QGraphicsItem *moveitem;
    //  QGraphicsScene *szene;

//     enum object_types_def
//     {
//       object_type_townhall,
//       object_type_market,
//       object_type_church,
//       object_type_port,
//       object_type_office,
//       object_type_bank,
//       object_type_tavern,
//       object_type_namespacexyz, 
//       object_type_land
//     };
const QGraphicsItem *currentGraphicsItem() const;
const MapObject *currentMapObject	() const;

    QGraphicsItem *activeItem;
    //  QGraphicsPixmapItem *activePixmapItem;
    bool itemSelected;
    bool itemGrabbed;
    int object_typ;
    int object_ZValue;
    QString object_filename;
    QString object_tooltip;
    int x, y;
    QPoint ziel;

//     QFileDialog *fd;
    QDialog *createObjectDialog;
    QString fd_filename;

    QString objectName;

  public slots:
    void newMap();
    void showMap();
//     void saveMap(QString);
//     void loadMap(QString);


    void newObjectDialog_ext();
    void newObject();
    void createObject();
//     void fileDialog(int);
    // void fileDialog(NameFilters::NFs);

    //  void setMapType(QString);
//     void setObjectType(int);
//     void setToolTipString(QString);
//     void setFileString(QString);
 
    void getObjectID(const QString&);
    void selectObject();
 
    void setXPos(int);
    void setYPos(int);
 
  protected:
    void mousePressEvent(QMouseEvent*);
    void mouseMoveEvent(QMouseEvent*);
    void mouseReleaseEvent (QMouseEvent *);
    void wheelEvent ( QWheelEvent * event );
    
    void keyPressEvent(QKeyEvent *);
    
    const MainWindow *m_MainWindow;
    QComboBox *fktComboBox;
    
    
    Map m_smap;
    
    QGraphicsItem *m_currentGraphicsItem;
    MapObject *m_currentMapObject;
    
    
  private:
//    Map *mapinstance();
//      Map *m_map;
     
  signals:
    void newObjectCreated(QGraphicsItem *);
//     void fileStringChanged(QString);
    void objectSelected(QGraphicsItem *);
    void objectMoved();
 
    void SIG_deleteObject();
};
 #endif
