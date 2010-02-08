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

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QXmlStreamReader>

#include <QtDebug>

#include "settings.h"
#include "map.h"

Settings::Settings()
{
  m_settingsfilepath = QDir().home().absolutePath() + "/.OpenHanse/mesettings.ohc";
  QString townhalllabel = tr("Rathaus"),
  marketlabel = tr("Markt"),
  churchlabel = tr("Kirche"),
  portlabel = tr("Hafen"),
  officelabel = tr("Kontor"),
  banklabel = tr("Darlehensgeber"),
  tavernlabel = tr("Kneipe"),
  land_breakelabel = tr("Landflaechen"),
  land_damagelabel = tr("\"boese\" Untiefen"), // Objekte, bei denen eine Kollision mit dem Schiff Schaden am Schiff verursacht.
  mapdecorationlabel = tr("Mapdeko");

  m_ObjectFunctionList.insert(Building::Townhall, townhalllabel);
  m_ObjectFunctionList.insert(Building::Market, marketlabel);
  m_ObjectFunctionList.insert(Building::Church, churchlabel);
  m_ObjectFunctionList.insert(Building::Port, portlabel);
  m_ObjectFunctionList.insert(Building::Kontor, officelabel);
  m_ObjectFunctionList.insert(Building::Bank, banklabel);
  m_ObjectFunctionList.insert(Building::Tavern, tavernlabel);
  m_ObjectFunctionList.insert(Land_breake, land_breakelabel);
  m_ObjectFunctionList.insert(Land_damage, land_damagelabel);
  m_ObjectFunctionList.insert(Mapdecoration, mapdecorationlabel);
  
  QString mt_sea = tr("See/Meer"),
  mt_coast = tr("Kueste"),
  mt_land = tr("Land"),
  mt_city = tr("(Stadt)");
  
  m_MapTypeList.insert(Map::Sea, mt_sea);
  m_MapTypeList.insert(Map::Coast, mt_coast);
  m_MapTypeList.insert(Map::Land, mt_land);
//   m_MapTypeList.insert(Map::Citymap, mt_city);
  readSettings();
  
}

Settings *Settings::instance()
{
  if(m_pointer == NULL )
  {
    m_pointer = new Settings();
  }
  return m_pointer;
  
}

QHash<int, QString> Settings::ObjectFunctions() const
{
  return m_ObjectFunctionList;
}

QHash<int, QString> Settings::MapTypes() const
{
    return m_MapTypeList;
}

QString Settings::settingsfilepath() const
{
  return m_settingsfilepath;
}

bool Settings::oldlayout() const
{
  return m_oldlayout;
}

void Settings::readSettings()
{
  bool ol = m_oldlayout;
  qWarning() << "Settingsfilepath:" << settingsfilepath();

  QFile settingsfile(m_settingsfilepath);
  settingsfile.open(QIODevice::ReadOnly);
  QXmlStreamReader reader(&settingsfile);
  while(!reader.atEnd())
  {
    switch(reader.readNext())
    {
      case QXmlStreamReader::StartElement:
      {
	if(reader.name().toString() == "layout")
	{
	  if(reader.attributes().value("old") == QString("true"))
	    m_oldlayout = true;
	  else
	    m_oldlayout = false;
	  
	}
	break;
      }
      case QXmlStreamReader::EndElement:
	break;
      default:
	break;
    }
  }
  settingsfile.close();
  
  if(ol != m_oldlayout)
  {
    emit changed();
  }
}

bool operator==(const Settings &s1, const Settings &s2)
{
  return s1.oldlayout() == s2.oldlayout();
}

bool operator!=(const Settings &s1, const Settings &s2)
{
  return !(s1 == s2);
}
