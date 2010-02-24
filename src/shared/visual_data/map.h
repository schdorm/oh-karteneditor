/***************************************************************************
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

#ifndef _MAPCLASS_H
#define _MAPCLASS_H

#include "definitions.h"

#include "mapobject.h"
#include <QtCore/QList>
#include <QtCore/QSize>
#include <QtCore/QPoint>

class AbstractCity;
#include "abstractcity.h"

class Map
{
  public:
    Map();
    //     Map(const Map &);
    Map(const Map *);		// Pointer - Copy-Constructor
    //     ~Map();
    
    //   enum MapType
    //   {
      //     
      //     Sea		= 10000,
      //     Coast	= 20000,
      //     Land	= 40000,
      //     Citymap	= 10001
    //   };
    enum Type
    {
      Sea		= 10000,
      Coast	= 12000,
      Land	= 16000,
      Citymap	= 20000
    };
    
    enum Orientations
    {
      null	= 0,
      North	= 128,
      East	= 129,
      South	= 130,
      West	= 132
    };
    
    // 	void loadStartMap(const QString &);
    bool load(const QString &);
    
    #ifdef OH_GAME
    bool load(Orientations);
    #endif
    #ifdef OH_MAPEDITOR
    void save(const QString & a_filename = QString());
    #endif
    
    // const City *ccity()	const	{	return m_city;		}
    bool isCity	()	const;
    
    // {	return (m_type == Map::Coast ^ Map::Citymap || m2type == Map::Land ^ Map::Citymap);	}
    // City city() const;
    // City city	()	const	{	return m_city;			}
    AbstractCity *city	()		{	return &m_city;			}
    const AbstractCity *city()	const	{	return &m_city;			}
    
    void setCity(const AbstractCity &newCity);
    // const QString &cityname()	const	{	return m_cityname;	}
    
    
    void addObject			(const MapObject *);
    void removeObject	(int);
    
    QList <MapObject> objects 		() const	{	return m_Objects.values();	}
    MapObject * object			(int a_id)	{	return &m_Objects[a_id];	}
    MapObject object			(int a_id) const{	return m_Objects.value(a_id);	}
    
    //     const MapObject *object		(int a_id) const{	return &m_Objects[a_id];	}
    //     QList <MapObject> *objects ()		{	return &m_ObjectList;	}
    //     const QList <MapObject> &objectList	() const	{	return m_ObjectLists		}
    
    
    QString filename 	()	const	{	return m_filename;	}
    QString name	() 	const 	{	return m_name;		}
    QPoint coordinates	()	const	{	return m_coordinates;	}
    QSize size		()	const	{	return m_size;		}
    QString background	()	const	{	return m_background;	}
    
    QString mapnorth	()	const	{	return m_mapnorth;	}
    QString mapeast	()	const	{	return m_mapeast;	}
    QString mapsouth	()	const	{	return m_mapsouth;	}
    QString mapwest	()	const	{	return m_mapwest;	} 
    
    int type		()	const	{	return m_type;		}
    
    
    void setCoordinates	(const QPoint &);
    void setSize		(const QSize &);
    void setBackground	(const QString &);
    void setName(const QString&);
    
    void setMapnorth	(const QString &);
    void setMapsouth	(const QString &);
    void setMapwest		(const QString &);
    void setMapeast		(const QString &);
    void setType	(int);
    
  private:
    //   City *cityinstance();
    // static int mapcounter;
    
    // QString m_mapdirectory;
    
  protected:
    
    QString m_filename;
    QString m_name;
    QPoint m_coordinates;
    QSize m_size;
    QString m_background;
    QString m_mapnorth;
    QString m_mapeast;
    QString m_mapsouth;
    QString m_mapwest;
    int m_type;
    AbstractCity m_city;
    
    QList <MapObject> m_ObjectList;
    QHash <int, MapObject> m_Objects;
    
};

Map &operator<=(Map &m1, const Map &m2);

#endif
