/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel and Joerg Thalheim  *
 *   oh.devs@googlemail.com   *
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
 
#include "mainwindow.h"

#include <QtGui/QMenuBar>
#include <QtGui/QAction>
#include <QtGui/QStatusBar>

#include <QtGui/QToolBar>

#include <QtGui/QHBoxLayout>
//#include <QtGui/QVBoxLayout>
//#include <QtGui/QFrame>
//#include <QtGui/QFileDialog>

//#include <QtCore/QDir>
#include <QtCore/QDebug>
//#include <QtCore/QSignalMapper>
#include <QtCore/QTimer>
#include <QtCore/QDir>
#include <QtCore/QFile>

MainWindow::MainWindow()
 {
	QTimer *autoSaveTimer = new QTimer(this);
	autoSaveTimer->start(10000);
 	existingMapFile = false;

	QWidget *zentralwidget = new QWidget();
	setCentralWidget(zentralwidget);


	SideBar = new SideBarClass();
	SideBar->setParent(zentralwidget);
// 	SideBar->setFixedWidth(100);

	MapView = new MapFrame();
	MapView->setParent(zentralwidget); 
	MapView->initMap();
//	MapView->setStatusTip("");
	
	SideBar->itemTyp->addItems(MapView->MapTypeEntries);
// 	SideBar->comboBoxObjectEntries = MapView->itemfktList;

	QHBoxLayout *layout = new QHBoxLayout(zentralwidget);
	layout->setMargin(5); 
     
	layout->addWidget(MapView);
	layout->addWidget(SideBar);

	zentralwidget->setLayout(layout);
	
	createActions();
	createMenus();
	
	QToolBar *tb = addToolBar(tr("Maintoolbar"));
	tb->addAction(newAct);
	tb->addAction(openAct);


	QString message = tr("A context menu is available by right-clicking");
	statusBar()->showMessage(message);

	setWindowTitle(tr("Karteneditor"));
	setMinimumSize(480, 360);
	resize(800, 600);
 
	connect(MapView, SIGNAL(newObjectCreated(QGraphicsItem *)), this, SLOT(addNewObjectToList(QGraphicsItem*)));
	connect(SideBar->itemListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(updateItemList(int)));
	connect(SideBar->itemTyp, SIGNAL(activated(QString)), this, SLOT(typeComboBoxHandler(QString)));
	connect(SideBar->selectFileButton, SIGNAL(clicked()), this, SLOT(sideBar_SelectFile()));
	connect(SideBar->editToolTip, SIGNAL(textEdited(QString)), this, SLOT(lineEditHandler(QString)));
 	connect(SideBar->XBox, SIGNAL(valueChanged(int)), this, SLOT(spinboxHandler()));
	connect(SideBar->YBox, SIGNAL(valueChanged(int)), this, SLOT(spinboxHandler()));
// 	connect(SideBar->XBox, SIGNAL(editingFinished()), this, SLOT(spinboxHandler()));
// 	connect(SideBar->YBox, SIGNAL(editingFinished()), this, SLOT(spinboxHandler()));
	connect(SideBar->ZBox, SIGNAL(valueChanged(double)), this, SLOT(spinboxHandler()));
	
	connect(MapView, SIGNAL(objectSelected(QGraphicsItem*)), this, SLOT(markListItem(QGraphicsItem*)));
	connect(MapView, SIGNAL(objectMoved()), this, SLOT(updateSpinbox()));
	
	connect(MapView, SIGNAL(SIG_deleteObject()), this, SLOT(deleteCurrentObject()));
	connect(SideBar, SIGNAL(SIG_deleteObject()), this, SLOT(deleteCurrentObject()));
	SideBar->itemListWidget->setCurrentRow(0);

	connect(autoSaveTimer, SIGNAL(timeout()), this, SLOT(autoSave()));
	loadAutoSaveMap();
}

void MainWindow::createActions()
 {
  newAct = new QAction(tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new map"));
  newAct->setIcon(QIcon(":.img/icon_new_map_01.png"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newMap()));

  openAct = new QAction(tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing map"));
  openAct->setIcon(QIcon(":.img/icon_open_map_03.png"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(open()));

  saveAct = new QAction(tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save the map with the current name to disk"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(saveHandler()));

  saveAsAct = new QAction(tr("&Save As ..."), this);
  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  saveAsAct->setStatusTip(tr("Save the map to disk"));
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveHandler()));
  
  autoSaveAct = new QAction(tr("Save Autosave"), this);
  autoSaveAct->setStatusTip(tr("Saves the Map as Autosaved Map"));
  connect(autoSaveAct, SIGNAL(triggered()), this, SLOT(autoSave()));
  
  loadAutoSaveAct = new QAction(tr("Load Autosave"), this);
  loadAutoSaveAct->setStatusTip(tr("Loads the Autosaved Map"));
  connect(loadAutoSaveAct, SIGNAL(triggered()), this, SLOT(loadAutoSaveMap()));

  quitAct = new QAction(tr("&Exit"), this);
  quitAct->setShortcut(tr("Ctrl+Q"));
  quitAct->setStatusTip(tr("Close the application"));
  quitAct->setIcon(QIcon(":.img/icon_quit_02.png"));
  connect(quitAct, SIGNAL(triggered()), this, SLOT(close()));
  
  newObjectAct = new QAction(tr("&New Object"), this);
  newObjectAct->setStatusTip(tr("Open Dialog for creating a new Object"));
 // QSignalMapper newObjectMapper;
//   connect(newObjectAct, SIGNAL(triggered()), &newObjectMapper, SLOT(map()));
//   newObjectMapper.setMapping(newObjectAct, QString("100,100"));
//   connect(&newObjectMapper, SIGNAL(mapped(QString)), MapView, SLOT(newObjectDialog(QPoint)));
  connect(newObjectAct, SIGNAL(triggered()), MapView, SLOT(newObjectDialog_ext()));

  rmcurrentObjectAct = new QAction(tr("&Remove current Object"), this);
  rmcurrentObjectAct->setStatusTip(tr("Removes the currently active Object"));
  connect(rmcurrentObjectAct, SIGNAL(triggered()), this, SLOT(deleteCurrentObject()));
 }
 
 void MainWindow::newMap()
 {
	qWarning() << "MainWindow::newMap()";
	SideBar->itemListWidget->clear();
	SideBar->itemListWidget->addItems(SideBar->staticListEntries);
	SideBar->XBox->setValue(1000);
	SideBar->YBox->setValue(1000);
	SideBar->ZBox->setValue(0);
	SideBar->editToolTip->setText(QString());
        SideBar->itemListWidget->setCurrentRow(1);
	MapView->newMap();
// 	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
// 	disconnect(saveAct, SIGNAL(triggered()), this, SLOT(savef()));
	existingMapFile = false;

 }

//  void MainWindow::newFile()
//  {
// 
// 	connect(saveAct, SIGNAL(triggered()), this, SLOT(save()));
// 	disconnect(saveAct, SIGNAL(triggered()), this, SLOT(savef()));
// 	
//  }

void MainWindow::loadAutoSaveMap()
{
	if(QFile(QDir().home().absolutePath().append("/.OpenHanse/automapsave.ohm")).exists())
	{
	MapView->fd_filename = QDir().home().absolutePath().append("/.OpenHanse/automapsave.ohm");
	openMap();
	existingMapFile = false;
	}
}

 void MainWindow::open()
 {
 MapView->fileDialog(NameFilters::Map);
connect(MapView->fd, SIGNAL(accepted()), this, SLOT(openMap()));
 }
 
 void MainWindow::openMap()
 {
 existingMapFile = true;
 mapfilename = MapView->fd_filename;
 qWarning() << MapView->fd_filename;
 SideBar->itemListWidget->clear();
 SideBar->itemListWidget->addItems(SideBar->staticListEntries);

 MapView->loadMap(MapView->fd_filename);
 SideBar->itemListWidget->setCurrentRow(0);
 }
 
 void MainWindow::saveHandler()
 {
 autoSave();
 qWarning() << "saveHandler()" << mapfilename;
 if(existingMapFile)
 {
 savef();
 return;
 }
 
 if(!existingMapFile)
 {
 save();
 return;
 }
 
 }
 
 void MainWindow::autoSave()
 {
 if(!autoSaved)
 {
 QDir dir = QDir().home();
 if(!dir.cd(".OpenHanse"))
 {
	if(dir.mkdir(".OpenHanse"))
	{
		dir.cd(".OpenHanse");
	}
	else	return;
 }
 QString savepath = dir.absolutePath();
 qWarning() << savepath;
 savepath.append("/automapsave.ohm");
 MapView->saveMap(savepath);
 autoSaved = true;
 }

 }

 void MainWindow::save()
 {
 MapView->fileDialog(NameFilters::Map|NameFilters::Save);

  //::getSaveFileName(this, tr("Save File"), home() ,;

 //MapView->saveMap(MapView->filename);
 connect(MapView->fd, SIGNAL(accepted()), this, SLOT(savef()));
 
 }
 
 void MainWindow::savef()
 {
// 	disconnect(saveAct, SIGN/**/AL(triggered()), this, SLOT(save()));
// 	connect(saveAct, SIGNAL(triggered()), this, SLOT(savef()));

	disconnect(MapView->fd, SIGNAL(accepted()), this, SLOT(savef()));
	if(!existingMapFile)
	{
	mapfilename = MapView->fd_filename;
	}
	setWindowTitle(tr("Karteneditor: ").append(mapfilename));
	MapView->saveMap(mapfilename);
 }

void MainWindow::createMenus()
 {
  fileMenu = menuBar()->addMenu(tr("&File"));
  fileMenu->addAction(newAct);
  fileMenu->addAction(openAct);
  fileMenu->addAction(saveAct);
  fileMenu->addAction(saveAsAct);
  fileMenu->addSeparator();
  fileMenu->addAction(loadAutoSaveAct);
  fileMenu->addAction(autoSaveAct);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAct);

  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(newObjectAct);
  editMenu->addAction(rmcurrentObjectAct);

  helpMenu = menuBar()->addMenu(tr("&Help"));
 }
 
void MainWindow::addNewObjectToList(QGraphicsItem *newObject)
{
autoSaved = false;

// SideBar->MapEntries << entry->data(0).toString().append(entry->data(1).toString()).append(entry->data(2).toString());
qWarning() << "MainWindow::addNewObjectToList(QGraphicsItem *newObject)";

// SideBar->itemListWidget->addItem(newObject->data(17).toString().append("; ").append(newObject->data(1).toString()).append(newObject->data(2).toString().right(14).left(10)));
 
 SideBar->itemListWidget->addItem(newObject->data(18).toString());
 
SideBar->itemListWidget->setCurrentRow(SideBar->itemListWidget->count() - 1);
 itemAdded = true;
}

void MainWindow::updateItemList(int selectedItemRow)
{
qWarning() << "MainWindow::updateItemList(int selectedItemRow)" << selectedItemRow;

	if(selectedItemRow == 0)	//mapbackgroundimagefilepath !-! allgemeine Mapprops
	{
		if(MapView->bgi_filename.size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->bgi_filename).right(39).prepend("..."));
		}
		else
		SideBar->fileView->setText(MapView->bgi_filename);
		
		//SideBar->editToolTip->setToolTip(tr("Comment"));
// 		SideBar->editToolTip->setEnabled(false);
// 		SideBar->editToolTip->setText(QString());
		if(MapView->maptyp == MapType::land_city || MapView->maptyp == MapType::coast_city)
		{
			SideBar->editToolTip->setEnabled(true);
			SideBar->editToolTip->setText(MapView->cityname);
		}
		else
		{
			SideBar->editToolTip->setEnabled(false);
			SideBar->editToolTip->setText(QString());
		}
		
		SideBar->selectFileButton->setEnabled(true);
//  		SideBar->selectFileButton->setDefault(true);

		SideBar->itemTyp->setEnabled(true);
		if(!SideBar->CB_mapprops)
		{
			SideBar->itemTyp->clear();
			SideBar->itemTyp->addItems(MapView->MapTypeEntries);
			SideBar->CB_mapprops = true;
			int MapTypeRow = MapView->maptyp;
			qWarning() << MapTypeRow;
// 			SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->findText(MapView->maptypename));
			SideBar->itemTyp->setCurrentIndex(MapTypeRow);
		}

		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(true);
		SideBar->XBox->setEnabled(true);
		SideBar->XBox->setValue(MapView->mapSize.width());
		SideBar->YBox->setValue(MapView->mapSize.height());
	}

// 	if(selectedItemRow == 0)	//city-name
// 	{
// 		SideBar->itemTyp->setEnabled(false);
// 		SideBar->editToolTip->setEnabled(true);
// 		SideBar->selectFileButton->setEnabled(false);
// 		if(MapView->cityname.size() > 43)
// 		{
// 			SideBar->fileView->setText(QString(MapView->cityname).right(39).prepend("..."));
// 		}
// 		else
// 			SideBar->editToolTip->setText(MapView->cityname);
// 		SideBar->fileView->setText(QString());
// 		SideBar->ZBox->setEnabled(false);
// 		SideBar->YBox->setEnabled(true);
// 		SideBar->XBox->setEnabled(true);
// 		SideBar->XBox->setValue(MapView->mapSize.width());
// 		SideBar->YBox->setValue(MapView->mapSize.height());
// 	}

	if(selectedItemRow == 1)	//Map: North
	{
		SideBar->itemTyp->setEnabled(false);
		SideBar->editToolTip->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		if(MapView->mapnorth.size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->mapnorth).right(39).prepend("..."));
		}
		else
		SideBar->fileView->setText(MapView->mapnorth);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);

// 		SideBar->XBox->setValue(MapView->mapSize.width());
// 		SideBar->YBox->setValue(MapView->mapSize.height());
	}
	if(selectedItemRow == 2)	//Map: West
	{
		SideBar->itemTyp->setEnabled(false);
		SideBar->editToolTip->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		if(MapView->mapwest.size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->mapwest).right(39).prepend("..."));
		}
		else
		SideBar->fileView->setText(MapView->mapwest);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);
// 		SideBar->XBox->setValue(MapView->mapSize.width());
// 		SideBar->YBox->setValue(MapView->mapSize.height());
	}
	if(selectedItemRow == 3)	//Map: South
	{
		SideBar->itemTyp->setEnabled(false);
		SideBar->editToolTip->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		if(MapView->mapsouth.size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->mapsouth).right(39).prepend("..."));
		}
		else
		SideBar->fileView->setText(MapView->mapsouth);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);
// 		SideBar->XBox->setValue(MapView->mapSize.width());
// 		SideBar->YBox->setValue(MapView->mapSize.height());
	}
	if(selectedItemRow == 4)	//Map: East
	{
		SideBar->itemTyp->setEnabled(false);
		SideBar->editToolTip->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		if(MapView->mapeast.size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->mapeast).right(39).prepend("..."));
		}
		else
		SideBar->fileView->setText(MapView->mapeast);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);
// 		SideBar->XBox->setValue(MapView->mapSize.width());
// 		SideBar->YBox->setValue(MapView->mapSize.height());
	}
	if(selectedItemRow > 4)		//MapObject
	{
		SideBar->editToolTip->setEnabled(true);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->YBox->setEnabled(true);
		SideBar->XBox->setEnabled(true);
		SideBar->ZBox->setEnabled(true);
		SideBar->itemTyp->setEnabled(true);
		if(SideBar->CB_mapprops)
		{
			SideBar->itemTyp->clear();
			SideBar->itemTyp->addItems(MapView->ObjectTypeEntries);
			SideBar->CB_mapprops = false;
		}
// 		QGraphicsItem *blah = new QGraphicsItem();
// 		MapView->activeItem = blah:
 		MapView->activeItem = MapView->itemMapList.value(SideBar->itemListWidget->currentItem()->text());
		
// 		MapView->activeItem->setPos(10000,1000);
// 				qWarning() << "DebugMeldung";
		
		SideBar->XBox->setValue(MapView->activeItem->x());
		SideBar->YBox->setValue(MapView->activeItem->y());

		SideBar->ZBox->setValue(MapView->activeItem->zValue());
		
		
		int obj_id = MapView->activeItem->data(0).toInt();
		qWarning() << obj_id;
		if(obj_id < 100)
			SideBar->itemTyp->setCurrentIndex(obj_id);
		if(obj_id >= 100 && obj_id < 500)
			SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->count() - 3);
		if(obj_id >= 500 && obj_id < 1000)
			SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->count() - 2);
		if(obj_id >= 1000)
			SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->count() - 1);
		SideBar->editToolTip->setText(MapView->activeItem->data(1).toString());
		
		
		if(MapView->activeItem->data(2).toString().size() > 43)
		{
			SideBar->fileView->setText(QString(MapView->activeItem->data(2).toString()).right(39).prepend("..."));
			
		}
		else
		SideBar->fileView->setText(MapView->activeItem->data(2).toString());
	}

}

void MainWindow::sideBar_SelectFile()
{
qWarning() << "MainWindow::sideBar_SelectFile()";
	if(SideBar->itemListWidget->currentRow() == 0 || SideBar->itemListWidget->currentRow() > 4)
	{
		MapView->fileDialog(NameFilters::Img);
	}
	else
	{
		MapView->fileDialog(NameFilters::Map);
	}
	connect(MapView->fd, SIGNAL(accepted()), this, SLOT(sideBar_FileSelected()));
}

void MainWindow::sideBar_FileSelected()
{
autoSaved = false;

qWarning() << "MainWindow::sideBar_FileSelected()";
	disconnect(MapView->fd, SIGNAL(accepted()), this, SLOT(sideBar_FileSelected()));
	SideBar->fileView->setText(MapView->fd_filename);
	if(SideBar->itemListWidget->currentRow() > 4)
	{
// // 		MapView->itemList.value(SideBar->itemListWidget->currentRow() - 6)->setData(1, QVariant(MapView->fd_filename));

// 		MapView->itemMapList.value(SideBar->itemListWidget->currentItem()->text())->setData(1, QVariant(MapView->fd_filename));

// // 		QGraphicsPixmapItem *blah = MapView->itemList.value(SideBar->itemListWidget->currentRow() - 6);
		MapView->activeItem->setData(1, QVariant(MapView->fd_filename));
		
		
//  		MapView->itemList.value(SideBar->itemListWidget->currentRow() - 6)->setPixmap(QPixmap(MapView->fd_filename));
	}
	if(SideBar->itemListWidget->currentRow() == 0)
	{
		MapView->bgi_filename = MapView->fd_filename;
		MapView->szene->setBackgroundBrush(QBrush(QImage(MapView->bgi_filename)));
	}
	if(SideBar->itemListWidget->currentRow() == 1)
	{
		MapView->mapnorth = MapView->fd_filename;
	}
	if(SideBar->itemListWidget->currentRow() == 2)
	{
		MapView->mapwest = MapView->fd_filename;
	}
	if(SideBar->itemListWidget->currentRow() == 3)
	{
		MapView->mapsouth = MapView->fd_filename;
	}
	if(SideBar->itemListWidget->currentRow() == 4)
	{
		MapView->mapeast = MapView->fd_filename;
	}

}

void MainWindow::spinboxHandler()
{
autoSaved = false;
// qWarning() << "MainWindow::spinboxHandler()";

if(SideBar->XBox->hasFocus() || SideBar->YBox->hasFocus() || SideBar->ZBox->hasFocus())
{
	if(SideBar->itemListWidget->currentRow() > 4 /*&& !MapView->itemGrabbed && !itemAdded*/)
	{
// 		MapView->itemMapList.value(SideBar->itemListWidget->currentItem()->text())->setPos(SideBar->XBox->value(), SideBar->YBox->value());
// 		MapView->itemMapList.value(SideBar->itemListWidget->currentRow() - 6)->setZValue(SideBar->ZBox->value());
		MapView->activeItem->setPos(SideBar->XBox->value(), SideBar->YBox->value());
		MapView->activeItem->setZValue(SideBar->ZBox->value());
	}
	if(SideBar->itemListWidget->currentRow() <= 4)
	{
		MapView->mapSize = QSize(SideBar->XBox->value(), SideBar->YBox->value());
		MapView->setSceneRect(0, 0, MapView->mapSize.width(), MapView->mapSize.height());
	}
	if(itemAdded)
		itemAdded = false;
}
}

void MainWindow::updateSpinbox()
{
autoSaved = false;
SideBar->XBox->setValue(MapView->activeItem->x());
SideBar->YBox->setValue(MapView->activeItem->y());
}


void MainWindow::typeComboBoxHandler(QString typ)
{
qWarning() << "MainWindow::setObjTyp(QString typ)" << typ;
autoSaved = false;
	if(!SideBar->CB_mapprops)
	{
		MapView->setObjectType(typ);
		MapView->activeItem->setData(0, QVariant(MapView->object_typ));
		qWarning() << MapView->object_typ << MapView->activeItem->data(0).toInt();
	}
	else
	{
		MapView->setMapType(typ);
		
		if(MapView->maptyp == MapType::land_city || MapView->maptyp == MapType::coast_city)
		{
			SideBar->editToolTip->setEnabled(true);
			SideBar->editToolTip->setText(MapView->cityname);
		}
		else
		{
			SideBar->editToolTip->setEnabled(false);
			SideBar->editToolTip->setText(QString());
		}
	}
}

void MainWindow::lineEditHandler(QString text)
{
autoSaved = false;
	if(SideBar->itemListWidget->currentRow() == 0 )
	{
	MapView->cityname = text;
	}
	if(SideBar->itemListWidget->currentRow() > 4)
	{
	MapView->activeItem->setData(1, QVariant(text));
	MapView->activeItem->setToolTip(text);
	}
}


void MainWindow::markListItem(QGraphicsItem *selectedItem)
{
// for(int i = 0; SideBar->itemListWidget->item(i) != 0; i++)
// {
// }
// QList <QListWidgetItem*> matchingListEntry =  SideBar->itemListWidget->findItems(selectedItem->data(0).toString(), Qt::MatchContains);
qWarning() << "MainWindow::markListItem(QGraphicsItem *selectedItem)";
SideBar->itemListWidget->setCurrentItem(SideBar->itemListWidget->findItems(selectedItem->data(18).toString(), Qt::MatchStartsWith).first());

}


void MainWindow::deleteCurrentObject()
{
autoSaved = false;
int currentListRow = SideBar->itemListWidget->currentRow();

if(!MapView->itemMapList.isEmpty() && currentListRow > 4)
{
qWarning() << "MainWindow::deleteCurrentObject()";
//int itemID = MapView->activeItem->data(17).toInt();

MapView->itemMapList.remove(SideBar->itemListWidget->currentItem()->text());


// SideBar->itemListWidget->removeItemWidget(SideBar->itemListWidget->item(currentListRow));
SideBar->itemListWidget->takeItem(currentListRow);
SideBar->itemListWidget->setCurrentRow(0);


// qWarning() << MapView->itemList.size();

MapView->szene->removeItem(MapView->activeItem);

 delete MapView->activeItem;
 
// MapView->szene->items

}
else
setStatusTip(tr("Error: There aren't any objects to delete!"));
}
