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

#include "mapsettingsdialog.h"
#include "mainwindow.h"
#include "mapframe.h"
#include "settings.h"
#include "gameparameter.h"
#include "sidebar.h"
#include "abstractcity.h"

#include <QtGui/QFileDialog>
#include <QtDebug>

 #define DEBUG_MAPPROPERTIES


MapSettingsDialog::MapSettingsDialog (const Map *a_map) /*: s_map(a_map)*/
{
  //   m_map = new Map(a_map);
  if(a_map != NULL)
  {
    qWarning() << "Stadtname: " << a_map->city()->name();
    m_map <= *a_map;
  }
  
  qWarning() << a_map->type() << m_map.type();
  ui.setupUi(this);
  
  /// add the items to the Maptypecombobox
  QHashIterator<int, QString> it(SETTINGS->MapTypes());
  while(it.hasNext())
  {
    it.next();
    ui.MapTypeBox->addItem(it.value(), QVariant(it.key()));
    qWarning() << it.value() << it.key();
  }
  
  /// add the items to the productionlistwidget
  it = QHashIterator<int, QString>(GAMEPARAMETER->GoodLabels());
  QString name;
  QListWidgetItem *goodlabel;
  bool MapIsCity = a_map->isCity();
  int maptype = a_map->type(); 
  while(it.hasNext())
  {
    it.next();
    #ifdef DEBUG_MAPPROPERTIES
    qWarning() << "Key | Value: " << it.key() << it.value() << "Production: " << m_map.city()->production().value(it.key()) ;
    #endif
    if(it.key() < 10)
      goodlabel = new QListWidgetItem ("0" + QString::number(it.key()) + " " + it.value()/*, ui.GoodsListWidget*/);
    else
      goodlabel = new QListWidgetItem (QString::number(it.key()) + " " + it.value()/*, ui.GoodsListWidget*/);
    
    if(MapIsCity) // == if map is City
    {
      switch (m_map.city()->production().value(it.key()) )
      {
	case Goods::HighProduction:
	{
	  ui.HighProductionListWidget->addItem(goodlabel);
	  break;
	}
	case Goods::NormalProduction:
	{
	  ui.NormalProductionListWidget->addItem(goodlabel);
	  break;
	}
	case Goods::LowProduction:
	{
	  ui.LowProductionListWidget->addItem(goodlabel);
	  break;
	}
	default:
	{
	  ui.GoodsListWidget->addItem(goodlabel);
	  break;
	}
      }
    }
    else
      ui.GoodsListWidget->addItem(goodlabel);
    
    goodlabel->setData(Qt::UserRole, QVariant(it.key()));
  }
  if(MapIsCity) // == if map is City
  {
    ui.CityCheckBox->setChecked(true);
    maptype ^= Map::Citymap;
    #ifdef DEBUG_MAPPROPERTIES
    qWarning() << "      ui.CitynameLine->setText(a_map->city()->name());";
    #endif 
    ui.CitynameLine->setText(a_map->city()->name());  
    qWarning() << "Cityname" << a_map->city()->name();
  }
/*      if(m_map.city()->production().value(it.key()) == Goods::HighProduction)
      {
	ui.LowProductionListWidget->addItem(goodlabel);
      }
      else if(m_map.city()->production().value(it.key()) == Goods::NormalProduction)
      {
	ui.LowProductionListWidget->addItem(goodlabel);
      }
      else if(m_map.city()->production().value(it.key()) == Goods::LowProduction)
      {
	ui.LowProductionListWidget->addItem(goodlabel);
      }
      else
	ui.GoodsListWidget->addItem(goodlabel);*/

  

  #ifdef DEBUG_MAPPROPERTIES
  qWarning() << "MapType" << maptype << a_map->type() << m_map.type();
  #endif
//   switch (maptype)
//   {
//     case Map::Land ^ Map::Citymap:
//     case Map::Coast ^ Map::Citymap:
//     {
//   if(a_map->isCity())
//   {
//       ui.CityCheckBox->setChecked(true);
      /*maptype ^= Map::Citymap;
      #ifdef DEBUG_MAPPROPERTIES
      qWarning() << "      ui.CitynameLine->setText(a_map->city()->name());";
      #endif
      
      qWarning() << "Cityname" << a_map->city().name();*/
//   }
//     }
//     default:
//     {
//       qWarning() << "MapType default" << maptype;
      ui.MapTypeBox->setCurrentIndex(ui.MapTypeBox->findData(QVariant(maptype)));
//       break;
//     }
//   }
  ui.BackgroundfileLabel->setText(m_map.background());
  ui.NorthmapfileLabel->setText(m_map.mapnorth());
  ui.SouthmapfileLabel->setText(m_map.mapsouth());
  ui.WestmapfileLabel->setText(m_map.mapwest());
  ui.EastmapfileLabel->setText(m_map.mapeast());
  ui.HeightSpinBox->setValue(a_map->size().height());
  ui.WidthSpinBox->setValue(a_map->size().width());
  
//   connect(this, SIGNAL(accepted()), this, SLOT(setMapProperties()));
}


void MapSettingsDialog::backgroundFileDialog()
{
  QString dir;
  if(m_map.background().isEmpty())
    dir = QDir::current().absolutePath();
  else
    dir = QFileInfo(m_map.background()).absolutePath();
  
  QString f_bgf = QFileDialog::getOpenFileName(this, tr("Background File"), dir, tr("Images (*.png *.jpg *.gif)"));
  
  if(!f_bgf.isEmpty())
  {
    m_map.setBackground(f_bgf);
    ui.BackgroundfileLabel->setText(f_bgf);
  }
  
}

void MapSettingsDialog::northmapFileDialog()
{
  QString dir;
  if(m_map.mapnorth().isEmpty())
    dir = QDir::current().absolutePath();
  else
    dir = QFileInfo(m_map.mapnorth()).absolutePath();
  
  QString f_mf = QFileDialog::getOpenFileName(this, tr("North joining Map File"), dir,tr("OpenHanse Maps (*.ohm)"));
  
  if(!f_mf.isEmpty())
  {
    m_map.setMapnorth(f_mf);
    ui.NorthmapfileLabel->setText(f_mf);
  }
}

void MapSettingsDialog::southmapFileDialog()
{
  QString dir;
  if(m_map.mapsouth().isEmpty())
    dir = QDir::current().absolutePath();
  else
    dir = QFileInfo(m_map.mapsouth()).absolutePath();
  
  QString f_mf = QFileDialog::getOpenFileName(this, tr("South joining Map File"), dir, tr("OpenHanse Maps (*.ohm)"));
  qWarning() << "f_MF: "<< f_mf;
  
  if(!f_mf.isEmpty())
  {
    m_map.setMapsouth(f_mf);
    ui.SouthmapfileLabel->setText(f_mf);
  }
}

void MapSettingsDialog::westmapFileDialog()
{
  QString dir;
  if(m_map.mapwest().isEmpty())
    dir = QDir::current().absolutePath();
  else
    dir = QFileInfo(m_map.mapwest()).absolutePath();
  
  QString f_mf = QFileDialog::getOpenFileName(this, tr("West joining Map File"), dir, tr("OpenHanse Maps (*.ohm)"));
  qWarning() << "f_MF: "<< f_mf;
  
  if(!f_mf.isEmpty())
  {
    m_map.setMapwest(f_mf);
    ui.WestmapfileLabel->setText(f_mf);
  }
}

void MapSettingsDialog::eastmapFileDialog()
{
  QString dir;
  if(m_map.mapeast().isEmpty())
    dir = QDir::current().absolutePath();
  else
    dir = QFileInfo(m_map.mapeast()).absolutePath();
  
  QString f_mf = QFileDialog::getOpenFileName(this, tr("East joining Map File"), dir, tr("OpenHanse Maps(*.ohm)"));
  qWarning() << "f_MF: "<< f_mf;
  
  if(!f_mf.isEmpty())
  {
    m_map.setMapeast(f_mf);
    ui.EastmapfileLabel->setText(f_mf);
  }
}


void MapSettingsDialog::setMapProperties()
{
  #ifdef DEBUG_MAPPROPERTIES
  qWarning() << "void MapSettingsDialog::setMapProperties()";
  #endif
  
  m_map.setSize(QSize(ui.WidthSpinBox->value(), ui.HeightSpinBox->value()));
  
  int maptype = ui.MapTypeBox->itemData(ui.MapTypeBox->currentIndex()).toInt();
  if(maptype == Map::Sea)
  {
    ui.CityCheckBox->setChecked(false);
//     ui.CityCheckBox->setEnabled(false);
  }
  ui.CityCheckBox->setEnabled(maptype != Map::Sea);
  

  
  
  #ifdef DEBUG_MAPPROPERTIES
    qWarning() << "End of \"void MapSettingsDialog::setMapProperties()\"";
  #endif
}

void MapSettingsDialog::setCityProperties()
{
  #ifdef DEBUG_MAPPROPERTIES
  qWarning() << "Start: \"void MapSettingsDialog::setCityProperties()\"";
  #endif
  int f_typ = ui.MapTypeBox->itemData(ui.MapTypeBox->currentIndex()).toInt();

  if(ui.CityCheckBox->isChecked())
  {
    f_typ = f_typ ^ Map::Citymap;
    
    #ifdef DEBUG_MAPPROPERTIES
      qWarning() << "f_typ:::: " << f_typ /*<< (f_typ ^ Map::Citymap)*/;
    #endif
    
    QString highproduction, normalproduction, lowproduction;
    int f_h, f_n, f_l;
    for(f_h = 0; f_h < ui.HighProductionListWidget->count(); f_h ++)
    {
      highproduction += ui.HighProductionListWidget->item(f_h)->data(Qt::UserRole).toString() + ",";
    }
    for(f_n = 0; f_n < ui.NormalProductionListWidget->count(); f_n ++)
    {
      normalproduction += ui.NormalProductionListWidget->item(f_n)->data(Qt::UserRole).toString() + ",";
    }
    for(f_l = 0; f_l < ui.LowProductionListWidget->count(); f_l ++)
    {
      lowproduction += ui.LowProductionListWidget->item(f_l)->data(Qt::UserRole).toString() + ",";
    }
    
    #ifdef DEBUG_MAPPROPERTIES
      qWarning() << "For iterations done";
    #endif
    
    if(f_h > 0)
    {
//       qWarning() << "Highproduction: " << highproduction;
//       highproduction.chop(1);
      m_map.city()->setProduction(Goods::HighProduction, highproduction);
      qWarning() << "Highproduction: " << highproduction;
    }
    if(f_n > 0)
    {
//       normalproduction.chop(1);
      m_map.city()->setProduction(Goods::NormalProduction, normalproduction);
      qWarning() << "Normalproduction: " << normalproduction;
    }
    if(f_l > 0)
    {
//       lowproduction.chop(1);
      m_map.city()->setProduction(Goods::LowProduction, lowproduction);
      qWarning() << "Lowproduction: " << lowproduction;
    }
    #ifdef DEBUG_MAPPROPERTIES
      qWarning() << "Production-ifs done";
    #endif
    
    if(!ui.CitynameLine->text().isEmpty())
      m_map.city()->setName(ui.CitynameLine->text());
  }
  
  #ifdef DEBUG_MAPPROPERTIES
    qWarning() << "  m_map.setType(f_typ); (setMapType)" << f_typ;
  #endif
  
  m_map.setType(f_typ);
  
  #ifdef DEBUG_MAPPROPERTIES
  qWarning() << "End of \"void MapSettingsDialog::setCityProperties()\"";
  #endif
}



void MainWindow::mapPropertiesDialog()
{
  MapSettings = new MapSettingsDialog(MapView->map());
  MapSettings->show();
  
  connect(MapSettings, SIGNAL(finished(int)), this, SLOT(closeMapSettingsDialog(int)));
  connect(MapSettings, SIGNAL(apply()), this, SLOT(applyMapSettings()));
//   connect(MapSettings, SIGNAL(accepted()), this, SLOT(closeMPDialog()));
//   connect(MapSettings, SIGNAL(rejected()), MapSettings, SLOT(deleteLater()));

}

void MainWindow::applyMapSettings()
{
  qWarning() << "void MainWindow::applyMapSettings()";
  MapSettings->setMapProperties();

  MapView->setMap(MapSettings->map());

}

void MainWindow::closeMapSettingsDialog(int f_result)
{
  #ifdef DEBUG_MAPPROPERTIES
  qWarning() << "void MainWindow::closeMapSettingsDialog()" << f_result;
  #endif
  
  if(f_result == 1)
  {
    MapSettings->setMapProperties();
    MapSettings->setCityProperties();
    MapView->setMap(MapSettings->map());
  }
  #ifdef DEBUG_MAPPROPERTIES
  qWarning() << "  MapSettings->deleteLater();  ";
  #endif
  
  MapSettings->deleteLater();  
  #ifdef DEBUG_MAPPROPERTIES
  qWarning() << "after: \"MapSettings->deleteLater();\"  ";
  #endif
  
  if(SETTINGS->oldlayout())
  {
    updateItemList(SideBar->itemListWidget->currentRow());
  }
  
  #ifdef DEBUG_MAPPROPERTIES
  qWarning() << "End of \"void MainWindow::closeMapSettingsDialog(int f_result)\"!";
  #endif
}
