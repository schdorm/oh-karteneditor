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
#include <QTimer>

#include "settings.h"
#include "map.h"
#include "qstring.h"

Settings::Settings()
{
  m_autosaveTimer = new QTimer(this);
  connect(m_autosaveTimer, SIGNAL(timeout()), this, SIGNAL(autosave()));
  
  m_autosavepath = QDir::home().absolutePath() + "/.OpenHanse/me_autosave_map.ohc";
  m_settingsfilepath = QDir::home().absolutePath() + "/.OpenHanse/mesettings.ohc";
  QDir dir = QDir::home();
  if(!dir.cd(".OpenHanse"))
  {
    if(dir.mkdir(".OpenHanse"))
    {
      dir.cd(".OpenHanse");
    }
    else
    {
      qFatal("Could not create OpenHanse settings folder");
    }
  }
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

QString Settings::autosavepath () const
{
  return m_autosavepath;
}

bool Settings::oldlayout() const
{
  return m_oldlayout;
}

bool Settings::autosaveEnabled() const
{
  return m_autosaveEnabled;
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
	if(reader.name().toString() == "oldlayout")
	{
	  m_oldlayout = toBool(reader.attributes().value("value"));
// 	  if(reader.attributes().value("old") == QString("true"))
// 	    m_oldlayout = true;
// 	  else
// 	    m_oldlayout = false;
	  
	}
	else if(reader.name().toString() == "autosave")
	{
	  m_autosaveEnabled = toBool(reader.attributes().value("value"));
// 	  if(reader.attributes().value("value") == QString("true"))
// 	    m_autosaveEnabled = true;
// 	  else
// 	    m_autosaveEnabled = false;
	  
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
  if(m_autosaveEnabled)
  {
    enableAutosave();
//     m_autosaveTimer->start(10000);
  }
  else
    disableAutosave();
//     m_autosaveTimer->stop();
}

void Settings::enableAutosave()
{
  m_autosaveTimer->start(10000);
}

void Settings::disableAutosave()
{
  m_autosaveTimer->stop();
}

bool operator==(const Settings &s1, const Settings &s2)
{
  return s1.oldlayout() == s2.oldlayout();
}

bool operator!=(const Settings &s1, const Settings &s2)
{
  return !(s1 == s2);
}
