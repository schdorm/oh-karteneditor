/***************************************************************************
 *   Copyright (C) 2009 - 2010 by Christian Doerffel                       *
 *   schdorm@googlemail.com                                                *
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

#ifndef _MapObjectDialog_H
#define _MapObjectDialog_H

#include <QtGui/QDialog>
#include <QtCore/QHash>

#include "ui_mapobjectdialog.h"

#include "mapobject.h"

class Map;
class QAbstractButton;
class QGraphicsItem;

class MapObjectDialog : public QDialog
{
  Q_OBJECT
  public:
    MapObjectDialog( Map *, const QPoint &, const QList<QGraphicsItem*> &ObjectList = QList<QGraphicsItem*>(), QGraphicsItem *a_currentGraphicsItem = 0);
    
    enum
    {
//       NewObjectItem = -1
      NewObjectSelectionBoxItem = -2,
    };
    
    void getObjectData(const MapObject *a_mapobject);
    void setObjectData(MapObject *a_mapobject);
    MapObject *newMapObject()	{	return &m_newMapObject; };
    
  public slots:
    void setObjectFile();
    void changeItem();
    void buttonClick(QAbstractButton *);
    
  signals:
    void newMapObjectCreated(MapObject *);
//     void newMapObjectCreated();
    void objectDataChanged(int);
    void objectImageChanged(int);
    
  private:
    Ui::MapObjectDialog ui;
    QHash<int, MapObject*> m_MapObjects;
    MapObject m_newMapObject;
    
    bool m_lockItemData;
  protected:
    
};

#endif
