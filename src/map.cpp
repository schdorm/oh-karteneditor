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

#include "map.h"
#include "mapobject.h"
#include "settings.h"
#include "city.h"




#include <QtDebug>
#include <QtCore/QXmlStreamReader>
#include <QtCore/QFileInfo>
#include <QtCore/QFile>
#include <QtCore/QDir>
#include <QtCore/QVariant>

MapObject::MapObject(const int &param_role,
		     const QString &param_filename,
		     const QString &param_tooltip,
		     const QPoint &param_position,
		     const double &param_zvalue) :
		     m_role (param_role),
		     m_filename (param_filename),
		     m_tooltip (param_tooltip),
		     m_position (param_position),
		     m_zValue (param_zvalue)
{
// 	m_role = param_role;
// 	m_filename = param_filename;
// 	m_tooltip = param_tooltip;
// 	m_position = param_position;
// 	m_zValue = param_zvalue;
}


// QString Map::mapdirectory;

Map::Map() : m_filename (QString()),
	     m_mapnorth (QString()),
	     m_mapeast (QString()),
	     m_mapsouth (QString()),
	     m_mapwest(QString())

{	     m_city = 0;
	     m_type = Sea;
// m_mapdirectory = SETTINGS->mapdirectory();
// 	if(mapdirectory.isEmpty())			// static QString in Map-Class
// 	{
// 		QDir dir= QDir().current();
// 		if(dir.cd("maps"))
// 		{
// 			mapdirectory = dir.absolutePath().append("/");
// 			qWarning()  << "Working Direktory:" << mapdirectory;
// 			
// 		}
// 		else
// 		{
// 			qWarning() << "Mapdir does not exist. Exiting.";
// 			mapdirectory = QString();
// 		}
// 	}
// 	m_filename = QString();
// 	m_mapnorth = QString();
// 	m_mapeast  = QString();
// 	m_mapsouth = QString();
// 	m_mapwest  = QString();
// 	m_city = 0;
}

Map::Map(Map *other) :
  m_filename (other->filename()),
  m_coordinates (other->coordinates()),
  m_size (other->size()),
  m_city (other->city()),
  m_background (other->background()),
  m_mapnorth (other->mapnorth()),
  m_mapeast (other->mapeast()),
  m_mapsouth (other->mapsouth()),
  m_mapwest (other->mapwest()),
  m_type (other->type()),
  m_ObjectList (other->objectList())
{
  
}

Map::~Map()
{
#ifdef _DEBUG_DESTRUCTORS
qWarning() << "Destructing Map";
#endif
MapObject *mapobjectit = 0;	//Map-Object-Iterator
foreach(mapobjectit, m_ObjectList)
{
delete mapobjectit;
}
#ifdef _DEBUG_DESTRUCTORS
qWarning() << "End of Destructing Map";
#endif
delete m_city;
}

void Map::setCity(City *newCity)
{
  delete m_city;
  m_city = newCity;
}



/*void Map::loadStartMap(const QString &mapfilename)
{
static bool started;
if(!started)
{
started = loadMap(mapfilename);
m_coordinates = QPoint(400,400);
}
else if(mapfilename == "false")
{
started = false;
}

}*/

bool Map::loadMap(Orientations mapdirection)
{
	switch(mapdirection)
	{
		case North:
		{
			if(loadMap(m_mapnorth))
			{
				m_coordinates.setY(m_coordinates.y() + 1);
				return true;
			}
			break;
		}
		case East:
		{
			if(loadMap(m_mapeast))
			{
				m_coordinates.setX(m_coordinates.x() + 1);
				return true;
			}
			break;
		}
		case South:
		{
			if(loadMap(m_mapsouth))
			{
				m_coordinates.setY(m_coordinates.y() - 1);
				return true;
			}
			break;
		}
		case West:
		{
			if(loadMap(m_mapwest))
			{
				m_coordinates.setX(m_coordinates.x() - 1);
				return true;
			}
			break;
		}
		default:
			break;
	}
	return false;
}


bool Map::loadMap(QString param_mapname)
{
	qWarning() << "bool Map::loadMap(QString param_mapname)" << "Karteladen: " << param_mapname;
QString mapdirectory = QFileInfo(param_mapname).absoluteDir().absolutePath();

	QFile file(param_mapname);		//Map-XML-Lesen
	if(file.exists())
	{
		m_filename = param_mapname;
		qWarning() << "Mapfile is existing";
		
		enum stati	{
				e_null,			// nothing
				e_object_null,			// Objectproperties
				e_object_role,			// Object-role
				e_object_tooltip,		// Object-Tooltip
				e_object_file,			// Image-File
				e_object_posx,			// X-Position
				e_object_posy,			// Y-Position
				e_object_zvalue,		// Z-Value
				e_map_null,			// Allgemein: Mapeigenschaften ///Mapproperties
				e_map_cityname,		// cityname
				e_map_img,			// mapbackground
				e_map_north,			//noerdliche angrenzende Map	mapnorth
				e_map_west,			//westliche a M			mapwest
				e_map_south,			//suedliche a M			mapsouth
				e_map_east,			//oestliche a M			mapeast
				e_map_width,		//breite			width
				e_map_height,		//hoehe				height
				e_map_typ,
				} status = e_null;	// status als dieses enum: zeigt an, was fuer ein Wert als naechstes ausgelesen wird
				
				// 		QString cityname = QString();
				// 		int id;
				
				bool f_isCity = false;

		int object_role = -1;				//Funktion des Objektes /// objectrole
		QString object_tooltip = QString();				//name/tooltip des objekts
		QString object_file = QString();				//name des Bildes des Objekts

		int object_posx = 0;				//x-Koordinate /// x-position
		int object_posy = 0;				//y-Koordinate /// y-position
		double object_zvalue = 0;			// z-Value = Hoehe /// Z-Value
		bool reading = true;

		
		file.open(QIODevice::ReadOnly);
		QXmlStreamReader reader(&file);
		while (reading) 
		{
		switch(reader.readNext())
		{
			case QXmlStreamReader::StartElement:
			{
			QString qualName = reader.name().toString();
			qWarning() << "\nStart:\t" << reader.name().toString();
				if(reader.name().toString() =="city")
				{
 				  f_isCity = true;
				  m_city = new City(reader.attributes().value("name").toString(), reader.attributes().value("id").toString().toInt());
// 				  cityname = reader.attributes().value("name").toString();
// 				  id = reader.attributes().value("id").toString().toInt();
// 				  m_city->setName(cityname);
// 				  m_city->setID(id);
// 				  status = e_map_cityname;
				  break;
				}
				else if(reader.name().toString() == "production" && f_isCity)
				{
// 				  QString lowp = reader.attributes().value("low").toString();
// 				  QString nmlp = reader.attributes().value("normal").toString();
// 				  QString highp = reader.attributes().value("high").toString();
				  m_city->setProduction(City::LowProduction, reader.attributes().value("low").toString());
				  m_city->setProduction(City::NormalProduction, reader.attributes().value("normal").toString());
				  m_city->setProduction(City::HighProduction, reader.attributes().value("high").toString());
				  break;
				}
				else if(reader.name().toString() =="map")
				{
// 				qWarning() << "Start: gamedata->currentMap";
// 				status = e_map_img;

				break;
				}
				else if(reader.name().toString() =="mapnorth")
				{
// 				qWarning() << "Start: gamedata->currentMap";
				status = e_map_north;
				break;
				}
				else if(reader.name().toString() =="mapeast")
				{
// 				qWarning() << "Start: gamedata->currentMap";
				status = e_map_east;
				break;
				}
				else if(reader.name().toString() =="mapsouth")
				{
// 				qWarning() << "Start: gamedata->currentMap";
				status = e_map_south;
				break;
				}
				else if(reader.name().toString() =="mapwest")
				{
// 				qWarning() << "Start: gamedata->currentMap";
				status = e_map_west;
				break;
				}
				else if(reader.name().toString() =="mapwidth")
				{
// 				qWarning() << "Start: gamedata->currentMap";
				status = e_map_width;
				break;
				}
				else if(reader.name().toString() =="mapheight")
				{
// 				qWarning() << "Start: gamedata->currentMap";
				status = e_map_height;
				break;
				}

				else if(reader.name().toString() =="maptype")
				{
// 				qWarning() << "Start: gamedata->currentMap";
				status = e_map_typ;
				break;
				}

				else if(reader.name().toString() =="object")
				{
// 				qWarning() << "object";
				status = e_object_null;
				break;
				}

				else if(reader.name().toString() == "objectfunktion")
				{
// 				qWarning() << "o_role";
				status = e_object_role;
				break;
				}

				else if(reader.name().toString() == "objecttooltip")
				{
// 				qWarning() << "o_tooltip";
				status = e_object_tooltip;
				break;
				}

				else if(reader.name().toString() == "objectdatei")
				{
// 				qWarning() << "o_file";
				status = e_object_file;
				break;
				}

				else if(reader.name().toString() == "objectpositionx")
				{
// 				qWarning() << "o_posx";
				status = e_object_posx;
				break;
				}

				else if(reader.name().toString() == "objectpositiony")
				{
// 				qWarning() << "o_posy";
				status = e_object_posy;
				break;
				}
				
				else if(reader.name().toString() == "objecthoehe")
				{
// 				qWarning() << "o_posy";
				status = e_object_zvalue;
				break;
				}
				else
				{
				status = e_null;
				}
				break;
			}

			case QXmlStreamReader::Invalid:
			{
			qWarning() << "Error:" << reader.errorString() <<"\nEnde Error" ;
			break;
			}

			case QXmlStreamReader::Characters:
			{
 				if(status != e_null)
				{qWarning() << "Chars:" <<reader.text().toString();}
				//Tags ohne Inhalt - nur mit Unterkategorien
				switch(status)
				{
				case e_map_null:
					break;
// 				case e_map_cityname:
// 				{
// 					m_cityname = reader.text().toString();
// 					m_isCity = true;
// // 					gamedata->setCurrentCity(reader.text().toString());
// 				break;
// 				}
				case e_map_img:
				{
					m_background = reader.text().toString();
					m_background.prepend(mapdirectory);
				break;
				}

				case e_map_north:
				{
					m_mapnorth = reader.text().toString();

					qWarning() << "Nord-Map:" << m_mapnorth;
				if(!QFile(m_mapnorth).exists())
					{
					qWarning() << m_mapnorth << "Existiert nicht";
					m_mapnorth = QString();
					}
				break;
				}

				case e_map_east:
				{
				m_mapeast = reader.text().toString();
				qWarning() << "Ost-Map:" << m_mapeast;
				if(!QFile(m_mapeast).exists())
					{
					qWarning() << m_mapeast << "Existiert nicht";
					m_mapeast = QString();
					}
				break;
				}

				case e_map_south:
				{
				m_mapsouth = reader.text().toString();
				qWarning() << "Sued-Map:" << m_mapsouth;
				if(!QFile(m_mapsouth).exists())
					{
					qWarning() << m_mapsouth << "Existiert nicht";
					m_mapsouth = QString();
					}
				break;
				}

				case e_map_west:
				{
				m_mapwest = reader.text().toString();
				qWarning() << "West-Map:" << m_mapwest;
				if(!QFile(m_mapwest).exists())
					{
					qWarning() << m_mapwest << "Existiert nicht";
					m_mapwest = QString();
					}
				break;
				}

				case e_map_width:
				{
					m_size.setWidth(reader.text().toString().toInt());
					break;
				}
				case e_map_height:
				{
					m_size.setHeight(reader.text().toString().toInt());
					break;
				}
				case e_map_typ:
				{
					m_type =
					  static_cast<Map::MapType>( reader.text().toString().toInt() );
					break;
				}

				case e_object_null:
				{
// 					status = e_object_null;
					break;
				}
				case e_object_role:
				{
					object_role = reader.text().toString().toInt();
					qWarning() << "\tObjektfkt" << object_role;
					break;
				}
				case e_object_tooltip:
				{
					object_tooltip = reader.text().toString();
					qWarning() << "\tObjekttooltip" << object_tooltip;
					break;
				}
				case e_object_file:
				{
					object_file = reader.text().toString();
					object_file.prepend(mapdirectory);
					qWarning() << "\tBild:" << object_file;
					break;
				}
				case e_object_posx:
				{
					object_posx = reader.text().toString().toInt();
					qWarning() << "\tPosX:" << object_posx;
					break;
				}
				case e_object_posy:
				{
					object_posy = reader.text().toString().toInt();
					qWarning() << "\tPosY" << object_posy;
					break;
				}
				case e_object_zvalue:
				{
					object_zvalue = reader.text().toString().toDouble();
					qWarning() << "\tPosY" << object_zvalue;
					break;
				}
				
				default:
					break;
				}
				break;
			}
			case QXmlStreamReader::EndElement:
			{
				qWarning() << "Ende :"<< reader.name().toString();
				if(reader.name().toString() == "object" && object_role != - 1 && !object_file.isEmpty())
	//jetzt zeichnen: habe alle Eigenschaften des Objektes erhalten?
				{
					qWarning() << object_file << object_role;
					
					MapObject *currentMO = new MapObject(object_role, object_file, object_tooltip, QPoint(object_posx, object_posy), object_zvalue);
					
					m_ObjectList << currentMO;
					
					object_role = -1;
					object_tooltip = QString();
					object_file = QString();
					object_posx = 0;
					object_posy = 0;
					object_zvalue = 0;
				}
				status = e_null;
				break;

			}
			default:
				break;

		}
		if(reader.atEnd() || (reader.tokenType() == QXmlStreamReader::EndElement && reader.name().toString() =="map"))
		{
		reading = false;
		}

		}
	
		if (reader.hasError())
		{
			qWarning() << reader.errorString();
		}
		qWarning() << "Returning true: Map sucessfully read!";
		return true;
	}
	else
	{
	qWarning() << "Mapfile not found!";
	return false;
	}
}


// void Map::setFilename	(const QString &nfn)
// {
//   m_filename = nfn;
// }
void Map::setCoordinates	(const QPoint &nc)
{
  m_coordinates = nc;
}
void Map::setSize		(const QSize &ns)
{
  m_size = ns;
}
void Map::setBackground	(const QString &nbg)
{
  m_background = nbg;
}

void Map::setMapnorth	(const QString &nmn)
{
  m_mapnorth = nmn;
}
void Map::setMapsouth	(const QString &nms)
{
  m_mapsouth = nms;
}
void Map::setMapwest		(const QString &nmw)
{
  m_mapwest = nmw;
}
void Map::setMapeast		(const QString &nme)
{
  m_mapeast = nme;
}

void Map::setType(int a_type)
{
  m_type = a_type;
}

void Map::saveMap(const QString &a_file)
{
  m_filename = a_file;
}
