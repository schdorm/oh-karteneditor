/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel                              *
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

#ifndef _City_H
#define _City_H

#include <QtCore/QString>

  #include "goods.h"

class QXmlStreamAttributes;

class City /*: public QObject*/
{
// Q_OBJECT
public:

enum
{
null = 0,
NoProduction = 0,
LowProduction = 1,		// 3		-- 186,67% of low production needed for normal demand
NormalProduction = 2,		// 7		-- goods demand requires 80% of normal Production	
HighProduction = 3,		// 10		-- 56 % of high production needed for normal demand
};


//#CityClass(int = -1, const QString &, const QList<int> &,  const QList<int> &, const QList<int> &);
// City( const QString &, const QList<int> &,  const QList<int> &, const QList<int> &, int = -1);

City( const QString & = 0, int = -1);
City(const City &);

// CityClass(const QString&, int [const_warenanzahl], int [const_warenanzahl], int [const_warenanzahl]);

// CityClass(const QString&, const int &param_hproduction[const_warenanzahl], const int &param_mproduction[const_warenanzahl], const int &param_lproduction[const_warenanzahl])

void init();


void setID		(const QList<int>&);
void setID		(int id = -1);
void setName		(const QString &);
void setProduction	(int, const QString&);
//QString ;

int id 			() const	{	return m_id;		}
const QString &name	() const	{	return m_name;		}

int inhabitants		() const	{	return m_inhabitants;	}

const Goods &production	() const	{	return m_production;	}	// returns production-things 


QXmlStreamAttributes characteristics () const;
void setCharacteristics (const QXmlStreamAttributes &);

private:
  int m_id;
  QString m_name;
  int m_inhabitants;
  Goods m_production;

};


#endif
