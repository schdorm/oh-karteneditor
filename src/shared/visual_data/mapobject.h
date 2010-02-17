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
#ifndef _MAPOBJECT_H
#define _MAPOBJECT_H
#include <QtCore/QPoint>
#include <QtCore/QString>

class MapObject
{
  public:
    MapObject(const int , const QString &, const QString &, const QPoint &, const qreal );
    MapObject(const int , const QString &, const QString &, const QString &, const qreal );
    
    enum 
    {
      Land_breake = 6000,
      Land_damage = 6001,
      Mapdecoration = 6002
    };
    
    
    
    int 	id	 () const 	{	return m_id;		}
    int		role 	 () const	{	return m_role;		}
    QString	filename () const	{	return m_filename;	}
    QString	tooltip  () const	{	return m_tooltip;	}
    QString	name	 () const 	{	return m_name;		}
    QPoint	position () const	{	return m_position;	}
    double	zValue	 () const	{	return m_zValue;	}
    
    void setRole 	(int);
    void setFilename	(const QString& );
    void setTooltip	(const QString& );
    void setName	( const QString&);
    void setPosition	(const QPoint &);
    void setZValue	(double);
    
    
//     void operator=(const MapObject &other);

  private:
    int m_id;
    int m_role;
    QString m_filename;
    QString m_tooltip;
    QString m_name;
    QPoint m_position;
    qreal m_zValue;
};

// void operator=(MapObject &m1, const MapObject &m2);

#endif
