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

#define DEBUG_MAPOBJECTDIALOG

#ifdef DEBUG_MAPOBJECTDIALOG
#include <QtDebug>
#endif

MapObjectDialog::MapObjectDialog(Map *a_map, const QPoint &a_point, const QList<QGraphicsItem*> &a_ObjectList, QGraphicsItem *a_currentGraphicsItem) : m_lockItemData(true)
{
  ui.setupUi(this);
  QList<QGraphicsItem*>::const_iterator f_giit;		// function-graphicsitem-iterator
//   QList<MapObject>::iterator f_moit;			// function-mapobject-iterator
//   QList<MapObject> *f_molist = a_map->objects();	// function-mapobject-list
  
  /*for(f_moit = f_molist->begin(); f_moit != f_molist->end(); f_moit++)	// iteration through mapobject-list
  {
    int f_moid = f_moit->id();
    for(f_giit = a_ObjectList.begin(); f_giit != a_ObjectList.end(); ++f_giit)
    {     
      if(f_moid == (*f_giit)->data(Qt::UserRole).toInt())	// if graphicsitemlist contains item with mapobject-id --> add mapobject to list
      {
	m_MapObjects[f_moid] = &*f_moit;
      }
    }
  }*/
  int f_moid;
  MapObject *f_mop;
  for(f_giit = a_ObjectList.begin(); f_giit != a_ObjectList.end(); ++f_giit)
  {
    f_moid = (*f_giit)->data(Qt::UserRole).toInt();
    f_mop = a_map->object(f_moid);
    if(f_mop->id() == f_moid)	// if graphicsitemlist contains item with mapobject-id --> add mapobject to list
    {
      m_MapObjects[f_moid] = f_mop;
    }
  }
  
  
  //   ui.ObjectSelectionBox.setItemData(ui.ObjectSelectionBox.currentIndex(), QVariant(NewObjectSelectionBoxItem));
  ui.ObjectSelectionBox->addItem(tr("<New Object>"), QVariant(NewObjectSelectionBoxItem));
  
  QHash<int, MapObject*>::iterator f_mopit;
  for(f_mopit = m_MapObjects.begin(); f_mopit != m_MapObjects.end(); f_mopit++)
  {
    ui.ObjectSelectionBox->addItem((*f_mopit)->name(), QVariant(f_mopit.key()));
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
  ui.ObjectFilePathLabel->setText(QString());
  //   changeItem();
  if(a_currentGraphicsItem != 0)
  {
    if(a_ObjectList.contains(a_currentGraphicsItem))
    {
      ui.ObjectSelectionBox->setCurrentIndex(ui.ObjectSelectionBox->findData(a_currentGraphicsItem->data(Qt::UserRole)));
      changeItem();
    }
  }
  m_lockItemData = false;
}

void MapObjectDialog::changeItem()
{
  QPushButton *f_applyButtonP = ui.buttonBox->button(QDialogButtonBox::Apply);
  QPushButton *f_accptButtonP = ui.buttonBox->button(QDialogButtonBox::Ok);
  MapObject *f_currentMO;
  if(f_applyButtonP != 0 )
  {
    f_applyButtonP->setEnabled(!ui.ObjectFilePathLabel->text().isEmpty());
  }
  if(f_accptButtonP != 0 )
  {
    f_accptButtonP->setEnabled(!ui.ObjectFilePathLabel->text().isEmpty());
  }
  if(ui.ObjectSelectionBox->itemData(ui.ObjectSelectionBox->currentIndex()).toInt() == NewObjectSelectionBoxItem)
  {
//     ui.ObjectSelectionBox->setEditable(false);
    f_currentMO = &m_newMapObject;
  }
  else
  {
    f_currentMO = m_MapObjects[ui.ObjectSelectionBox->itemData(ui.ObjectSelectionBox->currentIndex()).toInt()];
    
//     ui.ObjectSelectionBox->setEditable(true);
//     QPushButton *ptr = ui.buttonBox->button(QDialogButtonBox::Apply);
    if(f_applyButtonP != 0)
    {
      f_applyButtonP->setEnabled(true);
    }
    if(f_accptButtonP != 0 )
    {
      f_accptButtonP->setEnabled(true);
    }
//     ui.ObjectSelectionBox->setEditable(ui.ObjectSelectionBox->itemData(ui.ObjectSelectionBox->currentIndex()).toInt() != NewObjectSelectionBoxItem);
  }
  if(f_currentMO != 0)
  {
    getObjectData(f_currentMO);
  }
}

void MapObjectDialog::setObjectFile()
{
  QString objectfile = QFileDialog::getOpenFileName(this, tr("Object Image File"), QString(), tr("Images (*.png *.jpg *.gif *.svg)"));
  if(!objectfile.isEmpty())
  {
    ui.ObjectFilePathLabel->setText(objectfile);
    //     setObjectData();
    QPushButton *f_applyButtonP = ui.buttonBox->button(QDialogButtonBox::Apply);
    QPushButton *f_accptButtonP = ui.buttonBox->button(QDialogButtonBox::Ok);
    if(f_applyButtonP != 0 )
    {
      f_applyButtonP->setEnabled(true);
    }
    if(f_accptButtonP != 0 )
    {
      f_accptButtonP->setEnabled(true);
    }
  }
}


void MapObjectDialog::buttonClick(QAbstractButton *a_clickedButton)
{
  #ifdef DEBUG_MAPOBJECTDIALOG
  qWarning() << "void MapObjectDialog::applyChanges(QAbstractButton *a_clickedButton) ...";
  #endif
  if(ui.buttonBox->buttonRole(a_clickedButton) == QDialogButtonBox::ApplyRole || ui.buttonBox->buttonRole(a_clickedButton) == QDialogButtonBox::AcceptRole )	// if Apply-Button was clicked
  {
    #ifdef DEBUG_MAPOBJECTDIALOG
//     qWarning() << "void MapObjectDialog::applyChanges(QAbstractButton *a_clickedButton) + Accept/OK ...";
    #endif
    MapObject *f_currentMO = 0;
    
    bool f_newObject = (ui.ObjectSelectionBox->itemData(ui.ObjectSelectionBox->currentIndex()).toInt() == NewObjectSelectionBoxItem);
    if(f_newObject)
    {
      #ifdef DEBUG_MAPOBJECTDIALOG
//       qWarning() << "New Object";
      #endif
      f_currentMO = &m_newMapObject;
    }
    else
    {
      f_currentMO = m_MapObjects[ui.ObjectSelectionBox->itemData(ui.ObjectSelectionBox->currentIndex()).toInt()];
    }
    if(f_currentMO != 0)
    {
      #ifdef DEBUG_MAPOBJECTDIALOG
//       qWarning() << "    if(f_currentMO != 0)";
      #endif
      m_lockItemData = false;
      setObjectData(f_currentMO);
      if(f_newObject)
      {
	#ifdef DEBUG_MAPOBJECTDIALOG
 	qWarning() << "emit newMapObject(f_currentMO);" << f_currentMO->imagepath() << f_currentMO->id();
	#endif
	emit newMapObjectCreated(f_currentMO);
	ui.ObjectSelectionBox->setItemData(ui.ObjectSelectionBox->currentIndex(), QVariant(f_currentMO->id()));
      }
    }
  }
  #ifdef DEBUG_MAPOBJECTDIALOG
//   qWarning() << "End of \"void MapObjectDialog::applyChanges(QAbstractButton *a_clickedButton)\"";
  #endif
}

void MapObjectDialog::setObjectData(MapObject *a_mapobject)
{
  #ifdef DEBUG_MAPOBJECTDIALOG
  qWarning() << "void MapObjectDialog::setObjectData(MapObject *a_mapobject)";
  #endif
  if(!m_lockItemData)
  {
    QString f_oldimagepath(a_mapobject->imagepath());
    
    a_mapobject->setImagepath(ui.ObjectFilePathLabel->text());
    
    if(!ui.ObjectSelectionBox->currentText().isEmpty())
      a_mapobject->setName(ui.ObjectSelectionBox->currentText());
    else if(a_mapobject->name().isEmpty() && !a_mapobject->imagepath().isEmpty())
      a_mapobject->setName(a_mapobject->imagepath());
    else
      a_mapobject->setName(QString::number(a_mapobject->id()));
    a_mapobject->setRole(ui.ObjectRoleBox->itemData(ui.ObjectRoleBox->currentIndex()).toInt());
    a_mapobject->setTooltip(ui.TooltipLineEdit->text());
    a_mapobject->setPosition(QPoint(ui.XSpinBox->value(), ui.YSpinBox->value()));
    a_mapobject->setZValue(ui.ZSpinBox->value());
    
    emit objectDataChanged(a_mapobject->id());
    if(f_oldimagepath != a_mapobject->imagepath())
    {
      emit objectImageChanged(a_mapobject->id());
    }
  }
}

void MapObjectDialog::getObjectData(const MapObject *a_mapobject)
{
  bool f_oldLockState(m_lockItemData);
  m_lockItemData = true;
  ui.ObjectSelectionBox->setItemText(ui.ObjectSelectionBox->currentIndex(), a_mapobject->name());
  ui.ObjectFilePathLabel->setText(a_mapobject->imagepath());
  ui.ObjectRoleBox->setCurrentIndex(ui.ObjectRoleBox->findData(a_mapobject->role()));

//   ui.ObjectRoleBox->findItemData(ui.ObjectRoleBox->currentIndex(), QVariant(a_mapobject->role()));
  ui.TooltipLineEdit->setText(a_mapobject->tooltip());
  ui.XSpinBox->setValue(a_mapobject->position().x());
  ui.YSpinBox->setValue(a_mapobject->position().y());
  ui.ZSpinBox->setValue(a_mapobject->zValue());
  m_lockItemData = f_oldLockState;
}


void MapFrame::objectDialog(const QPoint &a_point)
{
  QList<QGraphicsItem*> f_ol = scene()->items(a_point);	// function_objectlist
  
  if(m_ObjectDialog != 0)
  {
    delete m_ObjectDialog;
    m_ObjectDialog = 0;
  }
  
  m_ObjectDialog = new MapObjectDialog(map(), a_point, f_ol, m_currentGraphicsItem);
//   m_ObjectDialog->exec();
  m_ObjectDialog->show();
  connect(m_ObjectDialog, SIGNAL(finished(int)), this, SLOT(closeObjectDialog(int)));
  connect(m_ObjectDialog, SIGNAL(newMapObjectCreated(MapObject *)), this, SLOT(addObject(MapObject*)));
  connect(m_ObjectDialog, SIGNAL(objectDataChanged(int)), this, SLOT(updateObject(int)));
  connect(m_ObjectDialog, SIGNAL(objectImageChanged(int)), this, SLOT(updateObjectImage(int)));
//     connect(m_ObjectDialog, SIGNAL(newMapObjectCreated()), this, SLOT(addObject()));

}

void MapFrame::addObject()
{
  addObject(m_ObjectDialog->newMapObject());
}


void MapFrame::closeObjectDialog(int a_result)
{
  if(a_result == 1)
  {
    printf("void MapFrame::closeObjectDialog(int a_result)");
  }
  
  m_ObjectDialog->deleteLater();
  m_ObjectDialog = 0;
  
}

