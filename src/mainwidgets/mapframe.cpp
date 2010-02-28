/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel and Joerg Thalheim           *
 *   Copyright (C) 2009 - 2010 by Christian Doerffel                       *
 *   oh.devs@googlemail.com  						   *
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

 #include "mapframe.h"
 #include "mainwindow.h"
 #include "sidebar.h"
 #include "settings.h"

 #include <QtCore/QDebug>
 #include <QtCore/QList>
 #include <QtCore/QDir>
 #include <QtCore/QFile>
 #include <QtCore/QTextStream>
 #include <QtGui/QComboBox>
 #include <QtGui/QLabel>
 #include <QtGui/QGridLayout>
 #include <QtGui/QHBoxLayout>
 #include <QtGui/QPushButton>
 #include <QtGui/QLineEdit>
 #include <QtGui/QSpinBox>
 
 #include <QtGui/QImageReader>
 
 #include <QtGui/QScrollBar>
 
 #include <QtGui/QKeyEvent>
 #include <QtGui/QWheelEvent>

 #include <QtCore/QStringList> 
 #include <QtCore/QSignalMapper>

#include <QtCore/QXmlStreamReader>


#include <QtSvg/QGraphicsSvgItem>
#include <QtSvg/QSvgRenderer>

#include <QtGui/QDialog>

#include <math.h>

//  #include <QtGui/QPixmap>
 
 
MapFrame::MapFrame( const MainWindow *mainwin) : m_ChangePerspective (false), m_translationD (0), m_scaleF (1), m_rotateXAngle(0), m_rotateYAngle(0), m_rotateZAngle(0), m_ObjectDialog (0), m_currentGraphicsItem (0), m_currentMapObject(0)
{
  qWarning() << "MapFrame::MapFrame( const MainWindow *mainwin)";
   
  m_MainWindow = mainwin;
//   activeItem = 0;
//   m_map = 0;
  QGraphicsScene *szene = new QGraphicsScene();
  setScene(szene);
  newMap();
  /*
//  ot_townhall = tr("Rathaus");
//  ot_market = tr("Markt");
//  ot_church = tr("Kirche");
//  ot_port = tr("Hafen");
//  ot_office = tr("Kontor");
//  ot_bank = tr("Darlehensgeber");
//  ot_tavern = tr("Kneipe");
//  ot_land = tr("Landflaechen");
//  ot_land2 = tr("\"boese\" Untiefen"); // Objekte, bei denen eine Kollision mit dem Schiff Schaden am Schiff verursacht.
//  ot_mapdecoration = tr("Mapdeko");
//  ObjectTypeEntries << ot_townhall << ot_market << ot_church << ot_port << ot_office << ot_bank << ot_tavern << ot_land << ot_land2 << ot_mapdecoration;
//  
//  mt_sea = tr("See/Meer");
//  mt_coast = tr("Kueste");
//  mt_land = tr("Land");
//  mt_coast_city = tr("Kueste (Stadt)");
//  mt_land_city = tr("Land (Stadt)");
//  MapTypeEntries << mt_sea  << mt_coast << mt_land << mt_coast_city << mt_land_city;*/
 
  m_itemGrabbed = false;
}

MapFrame::~MapFrame()
{
//   delete m_map;
}


void MapFrame::newMap()
{
  m_smap = Map();
  qWarning() << "void MapFrame::newMap()";
//   maptyp = Map::Sea;
//   bgi_filename = QString();
//   mapnorth = QString(); mapwest = QString();  mapsouth = QString(); mapeast = QString();
  
  scene()->clear();
  setSceneRect(QRectF(QPoint(0, 0), m_smap.size()));
  m_itemSelected = false;
  scene()->setBackgroundBrush(QBrush());
}

void MapFrame::saveMap(const QString &a_filename)
{
  m_smap.save(a_filename);
}

bool MapFrame::loadMap(const QString &a_filename)
{
  if(m_smap.load(a_filename))
  {
    applyMapSettings();
    repaintMap();
    return true;
  }
  else
    return false;
}


void MapFrame::repaintMap()		// clears the scene and adds each Object again to it
{
  scene()->clear();
  
  QList<MapObject> f_molist = m_smap.objects();
  int f_moid;
  QString f_imgpath;
  QGraphicsItem *f_graphicsItem = 0;
  
  for(QList<MapObject>::iterator f_moit = f_molist.begin(); f_moit != f_molist.end(); ++f_moit)
  {
      f_imgpath = f_moit->imagepath();
      //       if(imgpath.endsWith(".png", Qt::CaseInsensitive) || imgpath.endsWith(".jpg", Qt::CaseInsensitive) || imgpath.endsWith(".jpeg", Qt::CaseInsensitive) || imgpath.endsWith(".gif ") || imgpath.endsWith(".ppm", Qt::CaseInsensitive) || imgpath.endsWith(".tiff", Qt::CaseInsensitive) || imgpath.endsWith(".xbm", Qt::CaseInsensitive) || imgpath.endsWith(".xpm", Qt::CaseInsensitive))
      if(QImageReader(f_imgpath).canRead())
      {
	f_moid = f_moit->id();
	if(!f_imgpath.endsWith(".svg", Qt::CaseInsensitive))
	{
	  QGraphicsPixmapItem *f_pixmapItem = scene()->addPixmap(QPixmap(f_imgpath));
	  f_graphicsItem = f_pixmapItem;
	}
	else
	{
	  QGraphicsSvgItem *f_svgItem = new QGraphicsSvgItem(f_imgpath);
	  scene()->addItem(f_svgItem);
	  f_graphicsItem = f_svgItem;
	}
	if(!f_graphicsItem == 0)
	{
	  f_graphicsItem->setData(Qt::UserRole, QVariant(f_moid));
	  f_graphicsItem->setPos(f_moit->position());
	  f_graphicsItem->setToolTip(f_moit->tooltip());
	  f_graphicsItem->setZValue(f_moit->zValue());
	  f_graphicsItem = 0;
	}
      }
      else
	qWarning() << "Image hasn't a readable Format!";
    
  }
}

void MapFrame::showMap()		/// generates a viewable result of the data stored in m_smap
{
  QList<MapObject> f_molist = m_smap.objects();
  QList<QGraphicsItem *> f_gilist = scene()->items();
  QList<QGraphicsItem *>::iterator f_giit;
  bool f_gifound;	// true, if graphicsitem exists
  int f_moid;
  QString f_imgpath;
  QGraphicsItem *f_graphicsItem = 0;
  
  for(QList<MapObject>::iterator f_moit = f_molist.begin(); f_moit != f_molist.end(); ++f_moit)
  {
    f_gifound = false;
    f_moid = f_moit->id();
    for(f_giit = f_gilist.begin(); f_giit != f_gilist.end() && !f_gifound; ++f_giit)
    {
      f_gifound = ((*f_giit)->data(Qt::UserRole).toInt() == f_moid);
    }
    if(!f_gifound)
    {
      f_imgpath = f_moit->imagepath();
      //       if(imgpath.endsWith(".png", Qt::CaseInsensitive) || imgpath.endsWith(".jpg", Qt::CaseInsensitive) || imgpath.endsWith(".jpeg", Qt::CaseInsensitive) || imgpath.endsWith(".gif ") || imgpath.endsWith(".ppm", Qt::CaseInsensitive) || imgpath.endsWith(".tiff", Qt::CaseInsensitive) || imgpath.endsWith(".xbm", Qt::CaseInsensitive) || imgpath.endsWith(".xpm", Qt::CaseInsensitive))
      if(QImageReader(f_imgpath).canRead())
      {
	if(f_imgpath.endsWith(".svg", Qt::CaseInsensitive))
	{
	  QGraphicsPixmapItem *f_pixmapItem = scene()->addPixmap(QPixmap(f_imgpath));
	  f_graphicsItem = f_pixmapItem;
	}
	else
	{
	  QGraphicsSvgItem *f_svgItem = new QGraphicsSvgItem(f_imgpath);
	  scene()->addItem(f_svgItem);
	  f_graphicsItem = f_svgItem;
	}
	if(!f_graphicsItem == 0)
	{
	  f_graphicsItem->setData(Qt::UserRole, QVariant(f_moid));
	  f_graphicsItem->setPos(f_moit->position());
	  f_graphicsItem->setToolTip(f_moit->tooltip());
	  f_graphicsItem->setZValue(f_moit->zValue());
	  f_graphicsItem = 0;
	}
      }
      else
	qWarning() << "Image hasn't a readable Format!";
    }
  }
}



void MapFrame::addObject(MapObject *a_newObject)
{
  m_smap.addObject(a_newObject);
  QGraphicsItem *f_graphicsItem;
  QString f_imgpath = a_newObject->imagepath() ;
    qWarning() << "void MapFrame::addObject(MapObject *a_newObject)"<< f_imgpath << a_newObject->id();

  //       if(imgpath.endsWith(".png", Qt::CaseInsensitive) || imgpath.endsWith(".jpg", Qt::CaseInsensitive) || imgpath.endsWith(".jpeg", Qt::CaseInsensitive) || imgpath.endsWith(".gif ") || imgpath.endsWith(".ppm", Qt::CaseInsensitive) || imgpath.endsWith(".tiff", Qt::CaseInsensitive) || imgpath.endsWith(".xbm", Qt::CaseInsensitive) || imgpath.endsWith(".xpm", Qt::CaseInsensitive))
  if(QImageReader(f_imgpath).canRead())
  {
    qWarning() << "QImageReader(f_imgpath).canRead() = true";
    if(!f_imgpath.endsWith(".svg", Qt::CaseInsensitive))
    {
      QGraphicsPixmapItem *f_pixmapItem = scene()->addPixmap(QPixmap(f_imgpath));
      f_graphicsItem = f_pixmapItem;
    }
    else
    {
      QGraphicsSvgItem *f_svgItem = new QGraphicsSvgItem(f_imgpath);
      scene()->addItem(f_svgItem);
      f_graphicsItem = f_svgItem;
    }
    if(!f_graphicsItem == 0)
    {
      f_graphicsItem->setData(Qt::UserRole, QVariant(a_newObject->id()));
      f_graphicsItem->setPos(a_newObject->position());
      f_graphicsItem->setToolTip(a_newObject->tooltip());
      f_graphicsItem->setZValue(a_newObject->zValue());
    }
  }
  else
    qWarning() << "Image hasn't a readable Format!";
  emit dataChanged();
}


void MapFrame::updateObject(int a_id)
{
  qWarning() << "void MapFrame::updateObject(int a_id)";
  QList<QGraphicsItem *> f_gilist = scene()->items();
  QList<QGraphicsItem *>::iterator f_giit;
  bool f_gifound(false);	// true, if graphicsitem exists
  const MapObject *f_mo = m_smap.object(a_id);
  
  for(f_giit = f_gilist.begin(); f_giit != f_gilist.end() && !f_gifound; ++f_giit)
  {
    if((*f_giit)->data(Qt::UserRole).toInt() == a_id)
    {
      QGraphicsItem *f_graphicsItem = (*f_giit);
      f_graphicsItem->setPos(f_mo->position());
      f_graphicsItem->setToolTip(f_mo->tooltip());
      f_graphicsItem->setZValue(f_mo->zValue());
      f_gifound = true;
    }
  }
  emit dataChanged();
}

void MapFrame::updateObjectImage(int a_id)
{
  QList<QGraphicsItem *> f_gilist = scene()->items();
  QList<QGraphicsItem *>::iterator f_giit;
  bool f_gifound(false);			// true, if graphicsitem exists --> aborting the for-loop
  const MapObject *f_mo = m_smap.object(a_id);
  QString f_imagepath = f_mo->imagepath();
  if(QFile::exists(f_imagepath) && QImageReader(f_imagepath).canRead())
  {
    for(f_giit = f_gilist.begin(); f_giit != f_gilist.end() && !f_gifound; ++f_giit)
    {
      if((*f_giit)->data(Qt::UserRole).toInt() == a_id)
      {
	QGraphicsItem *f_graphicsItem = (*f_giit);
	f_gifound = true;
	if(!f_imagepath.endsWith(".svg", Qt::CaseInsensitive))
	{
	  QGraphicsPixmapItem *f_GPI = qgraphicsitem_cast<QGraphicsPixmapItem*>(f_graphicsItem);
	  if(f_GPI != 0)
	  {
	    f_GPI->setPixmap(QPixmap(f_imagepath));
	  }
	}
	else /*if(f_imagepath.endsWith(".svg", Qt::CaseInsensitive))*/
	{
	  QGraphicsSvgItem *f_GSvgItem = qgraphicsitem_cast<QGraphicsSvgItem*>(f_graphicsItem);
	  if(f_GSvgItem != 0)
	  {
	    QSvgRenderer *f_renderer = new QSvgRenderer (f_imagepath);
	    f_GSvgItem->setSharedRenderer(f_renderer);
	    
	    // 	  f_GSvgI->setPixmap(QPixmap(f_mo->imagepath()));
	  }
	}
      }
    }
  }
    emit dataChanged();

}

void MapFrame::applyMapSettings()
{
  if(QFile(m_smap.background()).exists())
  {
    scene()->setBackgroundBrush(QBrush(QPixmap(m_smap.background())));
  }
  
  setSceneRect(QRectF(sceneRect().topLeft(), m_smap.size()));
}

void MapFrame::setMap(const Map &a_map)
{
  #ifdef DEBUG_MAPFRAME
  qWarning() << "void MapFrame::setMap(const Map &a_map)" << a_map.type();
  #endif
  
  m_smap <= a_map;
  
  #ifdef DEBUG_MAPFRAME
  qWarning() << "Map setted" << m_smap.type();
  
  
  if(m_smap.isCity())
    qWarning() << "void MapFrame::setMap(const Map &a_map) ||| m_smap: Cityname: " << m_smap.city()->name();
  #endif
  //     *mapinstance() <= a_map;
  
  //   qWarning() << "Map setted" << mapinstance()->type();
}


Map *MapFrame::map()
{
  return &m_smap;
}

Map &MapFrame::mapref()
{
  return m_smap;
}

Map MapFrame::map() const
{
  return m_smap;
}

void MapFrame::setCurrentItem(int a_id)
{
  //   QGraphicsItem *f_item;
  qWarning() << "void MapFrame::setCurrentItem(int a_id)";
  if(m_currentGraphicsItem->data(Qt::UserRole).toInt() != a_id)
  {
    for(QList<QGraphicsItem*>::const_iterator f_item = scene()->items().begin(); f_item != scene()->items().end() && (*f_item)->data(Qt::UserRole).toInt() == a_id; ++ f_item)
    {
      m_currentGraphicsItem = *f_item;
    }
  }
  qWarning() << "current GraphicsItem setted; following Object";
  if(m_currentMapObject == 0)
  {
    m_currentMapObject = m_smap.object(a_id);
  }
  else if(m_currentMapObject->id() != a_id)
  {
    //     const QList<MapObject*> &molist = map()->objects();
//     QList<MapObject> molist = m_smap.objects();
//     for(QList<MapObject>::iterator f_mo = molist.begin(); f_mo != molist.end() || f_mo->id() == a_id; ++ f_mo)
//     {
//       m_currentMapObject = &*f_mo;
	 m_currentMapObject = m_smap.object(a_id);
//     }
  }
}



const QGraphicsItem * MapFrame::currentGraphicsItem() const
{
  return m_currentGraphicsItem;
}

const MapObject * MapFrame::currentMapObject () const
{
  return m_currentMapObject;
}


void MapFrame::mousePressEvent(QMouseEvent *event)
{
  m_mousepos = (mapToScene(event->pos()));
//   m_mousepos = (mapToScene(event->pos() + QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value())));
//   m_mousepos = mapToScene(event->pos());

  if(m_ChangePerspective)
  {
    qWarning() << "Change Perspective ...";
    return;
  }
  qWarning() << "MapFrame::mousePressEvent(QMouseEvent *event)";
  if(event->buttons() == Qt::LeftButton)
  {
    if(!m_itemGrabbed)
    {
//       int tx = event->x() + horizontalScrollBar()->value();
//       int ty = event->y() + verticalScrollBar()->value();
      
      QList<QGraphicsItem *> f_gilist = scene()->items(m_mousepos.toPoint());
      
//       qWarning() << "items(event->pos()); - Anzahl:" <<  f_gilist.size() << "Pos: " << event->pos() << m_mousepos << mapToScene(event->pos()) << event->pos() + QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value()) << (event->pos() + QPoint(horizontalScrollBar()->value(), verticalScrollBar()->value()) == mapToScene(event->pos()));
//       QGraphicsItem *item;
//       foreach(item, scene()->items())
// 	qWarning() << mapFromScene(item->sceneBoundingRect()) << item->sceneBoundingRect() << item->sceneBoundingRect().contains(m_mousepos) << item->sceneBoundingRect().contains(event->pos());
      
      if(f_gilist.isEmpty())
      {
	event->ignore();
	return;
      }
      else if(f_gilist.size() == 1 && !f_gilist.contains(m_currentGraphicsItem))
      {
	m_currentGraphicsItem = f_gilist.first();
	setCurrentItem(m_currentGraphicsItem->data(Qt::UserRole).toInt());
	
	emit objectSelected(m_currentGraphicsItem->data(Qt::UserRole).toInt());
	m_itemSelected = true;
	
      }
      else if(f_gilist.contains(m_currentGraphicsItem) && m_itemSelected)
      {
	m_itemGrabbed = true;
      }
      
    }
  }
  else if(event->button() == Qt::RightButton)
  {
//     activeItem = 0;
    m_currentGraphicsItem = 0;
    m_currentMapObject = 0;
    m_itemSelected = false;
    m_itemGrabbed = false;
    emit objectSelected( -1 );
  }
}
// 	else
	// 	{
	  // 		ObjectGraphicsItem *tempogi = 0;
	  /*		QList<ObjectGraphicsItem *> matchingOGI;
	  for(QList<QGraphicsItem *>::iterator Cit = QGIlistAtClick.begin(); Cit != QGIlistAtClick.end(); ++Cit)
	  // QGraphicsItem *Cit = 0;
	  // 		foreach(Cit, QGIlistAtClick)
	  {
	    // 		qWarning() << "test";
	    for(QList<ObjectGraphicsItem*>::iterator it = ogilist.begin(); it != ogilist.end(); ++it)
	    {
	      qWarning() << "test2" << (*it)->data(0).toInt() << (*Cit)->data(0).toInt() << (*it)->pos() << (*Cit)->pos();
	      //  				if((*it)->pos() == (*Cit)->pos())
	      {
		// 				qWarning() << "test 3";
		if((*it)->data(0).toInt() == (*Cit)->data(0).toInt() *//*&& (*it)->toolTip() == (*Cit)->toolTip()*//*)
		{
		  qWarning() << "test 4";
		  matchingOGI << *it;
		  break;
	      }
	    }
	    }
	    // 			tempogi = qgraphicsitem_cast<ObjectGraphicsItem*>(Cit);
	    // 			tempogi = dynamic_cast<ObjectGraphicsItem*>(Cit);
	    // 			qWarning() << "gecastet";
	    // 			if(tempogi != 0)
	    // 			{
	      // 			matchingOGI << tempogi;
	      // 			qWarning() << "added To List";
	      // 			}
	  }
	  qWarning() << "OGI-Size:" << matchingOGI.size();
	  */
	  
	  // ---> dynamic cast // qgraphicsitem_cast(qgi)
	  // 		ObjectGraphicsItem *OGI;
/// ***************************************************************************************************//{
/*			else
			{
			  
			QDialog *selectObjectDialog = new QDialog(this);
			selectObjectDialog->setModal(true);
			
			QComboBox *objectList = new QComboBox(selectObjectDialog);
			
			QStringList objectNameList;	QGraphicsItem *GIit;*/
/// ***************************************************************************************************//}
// 			ObjectGraphicsItem *ogiit;	//ObjectGraphicsItemIterator
// 			foreach(ogiit, matchingOGI)
// 			{
// 				objectNameList << QString(qgiit->data(17).toString()).append(QString("; ").append(qgiit->data(1).toString()).append(QString(" ...").append(qgiit->data(2).toString().right(15))));
// 				objectNameList << ogiit->data(Name).toString();
//  			}
/// ***************************************************************************************************//{
/*
			foreach(GIit, gilist)
			{
				objectNameList << GIit->data(Name).toString();
			}


			objectList->addItems(objectNameList) ;
			QPushButton *ok = new QPushButton ("Ok", selectObjectDialog);
			QHBoxLayout *sodlayout = new QHBoxLayout(selectObjectDialog);
			sodlayout->addWidget(objectList);
			sodlayout->addWidget(ok);
			selectObjectDialog->setLayout(sodlayout);
			selectObjectDialog -> show();
			connect(ok, SIGNAL(clicked()), selectObjectDialog, SLOT(deleteLater()));
			connect(ok, SIGNAL(clicked()), this, SLOT(selectObject()));
			connect(objectList, SIGNAL(activated(QString)), this, SLOT(getObjectID(QString)));
			getObjectID(objectList->currentText());
// 			qgilist = QGIlistAtClick;
			}*/
/// ***************************************************************************************************//}

qreal toRad(qreal arg)
{
  const qreal PI = 3.1415926535;
  return (arg * PI / 180);
}

void MapFrame::mouseMoveEvent(QMouseEvent *event)
{
  //  qWarning() << "void MapFrame::mouseMoveEvent(QMouseEvent *event)";
  // setStatusTip(QString("%1 %2").arg(curser.x(), curser.y()));
  QPointF point (mapToScene(event->pos()));

  const qreal f_dx = point.x() - m_mousepos.x();
  const qreal f_dy = point.y() - m_mousepos.y();
  if(m_itemGrabbed)
  {
    m_currentGraphicsItem->moveBy(f_dx, f_dy);
     usleep(4000);
    m_currentMapObject->setPosition(m_currentGraphicsItem->pos());
    /*
    m_currentMapObject->moveBy(f_dx, f_dy);
    usleep(2000);
    m_currentGraphicsItem->setPos(m_currentMapObject->position());*/

  }
  else if(m_ChangePerspective)
  {
     m_rotateXAngle += (cos(toRad(m_rotateYAngle)) * f_dy + (sin(toRad(m_rotateYAngle)) * f_dx))/100;
     m_rotateYAngle += (cos(toRad(m_rotateXAngle)) * f_dx + (sin(toRad(m_rotateXAngle)) * f_dy))/100;
     //      m_rotateZAngle;
     setViewTransform();
  }
  m_mousepos = point;

//  	activeItem->moveBy( event->x() - oldxpos,  event->y() - oldypos);
// 	emit objectMoved();

}


void MapFrame::mouseReleaseEvent(QMouseEvent *event)
{
  if(m_itemGrabbed)
  {
    m_itemGrabbed = false;
    //   itemSelected=false;
    emit objectMoved(m_currentGraphicsItem->data(Qt::UserRole).toInt());
  }
  if(event->button() == Qt::RightButton)
  {
    m_itemSelected = false;
  }
  emit dataChanged();
  
}

///************************************
// void MapFrame::getObjectID(const QString &objname)
// {
// QString copy = objname;
// int n = 0;
// while(!objname.left(n).contains(";"))
// {
// n++;
// }

/*qWarning() << objname/*.left(n-1)<< n*/;
/*objectName = objname/*.left(n-1)*/;
 /*QGraphicsItem *it;

 foreach(it, scene()->items())
{
	if(it->data(Name).toString() == objectName)
	{
	qWarning() << "ActiveItem->ID before" << activeItem->data(ID).toInt();
		activeItem = it;
	qWarning() << "ActiveItem->ID new" << activeItem->data(ID).toInt();
		return;
	}
}*/

// }
/// ***************************************************************************************************//{

/*void MapFrame::selectObject()
{
itemSelected = true;
 QGraphicsItem *it;
 foreach(it, scene()->items())
{
	if(it->data(Name).toString() == objectName)
	{
		activeItem = it;
		emit objectSelected(activeItem);
		return;
	}
}


// for(QList<ObjectGraphicsItem*>::iterator it = ogilist.begin(); it != ogilist.end(); ++it)
// {
// 	if((*it)->data(Name).toString() == objectName)
// 	{
// 		activeItem = *it;
// 	}
// }


}*/
/// ***************************************************************************************************//}



void MapFrame::setMapSize(const QSize &a_newSize)
{
  m_smap.setSize(a_newSize);
}




void MapFrame::setXPos(int xpos)
{
qWarning() << "MapFrame::setXPos(int xpos)" << xpos;
m_mousepos.setX(xpos);
}

void MapFrame::setYPos(int ypos)
{
qWarning() << "MapFrame::setYPos(int ypos)" << ypos;
m_mousepos.setY(ypos);
}

/// void MapFrame::setToolTipString(QString ttstring)
// {
/*qWarning() << "setToolTipString(QString ttstring)" << ttstring;
object_tooltip = ttstring;*/
// }


void MapFrame::newObject(const MapObject &a_newObject)
{
qWarning() << "MapFrame::newObject(const MapObject &a_newObject)";

}

void MapFrame::removeCurrentItem()
{
  scene()->removeItem(m_currentGraphicsItem);
  delete m_currentGraphicsItem;
  m_currentGraphicsItem = 0;

  m_smap.removeObject(m_currentMapObject->id());
  m_currentMapObject = 0;
  
}


void MapFrame::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Delete)
  {
  emit SIG_deleteObject();
  }
  if(event->key() == Qt::Key_O)
  {
    if(m_currentGraphicsItem != 0)
    {
      m_currentGraphicsItem->resetTransform();
      m_currentGraphicsItem->setData(98, QVariant(0));
      m_currentGraphicsItem->setData(99, QVariant(0));
      m_currentGraphicsItem->setData(100, QVariant(1));
    }
    m_rotateXAngle = 0;
    m_rotateYAngle = 0;
    m_translationD = 1;
    resetTransform();
    setViewTransform();
  }
  if(event->key() == Qt::Key_C)
  {
    qWarning() << "Anzahl Mapobjekte" << m_smap.objects().count();
  }
}

void MapFrame::setViewTransform()
{
  
  QTransform f_t;
  f_t.translate(horizontalScrollBar()->value() + width()/2,   verticalScrollBar()->value() + height()/2);
  
//      f_t.rotate(90, Qt::XAxis);
     f_t.scale(m_translationD, m_translationD);
//      f_t.rotate(-90, Qt::XAxis);
  
  f_t.rotate(m_rotateXAngle, Qt::XAxis);
  f_t.rotate(m_rotateYAngle, Qt::YAxis);
  f_t.translate( - (horizontalScrollBar()->value() + width()/2), - (verticalScrollBar()->value() + height()/2));
//   setTransform(f_t);
}


void MapFrame::wheelEvent ( QWheelEvent * event )
{
  if(m_ChangePerspective)
  {
    m_translationD += ((qreal)(event->delta())) / 2000;
    setViewTransform();
  }
  else
  {
    if(m_itemSelected)
    {
      //   int tx = event->x() + horizontalScrollBar()->value();
      //   int ty = event->y() + verticalScrollBar()->value();
      QPointF f_pt = mapToScene(event->pos());
      //    QGraphicsPixmapItem *edititem = qgraphicsitem_cast<QGraphicsPixmapItem *>(scene()->itemAt(x,y));
      QGraphicsItem *edititem = (scene()->itemAt(f_pt));
      
      if(edititem != 0)
      {
	//      edititem->data(99).toDouble;
	//      int angle = edititem->data(99).toDouble() + event->delta()/50;
	//     qWarning() << edititem->boundingRect() 	;
	
	edititem->resetTransform();
	int sx = f_pt.x() - edititem->x();
	//     const int sx2 = edititem->boundingRect().width()/2;
	int sy = f_pt.y() - edititem->y();
	//     const int sy2 = edititem->boundingRect().height()/2;
	
	if(sx < 5 && sy < 5)
	{
	  sx = 0;
	  sy = 0;
	}
	QTransform t;
	t.translate( sx, sy);
	
	double xangle = edititem->data(98).toDouble();
	double yangle = edititem->data(99).toDouble();
	double scale = edititem->data(100).toDouble();
	if(scale == 0)
	{
	  scale = 1;
	}
	
	
	if(event->modifiers() == Qt::ShiftModifier)
	{
	  yangle += event->delta()/50;
	  edititem->setData(99, QVariant(yangle));
	}
	else if(event->modifiers() == Qt::ControlModifier)
	{
	  scale += scale * event->delta()/500;
	  edititem->setData(100, QVariant(scale));
	  qWarning() << scale;
	}
	else
	{
	  xangle += event->delta()/50;
	  edititem->setData(98, QVariant(xangle));
	  
	}
	if(scale > 0)
	{
	  t.scale(scale, scale);
	}
	
	t.rotate(xangle, Qt::XAxis);
	t.rotate(yangle, Qt::YAxis);
	
	t.translate( - sx, - sy);
	
	edititem->setTransform(t);
	//      edititem->setData(99, QVariant(angle));
      }
    }
    else
    {
      switch(event->modifiers())
      {
	case Qt::ShiftModifier:
	{
	  // 	  horizontalScrollBar()->triggerAction(QAbstractSlider::SliderSingleStepAdd);
	  horizontalScrollBar()->setValue(horizontalScrollBar()->value() - event->delta()/2);
	  break;
	}
	case Qt::ControlModifier:
	{
	  QTransform t;
	  // 	  t.scale
	  m_scaleF += qreal(event->delta()) * m_scaleF / 500;
	  t.scale(m_scaleF, m_scaleF);
	  setTransform(t);
// 	  scale(event->delta()/1000, event->delta()/1000);
	  break;
	}
	case Qt::NoModifier:
	{
	    verticalScrollBar()->setValue(verticalScrollBar()->value() - event->delta()/2);
	}
	default:
	  break;
      }
    }
  }
  
}


bool MapFrame::changePerspectiveEnabled() const
{
  return m_ChangePerspective;
}

bool MapFrame::changePerspective()
{
  m_ChangePerspective = !m_ChangePerspective;
  return m_ChangePerspective;
}



void MapFrame::mouseDoubleClickEvent(QMouseEvent *event)
{
  objectDialog(event->pos());
}
