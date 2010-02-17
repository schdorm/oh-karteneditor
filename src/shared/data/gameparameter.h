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
#ifndef _gameparameter_h
#define _gameparameter_h

#include <QtCore/QHash>

#include "definitions.h"

#ifdef OH_MAPEDITOR
#define GAMEPARAMETER GameParameter::instance()
#endif

class GameParameter : public QObject	// inherited for the tr()-Function (translation)
{
  public:
    #ifdef OH_GAME
    GameParameter();
    const QHash <int, int> &GoodBasicPriceHash () const {	return m_GoodBasicPriceHash;	}
    
    const QString &firstName () const	{	return m_FirstName;	}
    const QString &lastName  () const	{	return m_LastName;	}
    #endif
    
    const QHash <int, QString> &GoodLabels  () const {	return m_GoodLabelHash;		}
    const QHash <int, QString> &BuildingLabels () const { 	return m_BuildingLabelHash;	}
    
    QString GoodName 	(int key) const {	return m_GoodLabelHash[key];		}
    
    #ifdef OH_MAPEDITOR
    static GameParameter * instance();
    #endif
    
  private:
    #ifdef OH_MAPEDITOR
    GameParameter();
    static GameParameter *m_instance;
    #endif
    
    #ifdef OH_GAME
    QHash<int, int> m_GoodBasicPriceHash;
    QString m_FirstName;		//Players first /
    QString m_LastName;			// last name
    #endif
    
    QHash<int, QString> m_GoodLabelHash;
    
    
    QHash<int, QString> m_BuildingLabelHash;
    
};

#endif
