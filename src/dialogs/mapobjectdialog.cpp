/***************************************************************************
 *   Copyright (C) 2009 - 2010 by Christian Doerffel                       *
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

#include "mapobjectdialog.h"
#include "map.h"
#include "mapframe.h"
#include "settings.h"

#include <QtCore/QList>
#include <QtGui/QGraphicsItem>
#include <QtGui/QFileDialog>

MapObjectDialog::MapObjectDialog(Map *a_map, const QPoint &a_point ,const QList<QGraphicsItem*> &a_ObjectList)
{
  ui.setupUi(this);
  QList<QGraphicsItem*>::const_iterator f_giit;		// function-graphicsitem-iterator
  QList<MapObject>::iterator f_moit;			// function-mapobject-iterator
  QList<MapObject> *f_molist = a_map->objects();	// function-mapobject-list
  
  for(f_moit = f_molist->begin(); f_moit != f_molist->end(); f_moit++)
  {
    int f_moid = f_moit->id();
    for(f_giit = a_ObjectList.begin(); f_giit != a_ObjectList.end(); ++f_giit)
    {     
      if(f_moid == (*f_giit)->data(Qt::UserRole).toInt())
      {
// 	m_MapObjectList << f_moit->;
      }
    }
  }
//   ui.ObjectSelectionBox.setItemData(ui.ObjectSelectionBox.currentIndex(), QVariant(NewObjectSelectionBoxItem));
  ui.ObjectSelectionBox->addItem(tr("<New Object>"), QVariant(NewObjectSelectionBoxItem));
  
  QList<MapObject *>::iterator f_mopit;
  for(f_mopit = m_MapObjectList.begin(); f_mopit != m_MapObjectList.end(); f_mopit++)
  {
  ui.ObjectSelectionBox->addItem((*f_mopit)->name(), QVariant((*f_mopit)->id()));
  }
  
  QHash<int, QString>::const_iterator f_roleIt;
  const QHash<int, QString> &f_objectfunctionshash = SETTINGS->ObjectFunctionsRef();
  for(f_roleIt = f_objectfunctionshash.begin(); f_roleIt != f_objectfunctionshash.end(); ++f_roleIt)
  {
    if(f_roleIt.key() > 5000 || a_map->isCity())
      ui.ObjectRoleBox->addItem(f_roleIt.value(), QVariant(f_roleIt.key()));
  }
  
  ui.XSpinBox->setRange(0, a_map->size().width());
  ui.YSpinBox->setRange(0, a_map->size().height());
  
  ui.XSpinBox->setValue(a_point.x());
  ui.YSpinBox->setValue(a_point.y());

  changeItem();
}

void MapObjectDialog::changeItem()
{
  if(ui.ObjectSelectionBox->itemData(ui.ObjectSelectionBox->currentIndex()).toInt() == NewObjectSelectionBoxItem)
  {
    ui.ObjectSelectionBox->setEditable(false);
    QPushButton *ptr = ui.buttonBox->button(QDialogButtonBox::Apply);
    if(ptr != 0)
    {
      ptr->setEnabled(false);
    }
  }
  else
  {
    ui.ObjectSelectionBox->setEditable(true);
    QPushButton *ptr = ui.buttonBox->button(QDialogButtonBox::Apply);
    if(ptr != 0)
    {
      ptr->setEnabled(true);
    }
//     ui.ObjectSelectionBox->setEditable(ui.ObjectSelectionBox->itemData(ui.ObjectSelectionBox->currentIndex()).toInt() != NewObjectSelectionBoxItem);
  }      
}

void MapObjectDialog::setObjectFile()
{
  QString objectfile = QFileDialog::getOpenFileName(this, tr("Object Image File"), QString(), tr("Images (*.png *.jpg *.gif *.svg)"));
  ui.ObjectFilePathLabel->setText(objectfile);
}


void MapObjectDialog::applyChanges(QAbstractButton *a_clickedButton)
{
  if(ui.buttonBox->buttonRole(a_clickedButton) == QDialogButtonBox::ApplyRole)	// if Apply-Button was clicked
  {
    if(ui.ObjectSelectionBox->itemData(ui.ObjectSelectionBox->currentIndex()).toInt() == NewObjectSelectionBoxItem)
    {
      
    }
    else
    {
      
    }
  }
}


void MapFrame::objectDialog(const QPoint &a_point)
{
  QList<QGraphicsItem*> f_ol = scene()->items(a_point);	// function_objectlist
  m_ObjectDialog = new MapObjectDialog(map(), a_point, f_ol);
  m_ObjectDialog->exec();
  connect(m_ObjectDialog, SIGNAL(finished(int)), this, SLOT(closeObjectDialog(int)));
  
}


void MapFrame::closeObjectDialog(int a_result)
{
  if(a_result == 1)
  {
    
  }
  
  m_ObjectDialog->deleteLater();
  
}

