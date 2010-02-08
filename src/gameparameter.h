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
#ifndef _gameparameter_h
#define _gameparameter_h

#include <QtCore/QHash>
#define GAMEPARAMETER GameParameter::instance()

class GameParameter : QObject
{
public:
const QHash <int, QString> &GoodLabels  () const {	return m_GoodLabelHash;		}

QString GoodName 	(int key) const {	return m_GoodLabelHash[key];		}

static GameParameter * instance();

private:
  GameParameter();
  static GameParameter *m_instance;
QHash<int, QString> m_GoodLabelHash;

QHash<int, QString> m_BuildingLabelHash;

};

#endif
