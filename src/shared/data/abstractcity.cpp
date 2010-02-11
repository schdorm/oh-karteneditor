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

#include "abstractcity.h"
#include <QtDebug>
#include "qstring.h"


#ifdef DEBUG_CITY
 int AbstractCity::citycounter = 0;


AbstractCity::AbstractCity(const AbstractCity &other) : m_name (other.name()), m_production(other.production()), m_inhabitants (other.inhabitants())
{
    citycounter ++;
    qWarning() << "AbstractCity counter:" <<citycounter;
}

AbstractCity::~AbstractCity()
{
  static int citydestrkt;
  citydestrkt ++;
  citycounter --;
  qWarning() << "AbstractCity destruktor" << citydestrkt << " Diff:" <<citycounter;
}
#endif




AbstractCity::AbstractCity(const QString &param_cityname) : m_name (param_cityname)
{
  #ifdef DEBUG_CITY
   static int cc;
   cc++;
   citycounter ++;
   qWarning() << "Count of city instances:" << citycounter << "all:" << cc;
  #endif
}

/*City::City(const City *other) : m_id (other->id()),
//  m_name (other->nameref()),
 m_inhabitants (other->inhabitants())//,
//   m_production (other->production())
{
  qWarning() << "Before setting name";
   m_name = other->name();
 m_production = other->production();
  qWarning() << "City successfull copied!";
qWarning() << other->name();
qWarning() << m_name;

}*/

void AbstractCity::init()
{
  m_inhabitants = 1300 + qrand()%1000;				//das kommt mal noch ins XML-Zeugs rein
}

void AbstractCity::setProduction(int quality, const QString &param_listing)		// quality = Low Production / Medium / High .....
{
  QString listing = removeWithespace(param_listing);		// removes any whitespace --> "qstring.h"
  QString goodID;
  
  #ifdef DEBUG_CITY
  qWarning() << "void AbstractCity::setProduction(int quality, const QString &param_listing)" << listing << listing.size();
  #endif

  		// iterating through string; when reaching "," --> production of that GOODID = quality
		
  for(int i = 0; i < listing.size(); i ++)
  {
    if(listing[i] == QChar(','))
    {
      #ifdef DEBUG_CITY
      qWarning() << "Set Production: " << goodID.toInt() << quality;
      #endif
      m_production[goodID.toInt()] = quality;
      goodID = QString();
    }
    else
      goodID.append(listing[i]);
  }
}

void AbstractCity::setName(const QString &name)
{
  #ifdef DEBUG_CITY
    qWarning() << "set Name" << name;
    qWarning() << "m_name" << m_name;
  #endif
  
  m_name = name;
  
  #ifdef DEBUG_CITY
    qWarning() << "Name set" << m_name;
  #endif
}


// void AbstractCity::setID(const QList<int> &idlist)
// {
// 	int i = 0;
// 	while(m_id < 0)
// 	{
// 		i++;
// 		if(!idlist.contains(i))
// 		{
// 		m_id = i;
// 		}
// 	}
// }
// 
// void AbstractCity::setID(int id)
// {
//   m_id = id;
// }
