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

#include "map.h"
#include "mapobject.h"
#include "settings.h"
#include "abstractcity.h"

#include "qstring.h"

// #define DEBUG_MAP

// int Map::mapcounter = 0;
int MapObject::MapObject_IDCounter;


#ifdef DEBUG_MAP
#include <QtDebug>
#endif

#include <QtCore/QVariant>

#include <QtCore/QDir>
#include <QtCore/QFile>
#include <QtCore/QFileInfo>

#include <QtCore/QXmlStreamReader>
#include <QtXml/QXmlStreamWriter>

///	MapOBJECT

MapObject::MapObject(int		a_role,
		     const QString	&a_filename,
		     const QPoint	&a_position,
		     const QString	&a_name,
		     const QString	&a_tooltip,
		     double 		a_zvalue) :
		     m_role (a_role),
		     m_filename	(a_filename),
		     m_position	(a_position),
		     m_name	(a_name),
		     m_tooltip (a_tooltip),
		     m_zValue (a_zvalue)
{
  MapObject_IDCounter++;
  m_id = MapObject_IDCounter;

// 	m_role = param_role;
// 	m_filename = param_filename;
// 	m_tooltip = param_tooltip;
// 	m_position = param_position;
// 	m_zValue = param_zvalue;

}

#ifdef OH_MAPEDITOR
MapObject::MapObject() : m_role (0), m_zValue(0)
{
  MapObject_IDCounter++;
  m_id = MapObject_IDCounter;
}
#endif

bool operator==(const MapObject &m1, const MapObject &m2)
{
//   m1.setRole(m2.role());
//   m1.setFilename(m2.filename());
//   m1.setTooltip(m2.tooltip());
//   m1.setName(m2.name());
//   m1.setPositon(m2.position());
//   m1.setZValue(m2.zValue());
  return ((m1.role() == m2.role()) && (m1.imagepath() == m2.imagepath()) && (m1.tooltip() == m2.tooltip()) && (m1.name() == m2.name()) && (m1.position() == m2.position()) && (m1.zValue() == m2.zValue()));
}

/*operator=(MapObject &m1, const MapObject &m2);
{
  m1.setRole(m2.role());
  m1.setFilename(m2.filename());
  m1.setTooltip(m2.tooltip());
  m1.setName(m2.name());
  m1.setPositon(m2.position());
  m1.setZValue(m2.zValue());
  return m1;
}*/
/*void MapObject::operator=(const MapObject &m2);
{
  setRole(m2.role());
  setFilename(m2.filename());
  setTooltip(m2.tooltip());
  setName(m2.name());
  setPositon(m2.position());
  setZValue(m2.zValue());
}*/

void MapObject::setRole (int a_role)
{
  m_role = a_role;
}

void MapObject::setFilename	(const QString &a_filename)
{
  m_filename = a_filename;
}

void MapObject::setImagepath	(const QString &a_path)
{
  m_filename = a_path;
}

void MapObject::setTooltip	(const QString &a_tooltip)
{
  m_tooltip = a_tooltip;
}

void MapObject::setName	( const QString &a_name)
{
  m_name = a_name;
}

void MapObject::setPosition(const QPointF &a_pos)
{
  m_position = a_pos;
}

void MapObject::moveBy (qreal a_xShift, qreal a_yShift)
{
  m_position.rx() += a_xShift;
  m_position.ry() += a_yShift;
}

void MapObject::setZValue (double a_zvalue)
{
  m_zValue = a_zvalue;
}


// QString MapObject::positionS() const
// {
//   return (QString::number(m_position.x()) + "|" + QString::number(m_position.y()));
// }


/*MapObject::MapObject(const int		a_role,
		     const QString	&a_filename,
		     const QString	&a_position,
		     const QString	&a_name,
		     const QString &a_tooltip,
		     const qreal a_zvalue) :
		     m_role (a_role),
		     m_filename (a_filename),
		     m_position(toPoint(a_position)),
		     m_name (a_name),
		     m_tooltip (a_tooltip),
		     m_zValue (a_zvalue)
{
  MapObject_IDCounter++;
  m_id = MapObject_IDCounter;
//   int c_index = a_position.indexOf("|");
//   m_position.setX(a_position.left(c_index).toInt());
//   m_position.setY(a_position.right(a_position.size() - 1 - c_index).toInt());
//   qWarning() << "MapObject-Constructor " << m_position << m_filename << " " << (a_position.size() - 1 - c_index) << a_position.right(a_position.size() - 1 - c_index);
  //   for(int i = 0; i < a_position.size(); i++)
//   {
//     if()
//   }

}*/


/*Map::Map(const Map &other) : m_coordinates(other.coordinates()), m_size(other.size()), m_background (other.background()), 
	     m_mapnorth (other.mapnorth()),
	     m_mapeast (other.mapeast()),
	     m_mapsouth (other.mapsouth()),
	     m_mapwest(other.mapwest()), m_type (other.type())
{
  mapcounter ++;
  qWarning() << "Count of map instances:" << mapcounter;
}*/

/// MAP
Map::Map() : m_filename (QString()),
	     m_size (QSize(1023,1023)),
	     m_background (QString("0")),
	     m_mapnorth (QString("0")),
	     m_mapeast (QString("0")),
	     m_mapsouth (QString("0")),
	     m_mapwest(QString("0")),
	     m_city (AbstractCity())
	     {
	       m_type = Sea;
	       
	       
/// mapcounter ++;
///   qWarning() << "Count of map instances:" << mapcounter;
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

Map::Map(const Map *other) :
  m_filename (other->filename()),
  m_coordinates (other->coordinates()),
  m_size (other->size()),
  m_background (other->background()),
  m_mapnorth (other->mapnorth()),
  m_mapeast (other->mapeast()),
  m_mapsouth (other->mapsouth()),
  m_mapwest (other->mapwest()),
  m_type (other->type()),
  m_Objects (other->m_Objects)
{
//   if(!other->city() == NULL )
  if(isCity() && other->isCity())
  {
    #ifdef DEBUG_MAP
    qWarning() << "before copiing city";
    #endif
    
    m_city = *other->city();
    
    #ifdef DEBUG_MAP
    qWarning() << "after copiiing city";
    #endif
  }
//   else
//     m_city = 0;
  
  ///mapcounter ++;
  ///qWarning() << "Count of map instances:" << mapcounter;
}

/*Map::~Map()
{
  ///static int diff;
  ///diff++;
  ///qWarning() << "Count of map instances:" << mapcounter << "Active:" << mapcounter - diff;
  
// #ifdef _DEBUG_DESTRUCTORS
// qWarning() << "Destructing Map";
// #endif
//  MapObject *mapobjectit = 0;	//Map-Object-Iterator
//  foreach(mapobjectit, m_ObjectList)
//  {
//     delete mapobjectit;
//  }
// #ifdef _DEBUG_DESTRUCTORS
// #endif
// qWarning() << "deleting city";
// if(!(m_city == NULL))
//    delete m_city;
qWarning() << "End of Destructing Map";

}*/

void Map::setCity(const AbstractCity &newCity)
{
  //   delet e m_city;
  #ifdef DEBUG_MAP
  qWarning() << "SetCity";
  #endif
  
  m_city = newCity;
  
  #ifdef DEBUG_MAP
  qWarning() << "City setted";
  #endif
}


// City *Map::cityinstance()
// {
//   qWarning() << "Cityinstance()";
//   if(m_city == NULL)
//   {
//     qWarning() << "New instance";
//     m_city = new City();
//   }
//   if(!isCity())
//   {
//     m_type ^= Citymap;
//   }
//   qWarning() << "Return";
//   return m_city;
// }

// City Map::city()	const
// {
//   if(m_city == NULL)
//   {
//     City ct;
//     return ct;
//   }
//   else
//     return *m_city;
// }

// const City *city()	const;
// {
//   return m_city;
// }



bool Map::isCity() const
{
  return ((m_type == (Coast ^ Citymap)) || (m_type == (Land ^ Citymap)));
}



/*void Map::loadStartMap(const QString &mapfilename)
{
static bool started;
if(!started)
{
started = load(mapfilename);
m_coordinates = QPoint(400,400);
}
else if(mapfilename == "false")
{
started = false;
}

}*/

#ifdef OH_GAME
bool Map::load(Orientations mapdirection)
{
	switch(mapdirection)
	{
		case North:
		{
			if(load(m_mapnorth))
			{
				m_coordinates.setY(m_coordinates.y() + 1);
				return true;
			}
			break;
		}
		case East:
		{
			if(load(m_mapeast))
			{
				m_coordinates.setX(m_coordinates.x() + 1);
				return true;
			}
			break;
		}
		case South:
		{
			if(load(m_mapsouth))
			{
				m_coordinates.setY(m_coordinates.y() - 1);
				return true;
			}
			break;
		}
		case West:
		{
			if(load(m_mapwest))
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
#endif


/*bool Map::load(QString param_mapname)
{
	qWarning() << "bool Map::load(QString param_mapname)" << "Karteladen: " << param_mapname;
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
				  m_city = AbstractCity(reader.attributes().value("name").toString()/*, reader.attributes().value("id").toString().toInt()*///);
				  /// City-ID
// 				  cityname = reader.attributes().value("name").toString();
// 				  id = reader.attributes().value("id").toString().toInt();
// 				  m_city.setName(cityname);
// 				  m_city.setID(id);
// 				  status = e_map_cityname;
/*
				  break;
				}
				else if(reader.name().toString() == "production" && f_isCity)
				{
// 				  QString lowp = reader.attributes().value("low").toString();
// 				  QString nmlp = reader.attributes().value("normal").toString();
// 				  QString highp = reader.attributes().value("high").toString();
				  m_city.setProduction(Goods::LowProduction, reader.attributes().value("low").toString());
				  m_city.setProduction(Goods::NormalProduction, reader.attributes().value("normal").toString());
				  m_city.setProduction(Goods::HighProduction, reader.attributes().value("high").toString());
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
				  m_type = reader.text().toString().toInt();
					/*m_type =
					  static_cast<Map::MapType>( reader.text().toString().toInt() );*//*
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
					
					MapObject currentMO = MapObject(object_role, object_file, object_tooltip, QPoint(object_posx, object_posy), object_zvalue);
					
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
}*/

#include <QtDebug>

#define LOAD_save_MAPOBJECTS


bool Map::load(const QString &a_filename)
{
  #ifdef DEBUG_MAP
  qWarning() << "bool Map::load(const QString &a_filename)" << " (Karteladen): " << a_filename;
  #endif
  QString mapdirectory = QFileInfo(a_filename).absoluteDir().absolutePath();
  
  QFile file(a_filename);		//Map-XML-Lesen
  if(file.exists())
  {
    m_filename = a_filename;
    #ifdef DEBUG_MAP
    qWarning() << "Mapfile is existing";
    #endif
    
    if(file.open(QIODevice::ReadOnly))
    {
      QXmlStreamReader reader(&file);
      while (!reader.atEnd()) 
      {
	switch(reader.readNext())
	{
	  case QXmlStreamReader::StartElement:
	  {
	    QStringRef elementname = reader.name();
	    #ifdef DEBUG_MAP
	    qWarning() << "\nStart:\t" << elementname.toString();
	    #endif
	    
#ifndef LOAD_save_MAPOBJECTS
	    if(elementname == "object")
	    {
	      QXmlStreamAttributes objectattributes = reader.attributes();
	      
	      
	      for(QXmlStreamAttributes::const_iterator it = objectattributes.begin(); it != objectattributes.end(); ++it)
		qWarning() << "add Object:" << it->name() << it->value() ;
	      
	      m_ObjectList << MapObject(objectattributes.value("role").toString().toInt(),(objectattributes.value("file").toString()), toPoint(objectattributes.value("position").toString()), (objectattributes.value("tooltip").toString()), (objectattributes.value("name").toString()), objectattributes.value("zvalue").toString().toDouble());
	      
	      break;
	    }
	    
#endif

///******************************************************************************************************///

#ifdef LOAD_save_MAPOBJECTS

	    if(elementname == "objects")
	    {
	      bool objects = true;
	      QStringRef it_name;
	      
	      QStringRef f_file, f_position, f_name, f_tooltip;
	      int f_role(-1);
	      double f_zvalue(0);
	      
	      while(objects)
	      {
		switch(reader.readNext())
		{
		  case QXmlStreamReader::StartElement:
		  {
		    if(reader.name() == "object")
		    {
		      QXmlStreamAttributes objectattributes = reader.attributes();
		      for(QXmlStreamAttributes::const_iterator it = objectattributes.begin(); it != objectattributes.end(); ++it)
		      {
			it_name = it->name();
			
			qWarning() << it_name;
			
			if(it_name == "role")
			{
			  f_role = it->value().toString().toInt();
			}
			else if(it_name == "file")
			{
			  f_file = it->value();
			}
			else if(it_name == "position")
			{
			  f_position = it->value();
			}
			else if(it_name == "zvalue")
			{
			  f_zvalue = it->value().toString().toDouble();
			}
			else if (it_name == "name")
			{
			  f_name = it->value();
			}
			else if(it_name == "tooltip")
			{
			  f_tooltip = it->value();
			}
		      }
		      qWarning() << "File/Pos/Name/Tooltip" << f_file << f_position << f_name << f_tooltip;
		      MapObject newMO(f_role, f_file.toString(), toPoint(f_position.toString()), f_name.toString() , f_tooltip.toString(), f_zvalue);
		      
		      m_Objects[newMO.id()] = newMO;
		      
		      f_file = f_position = f_name = f_tooltip = QStringRef();
		      f_role = -1;
		      f_zvalue = 0;
		      
		    }
		    break;
		  }
		  case QXmlStreamReader::EndElement:
		  {
		    if(reader.name() == "objects")
		    {
		      objects = false;
		    }
		    break;
		  }
		  default:
		    break;
		}
	      }
	      break;
	    }
	    #endif
	    
	    else if(elementname == "city")
	    {
	      m_city = AbstractCity(reader.attributes().value("name").toString()/*, reader.attributes().value("id").toString().toInt()*/);
	      /// City-ID
	      // 				  cityname = reader.attributes().value("name").toString();
	      // 				  id = reader.attributes().value("id").toString().toInt();
	      // 				  m_city.setName(cityname);
	      // 				  m_city.setID(id);
	      // 				  status = e_map_cityname;
	      break;
	    }
	    else if(reader.name().toString() == "production" && isCity())
	    {
// 				  QString lowp = reader.attributes().value("low").toString();
// 				  QString nmlp = reader.attributes().value("normal").toString();
// 				  QString highp = reader.attributes().value("high").toString();
	      m_city.setProduction(Goods::LowProduction, reader.attributes().value("low").toString());
	      m_city.setProduction(Goods::NormalProduction, reader.attributes().value("normal").toString());
	      m_city.setProduction(Goods::HighProduction, reader.attributes().value("high").toString());
	      break;
	    }
	    else if(reader.name() == "production_goods" && isCity())
	    {
	      m_city.production().setXmlAttributes(reader.attributes());
	    }
	    else if(reader.name().toString() == "map")
	    {
	       #ifdef DEBUG_MAP
	      qWarning() << reader.attributes().value("height").string()->toInt() << reader.attributes().value("height").toString().toInt();
	      #endif
// 	      m_size.setHeight(reader.attributes().value("height").string()->toInt());
// 	      m_size.setWidth(reader.attributes().value("width").string()->toInt());
	      m_size.setHeight(reader.attributes().value("height").toString().toInt());
	      m_size.setWidth(reader.attributes().value("width").toString().toInt());

	      m_type = reader.attributes().value("type").toString().toInt();
	      m_background = reader.attributes().value("background").toString();
	      break;
	    }
	    else if(reader.name().toString() == "adjoining_maps")
	    {
	      m_mapnorth= (reader.attributes().value("north").toString());
	      m_mapeast	= (reader.attributes().value("east").toString());
	      m_mapsouth= (reader.attributes().value("south").toString());
	      m_mapwest	= (reader.attributes().value("west").toString());
	      break;
	    }
	    break;
	  }
	  /// END : CASE StartElement
// 	    else if(reader.name().toString() == "mapeast")
// 	    {
// 	      status = e_map_east;
// 	      break;
// 	    }
// 				else if(reader.name().toString() =="mapsouth")
// 				{
// // 				qWarning() << "Start: gamedata->currentMap";
// 				status = e_map_south;
// 				break;
// 				}
// 				else if(reader.name().toString() =="mapwest")
// 				{
// // 				qWarning() << "Start: gamedata->currentMap";
// 				status = e_map_west;
// 				break;
// 				}
// 				else if(reader.name().toString() =="mapwidth")
// 				{
// // 				qWarning() << "Start: gamedata->currentMap";
// 				status = e_map_width;
// 				break;
// 				}
// 				else if(reader.name().toString() =="mapheight")
// 				{
// // 				qWarning() << "Start: gamedata->currentMap";
// 				status = e_map_height;
// 				break;
// 				}
// 
// 				else if(reader.name().toString() =="maptype")
// 				{
// // 				qWarning() << "Start: gamedata->currentMap";
// 				status = e_map_typ;
// 				break;
// 				}



// 				else if(reader.name().toString() == "objectfunktion")
// 				{
// // 				qWarning() << "o_role";
// 				status = e_object_role;
// 				break;
// 				}
// 
// 				else if(reader.name().toString() == "objecttooltip")
// 				{
// // 				qWarning() << "o_tooltip";
// 				status = e_object_tooltip;
// 				break;
// 				}
// 
// 				else if(reader.name().toString() == "objectdatei")
// 				{
// // 				qWarning() << "o_file";
// 				status = e_object_file;
// 				break;
// 				}
// 
// 				else if(reader.name().toString() == "objectpositionx")
// 				{
// // 				qWarning() << "o_posx";
// 				status = e_object_posx;
// 				break;
// 				}
// 
// 				else if(reader.name().toString() == "objectpositiony")
// 				{
// // 				qWarning() << "o_posy";
// 				status = e_object_posy;
// 				break;
// 				}
// 				
// 				else if(reader.name().toString() == "objecthoehe")
// 				{
// // 				qWarning() << "o_posy";
// 				status = e_object_zvalue;
// 				break;
// 				}
// 				else
// 				{
// 				status = e_null;
// 				}
// 				break;
// 	  }
	  #ifdef DEBUG_MAP
	  case QXmlStreamReader::Invalid:
	  {
	    qWarning() << "Error:" << reader.errorString() <<"\nEnde Error" ;
	    break;
	  }

	  case QXmlStreamReader::Characters:
	  {
	    qWarning() << "Characters:" << reader.text().toString();
	    break;
	  }
			  /*
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
				  m_type = reader.text().toString().toInt();
					/*m_type =
					  static_cast<Map::MapType>( reader.text().toString().toInt() );*//*
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
				}*/
// 	    break;
// 	  }
	  case QXmlStreamReader::EndElement:
	  {
	    qWarning() << "Ende :"<< reader.name().toString();
/*				if(reader.name().toString() == "object" && object_role != - 1 && !object_file.isEmpty())
	//jetzt zeichnen: habe alle Eigenschaften des Objektes erhalten?
				{
					qWarning() << object_file << object_role;
					
					MapObject currentMO = MapObject(object_role, object_file, object_tooltip, QPoint(object_posx, object_posy), object_zvalue);
					
					m_ObjectList << currentMO;
					
					object_role = -1;
					object_tooltip = QString();
					object_file = QString();
					object_posx = 0;
					object_posy = 0;
					object_zvalue = 0;
					 {
					   {
					     {
					       {
						 {
						   {
						     {
						       {
						       }
						     }
						   }
						 }
						 
					       }
					     }
					   }
					 }
					 
				}
				status = e_null;*/
	    break;
	  }
	#endif
	default:
	  break;
	}
	
	if (reader.hasError())
	{
	  #ifdef DEBUG_MAP
	  qWarning() << reader.errorString();
	  #endif
	  #ifndef DEBUG_MAP
	  qWarning(reader.errorString().toUtf8().data());
	  #endif
	  
	}
	
      }
      #ifdef DEBUG_MAP
      qWarning() << "Returning true: Map sucessfully read!";
      #endif
      return true;
    }
  }
  return false;
  
}

void Map::addObject(const MapObject *a_newMO)
{
  if(a_newMO != 0)
  {
    m_Objects[a_newMO->id()] = (*a_newMO);
  }
}

void Map::removeObject(int a_id)
{
  m_Objects.remove(a_id);
}

// const MapObject *Map::object(int a_id);
// {
//   if
// }



// void Map::setFilename	(const QString &nfn)
// {
//   m_filename = nfn;
// }

void Map::setName(const QString &a_name)
{
//   if(!a_name.isEmpty())
    m_name = a_name;
//   else
//     m_name = "0";
}


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

void Map::save(const QString &a_filename)
{
  #ifdef DEBUG_MAP
  qWarning() << "void Map::save(const QString &a_filename)" << a_filename;
  #endif
  if(!a_filename.isEmpty())
    m_filename = a_filename;
  if(!m_filename.isEmpty())
  {
    QFile savefile(m_filename);
    savefile.open(QIODevice::WriteOnly);
    QXmlStreamWriter writer(&savefile);
    #if QT_VERSION >= 0x040400
    writer.setAutoFormatting(true);
    #if QT_VERSION >= 0x040500
    writer.writeStartDocument();
    #endif
    #endif
    writer.writeStartElement("map");
    //     void (*save) (/*const*/ QString /*&*/, /*const*/ QString /*&*/) =  &writer.writeAttribute;
    //     void 
    //     void (QXmlStreamWriter::*save) (const QXmlStreamAttribute&);
    // //     save =  &QXmlStreamWriter::writeAttribute;/*writer.writeAttribute;*/
    //     save = &writer.writeAttribute;
    writer.writeAttribute("size", fromSize(m_size));
			  
    writer.writeAttribute(QXmlStreamAttribute("width",	QString::number(m_size.width())));
    writer.writeAttribute(QXmlStreamAttribute("height",	QString::number(m_size.height())));
    writer.writeAttribute(QXmlStreamAttribute("background", (m_background)));
    writer.writeAttribute(QXmlStreamAttribute("name", (m_name)));
    writer.writeAttribute(QXmlStreamAttribute("type",	QString::number(m_type)));
    
    writer.writeEmptyElement("adjoining_maps");
    writer.writeAttribute("north",	(m_mapnorth));
    writer.writeAttribute("east",	(m_mapeast));
    writer.writeAttribute("south",	(m_mapsouth));
    writer.writeAttribute("west",	(m_mapwest));
    
    
/*    if(!m_mapnorth.isEmpty() || !m_mapeast.isEmpty() ||!m_mapsouth.isEmpty() || !m_mapwest.isEmpty())
    {
      writer.writeStartElement("adjoining maps");
      if(!m_mapnorth.isEmpty())
      {
	writer.writeEmptyElement("north");
	writer.writeAttribute(QXmlStreamAttribute("file",	m_mapnorth));
      }
      if(!m_mapeast.isEmpty())
      {
	writer.writeEmptyElement("east");
	writer.writeAttribute(QXmlStreamAttribute("file",	m_mapeast));
      }
      if(!m_mapsouth.isEmpty())
      {
	writer.writeEmptyElement("south");
	writer.writeAttribute(QXmlStreamAttribute("file",	m_mapsouth));
      }
      if(!m_mapwest.isEmpty())
      {
	writer.writeEmptyElement("west");
	writer.writeAttribute(QXmlStreamAttribute("file",	m_mapwest));
      }
      writer.writeEndElement();
    }*/
    if(isCity())
    {
      writer.writeStartElement("city");
      writer.writeAttribute(QXmlStreamAttribute("name", m_city.name()));
//       writer.writeEmptyElement("production");
//       writer.writeAttribute(QXmlStreamAttribute("low",	m_city.lowproduction()));
//       writer.writeAttribute(QXmlStreamAttribute("normal",m_city.normalproduction()));
//       writer.writeAttribute(QXmlStreamAttribute("high",	m_city.highproduction()));
      writer.writeEmptyElement("production_goods");
      writer.writeAttributes(m_city.production().xmlAttributes());
      writer.writeEndElement();
    }
    
    
/*    enum     {      oFile = 0,      oRole = 1,      oName = 2,      oTooltip = 3,      oPosition = 4,  oZValue = 5    };    
    QString (*
    for(int i = 0; i < oZValue; i++)      {      }*/
    writer.writeStartElement("objects");
    QHash <int, MapObject>::const_iterator it;
    for(it = m_Objects.begin(); it != m_Objects.end(); ++it) 
    {
      
      writer.writeEmptyElement("object");

      writer.writeAttribute(QXmlStreamAttribute("file",		(it->filename())));
      writer.writeAttribute(QXmlStreamAttribute("role",		QString::number(it->role())));
      writer.writeAttribute(QXmlStreamAttribute("tooltip",	(it->tooltip())));
      writer.writeAttribute(QXmlStreamAttribute("name",		(it->name())));
      writer.writeAttribute(QXmlStreamAttribute("position",	fromPoint(it->position())));
      writer.writeAttribute(QXmlStreamAttribute("zvalue",	QString::number(it->zValue())));
    }
    writer.writeEndDocument();
    savefile.close();
  }
  
}

Map &operator<=(Map &m1, const Map &m2)
{
  #ifdef DEBUG_MAP
  qWarning() << "Map &operator<=(Map &m1, const Map &m2)";
  #endif
  
  m1.setBackground(m2.background());
  m1.setCoordinates(m2.coordinates());
  m1.setMapeast(m2.mapeast());
  m1.setMapnorth(m2.mapnorth());
  m1.setMapsouth(m2.mapsouth());
  m1.setMapwest(m2.mapwest());
  m1.setSize(m2.size());
  
  #ifdef DEBUG_MAP
  qWarning() << "  int m2type = m2.type();";
  #endif
  
  int m2type = m2.type();
  m1.setType(m2type);
  //   if(m2type == Map::Coast ^ Map::Citymap || m2type == Map::Land ^ Map::Citymap)
  if(m2.isCity())
  {
    
    #ifdef DEBUG_MAP
    qWarning() << "    m1.setCity(m2.city());" << m2.city()->name();
    #endif
    
    //     if(m2.city() != 0)
    m1.setCity(*m2.city());
  }
  
  #ifdef DEBUG_MAP
  qWarning() << "  return m1;";
  #endif
  
  return m1;
}


