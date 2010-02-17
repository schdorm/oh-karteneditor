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

#ifndef _MappropertyWidget_h
#define _MappropertyWidget_h

#include <QtGui/QDialog>
#include "ui_mapsettingsdialog.h"
#include "map.h"

class MapSettingsDialog : public QDialog
{
  Q_OBJECT
  public:
    MapSettingsDialog(const Map *);
  
    Map map()
    {
      return m_map;
    }
    
/*    Map smap() const
    {
      return s_map;
    }*/
    void setCityProperties();
    
  public slots:
    void backgroundFileDialog();
    void northmapFileDialog();
    void southmapFileDialog();
    void westmapFileDialog();
    void eastmapFileDialog();
    void setMapProperties();



    
    //   void startDeleteTimer();
  signals:
    void apply();
    

  private:
    Ui::MapSettingsDialog ui;
    Map m_map;
//     Map s_map;
  protected:

};

#endif
