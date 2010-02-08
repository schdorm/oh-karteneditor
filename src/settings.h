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

#ifndef _SETTINGS_H
#define _SETTINGS_H

#include <QtCore/QObject>
#include <QtCore/QHash>

#define SETTINGS Settings::instance()

// enum MapType
// {
//   Sea	= 10000,
//   Coast	= 20000,
//   Land	= 40000,
//   City	= 10001
// };

enum 
{
   Land_breake = 6000,
   Land_damage = 6001,
   Mapdecoration = 6002
};
 
 namespace Building
 {
     	enum Buildingtypes
	{
		null = 0,
		Factory = 4000,			// Werkstaetten - workshops
		Kontor = 4901,
		Market = 4902,
		Port = 4903,
		Townhall = 4904,
		Church  = 4905,
		Bank = 4906,
		Tavern = 4907,
		namespacexyz,
		Buildinglot = 4999			// Baustelle
	};
/* 	enum Buildingtypes
	{
		null = 0,
		Factory = 4000,			// Werkstaetten - workshops
		Kontor = Factory + 1,
		Market = Factory + 2,
		Port = Factory + 3,
		Townhall = Factory + 4,
		Church  = Factory + 5,
		Bank = Factory + 6,
		Tavern = Factory + 7,
		namespacexyz,
		Buildinglot = 4999			// Baustelle
	};*/
 }

class Settings : public QObject
{
  Q_OBJECT
  public:
    static Settings * instance();
    QHash<int, QString> ObjectFunctions() const;
    QHash<int, QString> MapTypes() const;
    QString settingsfilepath() const;
    bool oldlayout() const;
    
  public slots:
    void readSettings();
    
  signals:
    void changed();
    
  private:
    Settings();
    static Settings *m_pointer;
    
    QHash<int, QString> m_ObjectFunctionList;
    QHash<int, QString> m_MapTypeList;
    bool m_oldlayout;
    QString m_settingsfilepath;
};

bool operator!=(const Settings &s1, const Settings &s2);
bool operator==(const Settings &s1, const Settings &s2);

#endif
