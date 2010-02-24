/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel and Joerg Thalheim           *
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

#include "sidebar.h"
#include "mainwindow.h"
#include "mapframe.h"
#include "settings.h"
#include "mapobject.h"

#include <QtGui/QVBoxLayout>
#include <QtGui/QHBoxLayout>
#include <QtGui/QFrame>
#include <QtGui/QKeyEvent>

#include <QtGui/QListWidget>
#include <QtGui/QLineEdit>
#include <QtGui/QLabel>
#include <QtGui/QPushButton>
#include <QtGui/QSpinBox>
#include <QtGui/QComboBox>

#include <QtGui/QFileDialog>

#include <QtGui/QGraphicsItem>

SideBarClass::SideBarClass(const MainWindow *parentWindow)
{
  m_parent = parentWindow;
  staticListEntries << /*tr("Stadtname") << tr("Maphintergrund")*/ tr("Mapeigenschaften") << tr("Nordmap") << tr("Westmap") << tr("Suedmap") << tr("Ostmap");
  
  
  
  QVBoxLayout *SideBarLayout = new QVBoxLayout(this);
  
  //initMapEntriesList();
  itemListWidget = new QListWidget(this);
  itemListWidget->setSelectionMode(QAbstractItemView::SingleSelection);
  itemListWidget->addItems(staticListEntries);
  //itemListWidget->addItems(MapEntries);
  SideBarLayout->addWidget(itemListWidget);
  
  QFrame *line = new QFrame(this);
  line->setFrameShape(QFrame::HLine);
  SideBarLayout->addWidget(line);
  
  itemTyp = new QComboBox(this);
  SideBarLayout->addWidget(itemTyp);
  
  nameLineEdit = new QLineEdit(this);
  nameLineEdit->setMaxLength(30);
  SideBarLayout->addWidget(nameLineEdit);
  
  editToolTip = new QLineEdit(this);
  editToolTip->setMaxLength(30);
  SideBarLayout->addWidget(editToolTip );
  
  QWidget *wid = new QWidget(this);
  SideBarLayout->addWidget(wid);
  
  QHBoxLayout *objectFileLayout = new QHBoxLayout(wid);
  
  fileView = new QLineEdit(this);
  // 	fileView->setFrameShape(QFrame::Box);
  // 	fileView->setEnabled(false);
  fileView->setReadOnly(true);
  objectFileLayout->addWidget(fileView);
  
  selectFileButton = new QPushButton ("...", this);
  objectFileLayout->addWidget(selectFileButton);
  
  objectFileLayout->setStretch(0,5);
  
  QWidget *wid2 = new QWidget(this);
  SideBarLayout->addWidget(wid2);
  
  QHBoxLayout *objectFileLayout2 = new QHBoxLayout(wid2);
  
  XBox = new QSpinBox(wid2);
  objectFileLayout2->addWidget(XBox);
  XBox->setRange(0,16383);
  XBox->setValue(1000);
  XBox->setToolTip(tr("X-Position des Objekts bzw. Mapbreite"));
  
  YBox = new QSpinBox(wid2);
  objectFileLayout2->addWidget(YBox);
  YBox->setRange(0,16383);
  YBox->setValue(1000);
  YBox->setToolTip(tr("Y-Position des Objekts bzw. Maphoehe"));
  
  ZBox = new QDoubleSpinBox(wid2);
  objectFileLayout2->addWidget(ZBox);
  ZBox->setRange(0,10);
  ZBox->setValue(1);
  ZBox->setDecimals(3);
  ZBox->setToolTip(tr("Hoehe des Objekts"));
  
  
  QString townhalllabel = tr("Rathaus"),
  marketlabel = tr("Markt"),
  churchlabel = tr("Kirche"),
  portlabel = tr("Hafen"),
  officelabel = tr("Kontor"),
  banklabel = tr("Darlehensgeber"),
  tavernlabel = tr("Kneipe"),
  land_breakelabel = tr("Landflaechen"),
  land_damagelabel = tr("\"boese\" Untiefen"), // Objekte, bei denen eine Kollision mit dem Schiff Schaden am Schiff verursacht.
  mapdecorationlabel = tr("Mapdeko");
  
  functionLabels.insert(Building::Townhall, townhalllabel);
  functionLabels.insert(Building::Market, marketlabel);
  functionLabels.insert(Building::Church, churchlabel);
  functionLabels.insert(Building::Port, portlabel);
  functionLabels.insert(Building::Kontor, officelabel);
  functionLabels.insert(Building::Bank, banklabel);
  functionLabels.insert(Building::Tavern, tavernlabel);
  functionLabels.insert(MapObject::Land_breake, land_breakelabel);
  functionLabels.insert(MapObject::Land_damage, land_damagelabel);
  functionLabels.insert(MapObject::Mapdecoration, mapdecorationlabel);
  
  QString mt_sea = tr("See/Meer"),
  mt_coast = tr("Kueste"),
  mt_land = tr("Land"),
  mt_coast_city = tr("Kueste (Stadt)"),
  mt_land_city = tr("Land (Stadt)");
  
  maptypeLabels.insert(Map::Sea, mt_sea);
  maptypeLabels.insert(Map::Coast, mt_coast);
  maptypeLabels.insert(Map::Land, mt_land);
  maptypeLabels.insert(Map::Coast ^ Map::Citymap, mt_coast_city);
  maptypeLabels.insert(Map::Land ^ Map::Citymap, mt_land_city);
  
  QHashIterator <int, QString> it(maptypelabels());
  while(it.hasNext())
  {
    it.next();
    itemTyp->addItem(it.value(), it.key());
    // 		SideBar->itemTyp->addItems(MapView->MapTypeEntries);
  }
  //	SideBarLayout->addLayout(objectFileLayout);
  
  connect(selectFileButton, SIGNAL(clicked()), this, SLOT(selectFile()));
  connect(fileView, SIGNAL(textChanged(QString)), this, SIGNAL(dataChanged()));  
  connect(nameLineEdit, SIGNAL(textEdited(QString)), this,  SIGNAL(dataChanged()));
  connect(itemTyp, SIGNAL(activated(int)), this,  SIGNAL(dataChanged()));
  connect(selectFileButton, SIGNAL(clicked()), this,  SIGNAL(dataChanged()));
  connect(editToolTip, SIGNAL(textEdited(QString)), this,  SIGNAL(dataChanged()));
  connect(XBox, SIGNAL(valueChanged(int)), this,  SIGNAL(dataChanged()));
  connect(YBox, SIGNAL(valueChanged(int)), this,  SIGNAL(dataChanged()));
  connect(ZBox, SIGNAL(valueChanged(double)), this,  SIGNAL(dataChanged()));
  
}


void SideBarClass::addStaticListItems()
{
  QListWidgetItem *f_lwi;
//   for(QStringList::iterator it = staticListEntries.begin(); it != staticListEntries.end(); ++it)
//   {
//     ;
    for(int f_identifier = 1; f_identifier <= 5; f_identifier ++)
    {
//       if(~identifier == 
      f_lwi = new QListWidgetItem(staticListEntries.value(f_identifier), itemListWidget);
      f_lwi->setData(Qt::UserRole, QVariant((~f_identifier)));
    }
    
//   }
}


QString SideBarClass::maptypelabel(int key) const
{
  return maptypeLabels.value(key);
}

// void SideBarClass::initMapEntriesList()
//  {
  // 	//MapEntries.clear();
//  }

void SideBarClass::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Delete)
  {
    emit SIG_deleteObject();
  }
}

void SideBarClass::fillList(const QList<MapObject> &itemlist)
{
  bool found;
  QListWidgetItem *lwi;
  QList<QListWidgetItem*> lwilist;
  
  //   foreach(item, itemlist)
  
  for(QList<MapObject>::const_iterator moit = itemlist.begin(); moit != itemlist.end(); ++moit)
  {
    found = false;
    lwilist = itemListWidget->findItems(QString::number(moit->id()), Qt::MatchStartsWith);
    foreach(lwi, lwilist)
    {
      if(lwi->data(Qt::UserRole).toInt() == moit->id())
      {
	found = true;
	lwi = lwilist.last();
      }
    }
    if(!found)
    {
      lwi = new QListWidgetItem(QString::number(moit->id()) + moit->name(), itemListWidget);
      lwi->setData(Qt::UserRole, moit->id());
    }
  }
}

void SideBarClass::fillList(QList<QGraphicsItem*> itemlist)		
/// obsolete: data isn't stored in the qgraphicsitems any more - it is stored in the MapObjects in the map-class
//add missing entrys of QGraphicsItems to the itemlist
{
  bool found;
  QGraphicsItem *item; 
  QListWidgetItem *lwi;
  QList<QListWidgetItem*> lwilist;
  
  foreach(item, itemlist)
  {
    found = false;
    lwilist = itemListWidget->findItems(item->data(MapFrame::Name).toString(), Qt::MatchExactly);
    foreach(lwi, lwilist)
    {
      if(lwi->data(Qt::UserRole).toInt() == item->data(MapFrame::ID).toInt())
      {
	found = true;
	lwi = lwilist.last();
      }
    }
    if(!found)
    {
      lwi = new QListWidgetItem(item->data(MapFrame::Name).toString(), itemListWidget);
      lwi->setData(Qt::UserRole, item->data(MapFrame::ID).toInt());
    }
    // = new QListWidgetItem(newObject->data(MapFrame::Name).toString(), SideBar->itemListWidget);
    //   lwi->setData(Qt::UserRole, newObject->data(MapFrame::ID).toInt());
    // 	SideBar->itemListWidget->addItem(newObject->data(MapFrame::Name));
    
    //   qWarning() << newObject->data(MapFrame::Name);
    
    // SideBar->itemListWidget->setCurrentRow(SideBar->itemListWidget->count() - 1);
  }
}


void SideBarClass::selectFile()
{
  Filetypes type;
  QString filepath;
  int currentRow = itemListWidget->currentRow();
  if(currentRow == 0)
  {
    type = Backgroundbrush;
  }
  else if(currentRow > 0 && currentRow < 5)
  {
    type = OpenHanseMap;
  }
  else
    type = ItemImage;
  
  switch (type)
  {
    case OpenHanseMap:
    {
      filepath = QFileDialog::getOpenFileName(this, tr("OH-Map File"), QString(), tr("OpenHanse maps (*.ohm)"));
      break;
    }
    case Backgroundbrush:
    {
      filepath = QFileDialog::getOpenFileName(this, tr("Background File"), QString(), tr("Images (*.png *.jpg *.gif)"));
      break;
    }
    case ItemImage:
    {
      filepath = QFileDialog::getOpenFileName(this, tr("Object Image File"), QString(), tr("Images (*.png *.jpg *.gif *.svg)"));
      break;
    }
    default:
      break;
  }
  fileView->setText(filepath);
}

int SideBarClass::currentRow() const
{
  return itemListWidget->currentRow();
}

int SideBarClass::currentRowData()
{
  return itemListWidget->currentItem()->data(Qt::UserRole).toInt();
}


