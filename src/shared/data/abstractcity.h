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

#ifndef _AbstractCity_H
#define _AbstractCity_H

#include <QtCore/QString>

#include "goods.h"
#include "oh.h"
//   #define DEBUG_CITY


class QXmlStreamAttributes;

class AbstractCity /*: public QObject*/
{
  // Q_OBJECT
  public:
    #ifdef DEBUG_CITY
    AbstractCity(const AbstractCity &other);
    ~AbstractCity();
    #endif
    
    AbstractCity( const QString & = 0);
    
    
    void init();
    

    void setName		(const QString &);
    
    void setProduction	(int, const QString&);
    
    
    
    int id 			() const	{	return m_id;		}
    QString name		() const	{	return m_name;		}
    // const QString &nameref	() const	{	return m_name;		}
    
    int inhabitants		() const	{	return m_inhabitants;	}
    
    // returns production-things 
    const Goods &production	() const	{	return m_production;		}
    Goods &production 		() 		{	return m_production;		}

    #ifdef OH_MAPEDITOR
    #if OH_MAPEDITOR < 0x0302
    QString lowproduction	() const 	{	return m_lowproduction;		}
    QString normalproduction	() const 	{	return m_normalproduction;	}
    QString highproduction	() const 	{	return m_highproduction;	}
    #endif
    #endif
    
    QXmlStreamAttributes characteristics () const;
    void setCharacteristics (const QXmlStreamAttributes &);
    
  private:
    int m_id;
    QString m_name;
    int m_inhabitants;
    Goods m_production;
    
    #ifdef OH_MAPEDITOR
    #if OH_MAPEDITOR < 0x0302
    QString m_lowproduction, m_normalproduction, m_highproduction;
    #endif
    #endif
    
    #ifdef DEBUG_CITY
    static int citycounter;
    #endif
    
};


#endif

