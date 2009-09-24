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
 #include "mapframe.h"
 #include "sidebar.h"
 
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
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
 #include <QtGui/QListWidget>
 #include <QtGui/QLineEdit>
 #include <QtGui/QLabel>
 #include <QtGui/QPushButton>
 #include <QtGui/QSpinBox>
 #include <QtGui/QComboBox>
 
 #include <QtGui/QMessageBox>

// #define _DEBUG_

MainWindow::MainWindow()
 {
	autoSaveTimer = new QTimer(this);
	#ifndef _DEBUG_
	autoSaveTimer->start(10000);
	#endif
 	existingMapFile = false;

	QWidget *zentralwidget = new QWidget(this);
	setCentralWidget(zentralwidget);


	SideBar = new SideBarClass(this);
	SideBar->setParent(zentralwidget);
// 	SideBar->setFixedWidth(100);

	MapView = new MapFrame(this);
	MapView->setParent(zentralwidget); 
// 	MapView->initMap();
//	MapView->setStatusTip("");
	
	QHashIterator <int, QString> it(SideBar->maptypelabels());
	while(it.hasNext())
	{
		it.next();
		SideBar->itemTyp->addItem(it.value(), it.key());
// 		SideBar->itemTyp->addItems(MapView->MapTypeEntries);
	}
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
	
	connect(SideBar->nameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(nameLineEditHandler(QString)));
	connect(SideBar->itemListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(updateItemList(int)));
	connect(SideBar->itemTyp, SIGNAL(activated(int)), this, SLOT(typeComboBoxHandler(int)));
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
  
  disableAutoSaveAct = new QAction(tr("Disable Autosaves"), this);
//   disableAutoSaveAct->setStatusTip(tr("Saves the Map as Autosaved Map"));
  connect(disableAutoSaveAct, SIGNAL(triggered()), this, SLOT(disableAutoSave()));
  
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
 
 else
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
 void MainWindow::disableAutoSave()
 {
 autoSaveTimer->stop();
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
  fileMenu->addAction(disableAutoSaveAct);
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
qWarning() << "MainWindow::addNewObjectToList(QGraphicsItem *newObject) :: ID:" << newObject->data(MapFrame::ID).toInt();

// SideBar->itemListWidget->addItem(newObject->data(17).toString().append("; ").append(newObject->data(1).toString()).append(newObject->data(2).toString().right(14).left(10)));
 QListWidgetItem *lwi = new QListWidgetItem(newObject->data(MapFrame::Name).toString(), SideBar->itemListWidget);
 lwi->setData(64, newObject->data(MapFrame::ID).toInt());
// 	SideBar->itemListWidget->addItem(newObject->data(MapFrame::Name));
	qWarning() << newObject->data(MapFrame::Name);
 
// SideBar->itemListWidget->setCurrentRow(SideBar->itemListWidget->count() - 1);
SideBar->itemListWidget->setCurrentItem(lwi);
 itemAdded = true;
}

void MainWindow::updateItemList(int selectedItemRow)
{
if(selectedItemRow >= 0)
{
qWarning() << "MainWindow::updateItemList(int selectedItemRow)" << selectedItemRow;
	switch(selectedItemRow)
	{
	case en_mapproperties:	//mapbackgroundimagefilepath !-! allgemeine Mapprops
	{
// 		if(MapView->bgi_filename.size() > 43)
// 		{
// 			SideBar->fileView->setText(QString(MapView->bgi_filename).right(39).prepend("..."));
// 		}
// 		else
		SideBar->fileView->setText(MapView->bgi_filename);
		SideBar->nameLineEdit->setEnabled(true);
		//SideBar->editToolTip->setToolTip(tr("Comment"));
// 		SideBar->editToolTip->setEnabled(false);
// 		SideBar->editToolTip->setText(QString());
		if(MapView->maptyp == Landcitymap || MapView->maptyp == Coastcitymap)
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
// 			SideBar->itemTyp->addItems(MapView->MapTypeEntries);
			
				QHashIterator <int, QString> it(SideBar->maptypelabels());
				while(it.hasNext())
				{
					it.next();
					SideBar->itemTyp->addItem(it.value(), it.key());
				}
			
			SideBar->CB_mapprops = true;
			int MapTypeRow = MapView->maptyp;
			qWarning() << MapTypeRow;
// 			SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->findText(MapView->maptypename));
			SideBar->itemTyp->setCurrentIndex(MapTypeRow);
		}
		SideBar->nameLineEdit->setText(MapView->mapname);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(true);
		SideBar->XBox->setEnabled(true);
		SideBar->XBox->setValue(MapView->mapSize.width());
		SideBar->YBox->setValue(MapView->mapSize.height());
		break;
	}

	case en_mapnorth:		//Map: North
	{
		SideBar->itemTyp->setEnabled(false);
		SideBar->editToolTip->setEnabled(false);
		SideBar->nameLineEdit->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		SideBar->nameLineEdit->setText(QString());
// 		if(MapView->mapnorth.size() > 43)
// 		{
// 			SideBar->fileView->setText(QString(MapView->mapnorth).right(39).prepend("..."));
// 		}
// 		else
		SideBar->fileView->setText(MapView->mapnorth);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);
		break;
	}
	case en_mapwest:	//Map: West
	{
		SideBar->itemTyp->setEnabled(false);
		SideBar->editToolTip->setEnabled(false);
		SideBar->nameLineEdit->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		SideBar->nameLineEdit->setText(QString());
// 		if(MapView->mapwest.size() > 43)
// 		{
// 			SideBar->fileView->setText(QString(MapView->mapwest).right(39).prepend("..."));
// 		}
// 		else
		SideBar->fileView->setText(MapView->mapwest);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);
		break;
	}
	case en_mapsouth:	//Map: South
	{
		SideBar->itemTyp->setEnabled(false);
		SideBar->editToolTip->setEnabled(false);
		SideBar->nameLineEdit->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		SideBar->nameLineEdit->setText(QString());

// 		if(MapView->mapsouth.size() > 43)
// 		{
// 			SideBar->fileView->setText(QString(MapView->mapsouth).right(39).prepend("..."));
// 		}
// 		else
		SideBar->fileView->setText(MapView->mapsouth);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);
		break;
	}
	case en_mapeast:	//Map: East
	{
		SideBar->itemTyp->setEnabled(false);
		SideBar->editToolTip->setEnabled(false);
		SideBar->nameLineEdit->setEnabled(false);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->editToolTip->setText(QString());
		SideBar->nameLineEdit->setText(QString());

// 		if(MapView->mapeast.size() > 43)
// 		{
// 			SideBar->fileView->setText(QString(MapView->mapeast).right(39).prepend("..."));
// 		}
// 		else
		SideBar->fileView->setText(MapView->mapeast);
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(false);
		SideBar->XBox->setEnabled(false);
		break;
	}
	default:		//MapObject
	{
		SideBar->editToolTip->setEnabled(true);
		SideBar->selectFileButton->setEnabled(true);
		SideBar->nameLineEdit->setEnabled(true);
		SideBar->YBox->setEnabled(true);
		SideBar->XBox->setEnabled(true);
		SideBar->ZBox->setEnabled(true);
		SideBar->itemTyp->setEnabled(true);
		
		if(SideBar->CB_mapprops)
		{
			SideBar->itemTyp->clear();
				QHashIterator <int, QString> it(SideBar->functionlabels());
				while(it.hasNext())
				{
					it.next();
					SideBar->itemTyp->addItem(it.value(), it.key());
				}
// 			SideBar->itemTyp->addItems(MapView->ObjectTypeEntries);
			SideBar->CB_mapprops = false;
		}
// 		QGraphicsItem *blah = new QGraphicsItem();
// 		MapView->activeItem = blah:
// 		for(QList<QGraphicsItem>::iterator it = MapView->ogilist.begin(); it != MapView->ogilist.end(); ++it)
		foreach(MapView->activeItem, MapView->scene()->items())
		{
			qWarning() << "Item-ID:" << MapView->activeItem->data(MapFrame::ID).toInt();
			if(MapView->activeItem->data(MapFrame::ID).toInt() == SideBar->itemListWidget->currentItem()->data(64).toInt())
			{
				qWarning() << MapView->activeItem->data(MapFrame::ID);
				break;
			}
		}
//  		MapView->activeItem = MapView->itemMapList.value(SideBar->itemListWidget->currentItem()->text());
		
// 		MapView->activeItem->setPos(10000,1000);
// 				qWarning() << "DebugMeldung";
		
		SideBar->XBox->setValue(MapView->activeItem->x());
		SideBar->YBox->setValue(MapView->activeItem->y());

		SideBar->ZBox->setValue(MapView->activeItem->zValue());
		
		
		int obj_id = MapView->activeItem->data(MapFrame::Function).toInt();
		qWarning() << obj_id;
		
		for(int i = 0; i< SideBar->itemTyp->count(); i++)
		{
			if(SideBar->itemTyp->itemData(i).toInt() == obj_id)
			{
			SideBar->itemTyp->setCurrentIndex(i);
			break;
			}
		}
// 		if(obj_id < 100)
// 		{
// 			SideBar->itemTyp->setCurrentIndex(obj_id);
// 		}
// 		else if(obj_id >= 100 && obj_id < 500)
// 		{
// 			SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->count() - 3);
// 		}
// 		else if(obj_id >= 500 && obj_id < 1000)
// 		{
// 			SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->count() - 2);
// 		}
// 		else if(obj_id >= 1000)
// 		{
// 			SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->count() - 1);
// 		}
		SideBar->editToolTip->setText(MapView->activeItem->data(MapFrame::Tooltip).toString());
		SideBar->nameLineEdit->setText(MapView->activeItem->data(MapFrame::Name).toString());
		
		
// 		if(MapView->activeItem->data(MapFrame::Filename).toString().size() > 43)
// 		{
// 			SideBar->fileView->setText(QString(MapView->activeItem->data(MapFrame::Filename).toString().right(39).prepend("...")));
// 			
// 		}
// 		else
		{
			SideBar->fileView->setText(MapView->activeItem->data(MapFrame::Filename).toString());
		}
		
		break;
	}
	}
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

	switch(SideBar->itemListWidget->currentRow())
	{
	case 0:
	{
		MapView->bgi_filename = MapView->fd_filename;
		MapView->scene()->setBackgroundBrush(QBrush(QImage(MapView->bgi_filename)));
		break;
	}
	case 1:
	{
		MapView->mapnorth = MapView->fd_filename;
		break;
	}
	case 2:
	{
		MapView->mapwest = MapView->fd_filename;
		break;
	}
	case 3:
	{
		MapView->mapsouth = MapView->fd_filename;
		break;
	}
	case 4:
	{
		MapView->mapeast = MapView->fd_filename;
		break;
	}
	default:
	{

		if(MapView->fd_filename.endsWith(".svg") && MapView->activeItem->data(MapFrame::Filename).toString().endsWith(".svg"))
		{
		QMessageBox *warning = new QMessageBox();
		warning->setText("Change SVG-Files is not supported yet!");
		warning->exec();
// 		QGraphicsSvgItem *svgitem = qgraphicsitem_cast<QGraphicsSvgItem*>(MapView->activeItem);
		}
		else
		{
		MapView->activeItem->setData(MapFrame::Filename, MapView->fd_filename);

		QGraphicsPixmapItem *gpi = qgraphicsitem_cast<QGraphicsPixmapItem*>(MapView->activeItem);
		
		if(gpi != 0)
		{
		gpi->setPixmap(QPixmap(MapView->fd_filename));
		}
// 		GI2GPMI( MapView->activeItem )->setPixmap(QPixmap(MapView->fd_filename));
		
//		MapView->pixmapItemList[MapView->activeItem]->setPixmap(QPixmap(MapView->fd_filename));
		}
		break;
	}
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
	else if(SideBar->itemListWidget->currentRow() <= 4)
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


void MainWindow::typeComboBoxHandler(/*const QString &*/ int typ)
{
qWarning() << "MainWindow::typeComboBoxHandler(int typ)" << typ << "ID:" << SideBar->itemTyp->itemData(typ).toInt();
autoSaved = false;
	if(!SideBar->CB_mapprops)		// holds the property, if the type-combo-box contains the maptype-Properties or the object-type-properties
	{
// 		MapView->setObjectType(typ);
		MapView->activeItem->setData(MapFrame::Function, SideBar->itemTyp->itemData(typ));
		qWarning() << MapView->object_typ << MapView->activeItem->data(MapFrame::Function).toInt();
	}
	else
	{
// 		MapView->setMapType(typ);
		MapView->maptyp = SideBar->itemTyp->itemData(typ).toInt();
		if(MapView->maptyp == Landcitymap || MapView->maptyp == Coastcitymap)
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

void MainWindow::lineEditHandler(const QString &text)
{
autoSaved = false;
	if(SideBar->itemListWidget->currentRow() == 0 )
	{
	MapView->cityname = text;
	}
	else if(SideBar->itemListWidget->currentRow() > 4)
	{
	MapView->activeItem->setData(MapFrame::Tooltip, text);
 	MapView->activeItem->setToolTip(text);
	}
}


void MainWindow::nameLineEditHandler(const QString &text)
{
autoSaved = false;
	if(SideBar->itemListWidget->currentRow() == 0 )
	{
	MapView->mapname = text;
	}
	else if(SideBar->itemListWidget->currentRow() > 4 && !QString(text).trimmed().simplified().isEmpty())
	{
	MapView->activeItem->setData(MapFrame::Name, text);
	SideBar->itemListWidget->currentItem()->setText(text);
	}
}


void MainWindow::markListItem(QGraphicsItem *selectedItem)
{
// for(int i = 0; SideBar->itemListWidget->item(i) != 0; i++)
// {
// }
// QList <QListWidgetItem*> matchingListEntry =  SideBar->itemListWidget->findItems(selectedItem->data(0).toString(), Qt::MatchContains);

qWarning() << "MainWindow::markListItem(QGraphicsItem *selectedItem)" //<< SideBar->itemListWidget->findItems(selectedItem->data(17).toString(), Qt::MatchStartsWith).first().text()
<< selectedItem->data(MapFrame::ID) << selectedItem->data(0).toInt();
// SideBar->itemListWidget->setCurrentItem(SideBar->itemListWidget->findItems(selectedItem->data(17).toString(), Qt::MatchStartsWith).first());
if(selectedItem != 0)
{
for(int i = 0; i < SideBar->itemListWidget->count(); i++)
{
qWarning() << SideBar->itemListWidget->item(i)->data(0);
	if(SideBar->itemListWidget->item(i)->data(64).toInt() == selectedItem->data(MapFrame::ID))
	{
		SideBar->itemListWidget->setCurrentRow(i);
		break;
	}
}
}
}


void MainWindow::deleteCurrentObject()
{
qWarning() << MapView->activeItem->data(MapFrame::Filename).toString();
autoSaved = false;
int currentListRow = SideBar->itemListWidget->currentRow();

if(!MapView->scene()->items().isEmpty() && currentListRow > 4)
{
qWarning() << "MainWindow::deleteCurrentObject()";
//int itemID = MapView->activeItem->data(17).toInt();

// MapView->itemMapList.remove(SideBar->itemListWidget->currentItem()->text());


// SideBar->itemListWidget->removeItemWidget(SideBar->itemListWidget->item(currentListRow));
SideBar->itemListWidget->setCurrentRow(0);
SideBar->itemListWidget->takeItem(currentListRow);



// qWarning() << MapView->itemList.size();
// MapView->ogilist.removeOne(MapView->activeItem);

qWarning() << "Deleting Item";
MapView->scene()->removeItem(MapView->activeItem);

 delete MapView->activeItem;
 
//  if(!ogilist.isEmpty())
//  {
//  activeItem = ogilist.first();
//  }
// MapView->szene->items

}
else
setStatusTip(tr("Error: There aren't any objects to delete!"));
}
