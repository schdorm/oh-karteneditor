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

#include "qstring.h"

#include <QtCore/QSize>
#include <QtCore/QPoint>

QString removeWithespace(const QString &a_string)
{
  QString string(a_string);
  string.simplified();
  string.remove(" ");
  return string;
}

QString removeWithespaceRef(QString &a_string)
{
  a_string.simplified();
  a_string.remove(" ");
  return a_string;
}

#ifdef USE_SETVALID
QString setValid(const QString &a_string)
{
  if(!a_string.isEmpty())
  {
    return a_string;
  }
  else
    return "0";
}

QString getFromValid(const QString &a_string)
{
  if(a_string == "0")
  {
    return QString();
  }
  else
    return a_string;
}
#endif

QString fromPoint1(const QPoint &a_point)
{
  return QString("Point(" + QString::number(a_point.x()) + "|" + QString::number(a_point.y()) + ")");
}

QString fromPoint(const QPoint &a_point)
{
  return QString("Point(%1|%2)").arg(a_point.x()).arg(a_point.y());
}


QPoint toPoint(const QString &a_string)
{
  QString w_string(a_string);
  removeWithespaceRef(w_string);
  w_string.remove("Point(");
  w_string.remove(")");
  int c_index = w_string.indexOf("|");
  return QPoint(w_string.left(c_index).toInt(), w_string.right(w_string.size() - 1 - c_index).toInt());
}


QString fromSize(const QSize &a_size)
{
  return QString("Size(%1|%2)").arg(a_size.width()).arg(a_size.height());
}


QSize toSize(const QString &a_string)
{
  QString w_string(a_string);
  removeWithespaceRef(w_string);
  w_string.remove("Size(");
  w_string.remove(")");
  int c_index = w_string.indexOf("|");
  return QSize(w_string.left(c_index).toInt(), w_string.right(w_string.size() - 1 - c_index).toInt());
}

QString fromBool(bool boolean)
{
  if(boolean)
    return "true";
  else
    return "false";
}

bool toBool(const QString &string)
{
  return (string == "true" || string.toInt() == 1);
}

bool toBool(const QStringRef stringref)
{
    return (stringref == "true" || stringref.string()->toInt() == 1);
}
