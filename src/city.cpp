/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel                              *
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

#include "city.h"
// #include <QtDebug>
#include "qstring.h"

	void City::init()
	{
	m_inhabitants = 1300 + qrand()%1000;				//das kommt mal noch ins XML-Zeugs rein


	}


City::City(const QString &param_cityname, int arg_id) : m_id(arg_id), m_name (param_cityname)
{
}




City::City(const City &other) :
 m_id (other.m_id),
 m_name (other.m_name),
 m_inhabitants (other.m_inhabitants),
 m_production (other.m_production)
{
  
}


void City::setProduction(int quality, const QString &param_listing)		// quality = Low Production / Medium / High .....
{
  QString listing = removeWithespace(param_listing);		// removes any whitespace --> "qstring.h"
  QString goodID;
  
  		// iterating through string; when reaching "," --> production of that GOODID = quality
		
  for(int i = 0; i < listing.size(); i ++)
  {
    if(listing[i] == QChar(','))
    {
      m_production[goodID.toInt()] = quality;
      goodID = QString();
    }
    else
      goodID.append(listing[i]);
  }
}

void City::setName(const QString &name)
{
  m_name = name;
}


void City::setID(const QList<int> &idlist)
{
	int i = 0;
	while(m_id < 0)
	{
		i++;
		if(!idlist.contains(i))
		{
		m_id = i;
		}
	}
}

void City::setID(int id)
{
  m_id = id;
}
