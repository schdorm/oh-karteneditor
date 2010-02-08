/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel                              *
 *   schdorm@googlemail.com                                                *
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

#include <QtGui/QFileDialog>
#include <QtDebug>

MapSettingsDialog::MapSettingsDialog (const Map *a_map) /*: m_map(*a_map)*/
{
  m_map = new Map(*a_map);
  qWarning() << a_map->type() << m_map->type();
  ui.setupUi(this);
  
 /// add the items to the Maptypecombobox
  QHashIterator<int, QString> it(SETTINGS->MapTypes());
  while(it.hasNext())
  {
    it.next();
    ui.MapTypeBox->addItem(it.value(), QVariant(it.key()));
    qWarning() << it.value() << it.key();
//     ui.MapTypeBox->addItem(it.value());
  }
  
   /// add the items to the productionlistwidget
//   QListWidgetItem *goodlabel = new QListWidgetItem (, QListWidget * parent = 0, int type = Type )
  it = QHashIterator<int, QString>(GAMEPARAMETER->GoodLabels());
  QString name;
  QListWidgetItem *goodlabel;
  while(it.hasNext())
  {
    it.next();
    if(it.key() < 10)
      goodlabel = new QListWidgetItem ("0" + QString::number(it.key()) + " " + it.value(), ui.GoodsListWidget);
    else
      goodlabel = new QListWidgetItem (QString::number(it.key()) + " " + it.value(), ui.GoodsListWidget);
      
    goodlabel->setData(Qt::UserRole, QVariant(it.key()));
//     ui.GoodsListWidget->addItem(goodlabel);
  }
  

  int maptype = a_map->type(); 
  qWarning() << "MapType" << maptype << a_map->type() << m_map->type();

  switch (maptype)
  {
    case Map::Land ^ Map::Citymap:
    case Map::Coast ^ Map::Citymap:
    {
      ui.CityCheckBox->setChecked(true);
      maptype ^= Map::Citymap;
    }
    default:
    {
//       qWarning() << "MapType default" << maptype;
      ui.MapTypeBox->setCurrentIndex(ui.MapTypeBox->findData(QVariant(maptype)));
      break;
    }
  }
}


void MapSettingsDialog::backgroundFileDialog()
{
  QString dir;
  if(m_map->background().isEmpty())
    dir = QDir::current().absolutePath();
  else
    dir = QFileInfo(m_map->background()).absolutePath();
  
  QString f_bgf = QFileDialog::getOpenFileName(this, tr("Background File"), dir, tr("Images (*.png *.jpg *.gif)"));
  
  if(!f_bgf.isEmpty())
  {
    m_map->setBackground(f_bgf);
    ui.BackgroundfileLabel->setText(f_bgf);
  }
  
}

void MapSettingsDialog::northmapFileDialog()
{
  QString dir;
  if(m_map->mapnorth().isEmpty())
    dir = QDir::current().absolutePath();
  else
    dir = QFileInfo(m_map->mapnorth()).absolutePath();
  
  QString f_mf = QFileDialog::getOpenFileName(this, tr("North joining Map File"), dir,tr("OpenHanse Maps (*.ohm)"));
  
  if(!f_mf.isEmpty())
  {
    m_map->setMapnorth(f_mf);
    ui.NorthmapfileLabel->setText(f_mf);
  }
}

void MapSettingsDialog::southmapFileDialog()
{
  QString dir;
  if(m_map->mapsouth().isEmpty())
    dir = QDir::current().absolutePath();
  else
    dir = QFileInfo(m_map->mapsouth()).absolutePath();
  
  QString f_mf = QFileDialog::getOpenFileName(this, tr("South joining Map File"), dir, tr("OpenHanse Maps (*.ohm)"));
  qWarning() << "f_MF: "<< f_mf;
  
  if(!f_mf.isEmpty())
  {
    m_map->setMapsouth(f_mf);
    ui.SouthmapfileLabel->setText(f_mf);
  }
}

void MapSettingsDialog::westmapFileDialog()
{
  QString dir;
  if(m_map->mapwest().isEmpty())
    dir = QDir::current().absolutePath();
  else
    dir = QFileInfo(m_map->mapwest()).absolutePath();
  
  QString f_mf = QFileDialog::getOpenFileName(this, tr("West joining Map File"), dir, tr("OpenHanse Maps (*.ohm)"));
  qWarning() << "f_MF: "<< f_mf;
  
  if(!f_mf.isEmpty())
  {
    m_map->setMapwest(f_mf);
    ui.WestmapfileLabel->setText(f_mf);
  }
}

void MapSettingsDialog::eastmapFileDialog()
{
  QString dir;
  if(m_map->mapeast().isEmpty())
    dir = QDir::current().absolutePath();
  else
    dir = QFileInfo(m_map->mapeast()).absolutePath();
  
  QString f_mf = QFileDialog::getOpenFileName(this, tr("East joining Map File"), dir, tr("OpenHanse Maps(*.ohm)"));
  qWarning() << "f_MF: "<< f_mf;
  
  if(!f_mf.isEmpty())
  {
    m_map->setMapeast(f_mf);
    ui.EastmapfileLabel->setText(f_mf);
  }
}

void MapSettingsDialog::setMapProperties()
{

  int f_typ = ui.MapTypeBox->itemData(ui.MapTypeBox->currentIndex()).toInt();
  if(ui.CityCheckBox->isChecked())
  {
    f_typ = f_typ ^ Map::Citymap;
  }
  m_map->setType(f_typ);
    qWarning() << "void MapSettingsDialog::setMapProperties()" << f_typ << (f_typ ^ Map::Citymap);

  
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
  MapView->setMap(MapSettings->map());

}

void MainWindow::closeMapSettingsDialog(int f_result)
{
  qWarning() << "void MainWindow::closeMapSettingsDialog()" << f_result;
  
  if(f_result == 1)
    MapView->setMap(MapSettings->map());
  
  MapSettings->deleteLater();  
  if(SETTINGS->oldlayout())
  {
    updateItemList(SideBar->itemListWidget->currentRow());
  }
}
