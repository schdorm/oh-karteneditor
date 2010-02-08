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


#include "mapobject.h"
#include <QtCore/QList>
#include <QtCore/QSize>
#include <QtCore/QPoint>

class City;

class Map
{
  public:
    Map();
    Map(Map *);		// Pointer - Copy-Constructor
    ~Map();

  enum MapType
  {
    Sea		= 10000,
    Coast	= 20000,
    Land	= 40000,
    Citymap	= 10001
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
	bool loadMap(QString);

	bool loadMap(Orientations);
	
	void saveMap(const QString &);
	

const City *city()	const	{	return m_city;		}
City *city	()		{	return m_city;		}
void setCity(City *newCity = 0);
// const QString &cityname()	const	{	return m_cityname;	}


const QList <MapObject*> &objectList() const	{return m_ObjectList;	}

QString filename 	()	const	{	return m_filename;	}
QPoint coordinates	()	const	{	return m_coordinates;	}
QSize size		()	const	{	return m_size;		}
QString background	()	const	{	return m_background;	}

QString mapnorth	()	const	{	return m_mapnorth;	}
QString mapeast		()	const	{	return m_mapeast;	}
QString mapsouth	()	const	{	return m_mapsouth;	}
QString mapwest		()	const	{	return m_mapwest;	} 

int type		()	const	{	return m_type;		}


void setCoordinates	(const QPoint &);
void setSize		(const QSize &);
void setBackground	(const QString &);

void setMapnorth	(const QString &);
void setMapsouth	(const QString &);
void setMapwest		(const QString &);
void setMapeast		(const QString &);
void setType	(int);

private:


// QString m_mapdirectory;

protected:

QString m_filename;
QPoint m_coordinates;
QSize m_size;
City *m_city;
QString m_background;
QString m_mapnorth;
QString m_mapeast;
QString m_mapsouth;
QString m_mapwest;
int m_type;

	QList <MapObject*> m_ObjectList;

};

#endif
