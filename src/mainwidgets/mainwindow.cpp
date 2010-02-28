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
 
#include "mainwindow.h"
#include "mapframe.h"
#include "sidebar.h"
#include "settingsdialog.h"
#include "settings.h"


#include <QtCore/QSignalMapper>
#include <QtGui/QAction>
#include <QtGui/QMenu>
#include <QtGui/QMenuBar>
#include <QtGui/QStatusBar>

#include <QtGui/QToolBar>
#include <QtGui/QToolButton>

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
 #include <QtGui/QFileDialog>

// #define _DEBUG_

MainWindow::MainWindow()
 {
// 	autoSaveTimer = new QTimer(this);
	#ifndef _DEBUG_
// 	autoSaveTimer->start(10000);
	#endif
 	existingMapFile = false;

	QWidget *zentralwidget = new QWidget(this);
	setCentralWidget(zentralwidget);


	SideBar = 0;

	MapView = new MapFrame(this);
	MapView->setParent(zentralwidget); 
// 	MapView->initMap();
//	MapView->setStatusTip("");
	
// 	QHashIterator <int, QString> it(SideBar->maptypelabels());
// 	while(it.hasNext())
// 	{
// 		it.next();
// 		SideBar->itemTyp->addItem(it.value(), it.key());
// 		SideBar->itemTyp->addItems(MapView->MapTypeEntries);
// 	}
// 	SideBar->comboBoxObjectEntries = MapView->itemfktList;

	QHBoxLayout *layout = new QHBoxLayout(zentralwidget);
// 	layout->setMargin(5); 
     
	layout->addWidget(MapView);

	zentralwidget->setLayout(layout);
	
	createActions();
	createMenus();


	QString message = tr("A context menu is available by right-clicking");
	statusBar()->showMessage(message);

	setWindowTitle(tr("OpenHanse Mapeditor"));
	setWindowIcon(QIcon(":.img/icon_map_vertikal2.png"));
	setMinimumSize(480, 360);
	resize(800, 600);
	
	if(SETTINGS->oldlayout())
	{
	  createOldLayout();
	}
	
	
	connect(MapView, SIGNAL(mapChanged()), this, SLOT(updateData()));
	
	connect(MapView, SIGNAL(newObjectCreated(QGraphicsItem *)), this, SLOT(addNewObjectToList(QGraphicsItem*)));
	

	connect(MapView, SIGNAL(objectSelected(int)), this, SLOT(markListItem(int)));

// 	connect(MapView, SIGNAL(objectSelected(QGraphicsItem*)), this, SLOT(markListItem(QGraphicsItem*)));
	
	connect(MapView, SIGNAL(objectMoved()), this, SLOT(updateSpinbox()));
	
	connect(MapView, SIGNAL(SIG_deleteObject()), this, SLOT(deleteCurrentObject()));
	
	connect(SETTINGS, SIGNAL(changed()), this, SLOT(applySettings()));
	
	connect(MapView, SIGNAL(dataChanged()), this, SLOT(autoSaveMap()));


	connect(SETTINGS, SIGNAL(autosave()), this, SLOT(autoSave()));
	loadAutoSave();
}


MainWindow::~MainWindow()
{
  delete MapView;
  if(SETTINGS->oldlayout())
  {
    delete SideBar;
  }
}

void MainWindow::createActions()
{
  newAct = new QAction(tr("&New"), this);
  newAct->setShortcuts(QKeySequence::New);
  newAct->setStatusTip(tr("Create a new map"));
  newAct->setIcon(QIcon(":.img/icon_new_map_01.png"));
  connect(newAct, SIGNAL(triggered()), this, SLOT(newMap()));
  connect(newAct, SIGNAL(triggered()), MapView, SLOT(newMap()));
  
  openAct = new QAction(tr("&Open..."), this);
  openAct->setShortcuts(QKeySequence::Open);
  openAct->setStatusTip(tr("Open an existing map"));
  openAct->setIcon(QIcon(":.img/icon_open_map_03.png"));
  connect(openAct, SIGNAL(triggered()), this, SLOT(loadMap()));
  
  saveAct = new QAction(tr("&Save"), this);
  saveAct->setShortcuts(QKeySequence::Save);
  saveAct->setStatusTip(tr("Save the map with the current name to disk"));
  connect(saveAct, SIGNAL(triggered()), this, SLOT(saveMap()));
  
  saveAsAct = new QAction(tr("Save As ..."), this);
  #if QT_VERSION > 0x040501
  saveAsAct->setShortcuts(QKeySequence::SaveAs);
  #endif
  saveAsAct->setStatusTip(tr("Save the map to disk"));
  connect(saveAsAct, SIGNAL(triggered()), this, SLOT(saveMapAs()));
  
  
  //   m_AS_saveMapper = new QSignalMapper(this);
  //   connect(m_AS_saveMapper, SIGNAL(mapped(QString)), this, SLOT(autoSave(QString)));
  
  autoSaveAct = new QAction(tr("Save Autosave"), this);
  autoSaveAct->setShortcut(QKeySequence(tr("Ctrl + 1")));
  autoSaveAct->setStatusTip(tr("Saves the Map as Autosaved Map"));
  //   connect(autoSaveAct, SIGNAL(triggered()), m_AS_saveMapper, SLOT(map()));
  connect(autoSaveAct, SIGNAL(triggered()), this, SLOT(autoSave()));
  
  //   m_AS_loadMapper = new QSignalMapper(this);
  //   connect(m_AS_loadMapper, SIGNAL(mapped(QString)), this, SLOT(loadAutoSave(QString)));
  
  loadAutoSaveAct = new QAction(tr("Load Autosave"), this);
  loadAutoSaveAct->setShortcut(QKeySequence(tr("Ctrl + 2")));
  loadAutoSaveAct->setStatusTip(tr("Loads the Autosaved Map"));
  //   connect(loadAutoSaveAct, SIGNAL(triggered()), m_AS_loadMapper, SLOT(map()));
  connect(loadAutoSaveAct, SIGNAL(triggered()), this, SLOT(loadAutoSave()));
  
  
  
  //   disableAutoSaveAct = new QAction(tr("Disable Autosaves"), this);
  //   disableAutoSaveAct->setStatusTip(tr("Disables autosaves for this session (temporary)"));
  //   connect(disableAutoSaveAct, SIGNAL(triggered()), SETTINGS, SLOT(disableAutosave()));
  
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
  
  m_MapPropertiesAction = new QAction(tr("Mapproperties"), this);
  m_MapPropertiesAction->setStatusTip(tr("Opens a Dialog for setting different properties of maps."));
  connect(m_MapPropertiesAction, SIGNAL(triggered()), MapView, SLOT(mapSettingsDialog()));
  
  m_PreferencesAction = new QAction(tr("Preferences"), this);
  m_PreferencesAction->setStatusTip("Set your preferences in the settings-dialog.");
  connect(m_PreferencesAction, SIGNAL(triggered()), this, SLOT(preferences()));
  
  m_ChangePerspectiveButton = new QToolButton(this);
  m_ChangePerspectiveButton->setText("View");
  m_ChangePerspectiveButton->setEnabled(false);
  connect(m_ChangePerspectiveButton, SIGNAL(clicked()), this, SLOT(changeViewSlot()));
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
  //   fileMenu->addAction(disableAutoSaveAct);
  fileMenu->addSeparator();
  fileMenu->addAction(quitAct);
  
  editMenu = menuBar()->addMenu(tr("&Edit"));
  editMenu->addAction(newObjectAct);
  editMenu->addAction(rmcurrentObjectAct);
  
  helpMenu = menuBar()->addMenu(tr("&Help"));
  helpMenu->addAction(m_PreferencesAction);
  
  QToolBar *tb = addToolBar(tr("Maintoolbar"));
  tb->addAction(newAct);
  tb->addAction(openAct);
  tb->addSeparator();
  tb->addAction(m_MapPropertiesAction);
  tb->addWidget(m_ChangePerspectiveButton);
}


void MainWindow::createOldLayout()
{
  SideBar = new SideBarClass(this);
  SideBar->setParent(centralWidget());
  centralWidget()->layout()->addWidget(SideBar);
  //    connect(SideBar->nameLineEdit, SIGNAL(textEdited(QString)), this, SLOT(nameLineEditHandler(QString)));
  connect(SideBar->itemListWidget, SIGNAL(currentRowChanged(int)), this, SLOT(updateItemList(int)));
  //   connect(SideBar->itemTyp, SIGNAL(activated(int)), this, SLOT(typeComboBoxHandler(int)));
  //   connect(SideBar->selectFileButton, SIGNAL(clicked()), this, SLOT(sideBar_SelectFile()));
  //   connect(SideBar->editToolTip, SIGNAL(textEdited(QString)), this, SLOT(lineEditHandler(QString)));
  //   connect(SideBar->XBox, SIGNAL(valueChanged(int)), this, SLOT(spinboxHandler()));
  //   connect(SideBar->YBox, SIGNAL(valueChanged(int)), this, SLOT(spinboxHandler()));
  //   // 	connect(SideBar->XBox, SIGNAL(editingFinished()), this, SLOT(spinboxHandler()));
  //   // 	connect(SideBar->YBox, SIGNAL(editingFinished()), this, SLOT(spinboxHandler()));
  //   connect(SideBar->ZBox, SIGNAL(valueChanged(double)), this, SLOT(spinboxHandler()));
  connect(SideBar, SIGNAL(dataChanged()), this, SLOT(updateData()));
  connect(SideBar, SIGNAL(SIG_deleteObject()), this, SLOT(deleteCurrentObject()));
  SideBar->fillList(MapView->scene()->items());
  SideBar->itemListWidget->setCurrentRow(0);
}

void MainWindow::newMap()
{
  qWarning() << "MainWindow::newMap()";
  if(SETTINGS->oldlayout())
  {
    SideBar->itemListWidget->clear();
//     SideBar->itemListWidget->addItems(SideBar->staticListEntries);
    SideBar->addStaticListItems();
    SideBar->XBox->setValue(1000);
    SideBar->YBox->setValue(1000);
    SideBar->ZBox->setValue(0);
    SideBar->editToolTip->setText(QString());
    SideBar->itemListWidget->setCurrentRow(0);
  }
  existingMapFile = false;
}


void MainWindow::loadAutoSave()
{
  if(QFile(SETTINGS->autosavepath()).exists())
  {
    existingMapFile = MapView->loadMap(SETTINGS->autosavepath());
    if(!existingMapFile)
    {
      MapView->newMap();
    }
  }
  
}


void MainWindow::loadMap()
{
  existingMapFile = true;
  
  QString mapfilename = QFileDialog::getOpenFileName(this, "Open Map File", QDir().current().absolutePath(), tr("OpenHanse Map files (*.ohm)" ));
  
  qWarning() << mapfilename;
  
  
  if(SETTINGS->oldlayout())
  {
    SideBar->itemListWidget->clear();
    SideBar->itemListWidget->addItems(SideBar->staticListEntries);
  }
  
  MapView->loadMap(mapfilename);
  
  setWindowTitle(QFileInfo(MapView->map()->filename()).baseName() + tr(" - OpenHanse Mapeditor"));

  
  if(SETTINGS->oldlayout())
    SideBar->itemListWidget->setCurrentRow(0);
  
}


void MainWindow::autoSave()
{
  if(!m_autoSaved)
  {
    MapView->saveMap(SETTINGS->autosavepath());
    m_autoSaved = true;
  }
}

void MainWindow::autoSaveMap()
{
  MapView->saveMap(SETTINGS->autosavepath());
  m_autoSaved = true;
}


 void MainWindow::saveMapAs()
 {
 
  MapView->map()->save( QFileDialog::getSaveFileName(this, tr("Save File"), QString() , tr("OpenHanse maps (*.ohm);; All Files (*)")));

  setWindowTitle(QFileInfo(MapView->map()->filename()).baseName() + tr(" - OpenHanse Mapeditor"));
}

void MainWindow::saveMap()
{
  if(QFile(MapView->map()->filename()).exists())
  {
    MapView->map()->save();
  }
  else
    saveMapAs();
}




 
 void MainWindow::preferences()
 {
   Settingswidget *settings = new Settingswidget();
   settings->show();
   connect(settings, SIGNAL(finished(int)), settings, SLOT(deleteLater()));
 }
 
 void MainWindow::applySettings()
 {
   if(!SETTINGS->oldlayout())
   {
     delete SideBar;
     SideBar = 0;
   }
   else if(SideBar == 0)
   {
     createOldLayout();
   }
   
 }
 

 
void MainWindow::addNewObjectToList(QGraphicsItem *newObject)
{
m_autoSaved = false;

// SideBar->MapEntries << entry->data(0).toString().append(entry->data(1).toString()).append(entry->data(2).toString());
qWarning() << "MainWindow::addNewObjectToList(QGraphicsItem *newObject) :: ID:" << newObject->data(MapFrame::ID).toInt();

// SideBar->itemListWidget->addItem(newObject->data(17).toString().append("; ").append(newObject->data(1).toString()).append(newObject->data(2).toString().right(14).left(10)));
if(SETTINGS->oldlayout())
{
  QListWidgetItem *lwi = new QListWidgetItem(newObject->data(MapFrame::Name).toString(), SideBar->itemListWidget);
  lwi->setData(Qt::UserRole, newObject->data(MapFrame::ID).toInt());
  // 	SideBar->itemListWidget->addItem(newObject->data(MapFrame::Name));
  
  qWarning() << newObject->data(MapFrame::Name);
 
// SideBar->itemListWidget->setCurrentRow(SideBar->itemListWidget->count() - 1);
  SideBar->itemListWidget->setCurrentItem(lwi);
  itemAdded = true;
}
}


void MainWindow::updateData()
{
  m_autoSaved = false;
  
  if(SETTINGS->oldlayout())
  {
    spinboxHandler();
  }
}

void MainWindow::updateSideBar(int selectedItemRow)
{
  if(SETTINGS->oldlayout())
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
		SideBar->fileView->setText(MapView->map()->background());
		SideBar->nameLineEdit->setEnabled(true);
		//SideBar->editToolTip->setToolTip(tr("Comment"));
// 		SideBar->editToolTip->setEnabled(false);
// 		SideBar->editToolTip->setText(QString());
// 		if(MapView->maptyp == (Map::Land ^ Map::Citymap) || MapView->maptyp == (Map::Coast ^ Map::Citymap))
		if(MapView->map()->isCity())
		{
			SideBar->editToolTip->setEnabled(true);
			SideBar->editToolTip->setText(MapView->map()->city()->name());
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
// 			int MapTypeRow = MapView->maptyp;
// 			qWarning() << MapTypeRow;
// 			SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->findText(MapView->maptypename));
// 			SideBar->itemTyp->setCurrentIndex(MapTypeRow);
		}
	  SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->findData(QVariant(MapView->map()->type())));
		SideBar->nameLineEdit->setText(MapView->map()->filename());
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(true);
		SideBar->XBox->setEnabled(true);
		SideBar->XBox->setValue(MapView->map()->size().width());
		SideBar->YBox->setValue(MapView->map()->size().height());
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
		SideBar->fileView->setText(MapView->map()->mapnorth());
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
		SideBar->fileView->setText(MapView->map()->mapwest());
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
		SideBar->fileView->setText(MapView->map()->mapsouth());
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
		SideBar->fileView->setText(MapView->map()->mapeast());
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
///		foreach(MapView->activeItem, MapView->scene()->items())
/*		{
			qWarning() << "Item-ID:" << MapView->activeItem->data(Qt::UserRole).toInt();
			if(MapView->activeItem->data(Qt::UserRole).toInt() == SideBar->itemListWidget->currentItem()->data(Qt::UserRole).toInt())
			{
				qWarning() << MapView->activeItem->data(Qt::UserRole);
				break;
			}
		}*/ ///**************************************
//  		MapView->activeItem = MapView->itemMapList.value(SideBar->itemListWidget->currentItem()->text());
		
// 		MapView->activeItem->setPos(10000,1000);
// 				qWarning() << "DebugMeldung";
		
		SideBar->XBox->setValue(MapView->activeItem()->x());
		SideBar->YBox->setValue(MapView->activeItem()->y());

		SideBar->ZBox->setValue(MapView->activeItem()->zValue());
		
		
		int obj_id = MapView->activeItem()->data(MapFrame::Function).toInt();
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
		SideBar->editToolTip->setText(MapView->activeItem()->data(MapFrame::Tooltip).toString());
		SideBar->nameLineEdit->setText(MapView->activeItem()->data(MapFrame::Name).toString());
		
		
// 		if(MapView->activeItem->data(MapFrame::Filename).toString().size() > 43)
// 		{
// 			SideBar->fileView->setText(QString(MapView->activeItem->data(MapFrame::Filename).toString().right(39).prepend("...")));
// 			
// 		}
// 		else
		{
			SideBar->fileView->setText(MapView->activeItem()->data(MapFrame::Filename).toString());
		}
		
		break;
	}
      } // end switch-case
    } // endif selectedRow >= 0
  } // endif oldlayout
}

// void MainWindow::sideBar_SelectFile()
// {
//   if(SETTINGS->oldlayout())
//   {
//     qWarning() << "MainWindow::sideBar_SelectFile()";
//     if(SideBar->itemListWidget->currentRow() == 0 || SideBar->itemListWidget->currentRow() > 4)
//     {
//       ///MapView->fileDialog(NameFilters::Img);
//     }
//     else
//     {
//       ///MapView->fileDialog(NameFilters::Map);
//     }
//     /// connect(MapView->fd, SIGNAL(accepted()), this, SLOT(sideBar_FileSelected()));
//   }
// }

// void MainWindow::sideBar_FileSelected()
// {
//   autoSaved = false;
//   
//   qWarning() << "MainWindow::sideBar_FileSelected()";
//   ///disconnect(MapView->fd, SIGNAL(accepted()), this, SLOT(sideBar_FileSelected()));
//   
//   if(SETTINGS->oldlayout())
//   {
//     SideBar->fileView->setText(MapView->fd_filename);
//     
//     switch(SideBar->itemListWidget->currentRow())
//     {
//       case 0:
//       {
// 	// 		MapView->bgi_filename = MapView->fd_filename;
// 	// 		MapView->scene()->setBackgroundBrush(QBrush(QImage(MapView->bgi_filename)));
// 	break;
//       }
//       case 1:
//       {
// 	// 		MapView->mapnorth = MapView->fd_filename;
// 	break;
//       }
//       case 2:
//       {
// 	// 		MapView->mapwest = MapView->fd_filename;
// 	break;
//       }
//       case 3:
//       {
// 	// 		MapView->mapsouth = MapView->fd_filename;
// 	break;
//       }
//       case 4:
//       {
// 	// 		MapView->mapeast = MapView->fd_filename;
// 	break;
//       }
//       default:
//       {
// 	
// 	/*		if(MapView->fd_filename.endsWith(".svg") && MapView->activeItem->data(MapFrame::Filename).toString().endsWith(".svg"))
// 	{
// 	  QMessageBox *warning = new QMessageBox();
// 	  warning->setText("Change SVG-Files is not supported yet!");
// 	  warning->exec();
// 	  // 		QGraphicsSvgItem *svgitem = qgraphicsitem_cast<QGraphicsSvgItem*>(MapView->activeItem);
//       }
//       else
//       {
// 	MapView->activeItem->setData(MapFrame::Filename, MapView->fd_filename);
// 	
// 	QGraphicsPixmapItem *gpi = qgraphicsitem_cast<QGraphicsPixmapItem*>(MapView->activeItem);
// 	
// 	if(gpi != 0)
// 	{
// 	  gpi->setPixmap(QPixmap(MapView->fd_filename));
//       }
//       // 		GI2GPMI( MapView->activeItem )->setPixmap(QPixmap(MapView->fd_filename));
//       
//       //		MapView->pixmapItemList[MapView->activeItem]->setPixmap(QPixmap(MapView->fd_filename));
//     }
//     break;*/
//       }
//     }
//   }
// }

void MainWindow::spinboxHandler()
{
  if(SETTINGS->oldlayout())
  {
    m_autoSaved = false;
    // qWarning() << "MainWindow::spinboxHandler()";

    if(SideBar->XBox->hasFocus() || SideBar->YBox->hasFocus() || SideBar->ZBox->hasFocus())
    {
	if(SideBar->itemListWidget->currentRow() > 4 /*&& !MapView->itemGrabbed && !itemAdded*/)
	{
// 		MapView->itemMapList.value(SideBar->itemListWidget->currentItem()->text())->setPos(SideBar->XBox->value(), SideBar->YBox->value());
// 		MapView->itemMapList.value(SideBar->itemListWidget->currentRow() - 6)->setZValue(SideBar->ZBox->value());
///		MapView->activeItem->setPos(SideBar->XBox->value(), SideBar->YBox->value());
///		MapView->activeItem->setZValue(SideBar->ZBox->value());
	}
	else if(SideBar->itemListWidget->currentRow() <= 4)
	{
		MapView->setMapSize(QSize(SideBar->XBox->value(), SideBar->YBox->value()));
		MapView->setSceneRect(0, 0, MapView->mapSize().width(), MapView->mapSize().height());
	}
	if(itemAdded)
		itemAdded = false;
    }
  }
}

void MainWindow::updateSpinbox()
{
  if(SETTINGS->oldlayout())
  {
    m_autoSaved = false;
    SideBar->XBox->setValue(MapView->activeItem()->x());
    SideBar->YBox->setValue(MapView->activeItem()->y());
  }
}


void MainWindow::typeComboBoxHandler(/*const QString &*/ int typ)
{
  if(SETTINGS->oldlayout())
  {
    qWarning() << "MainWindow::typeComboBoxHandler(int typ)" << typ << "ID:" << SideBar->itemTyp->itemData(typ).toInt();
    m_autoSaved = false;
	if(!SideBar->CB_mapprops)		// holds the property, if the type-combo-box contains the maptype-Properties or the object-type-properties
	{
// 		MapView->setObjectType(typ);
/// 		MapView->activeItem->setData(MapFrame::Function, SideBar->itemTyp->itemData(typ));
// 		qWarning() << MapView->activeItem->data(MapFrame::Function).toInt();
	}
	else
	{
// 		MapView->setMapType(typ);
		MapView->maptyp = SideBar->itemTyp->itemData(typ).toInt();
		if(MapView->map()->isCity())
		{
			SideBar->editToolTip->setEnabled(true);
			SideBar->editToolTip->setText(MapView->map()->city()->name());
		}
		else
		{
			SideBar->editToolTip->setEnabled(false);
			SideBar->editToolTip->setText(QString());
		}
	}
  }
}

void MainWindow::lineEditHandler(const QString &text)
{
  if(SETTINGS->oldlayout())
  {
    m_autoSaved = false;
	if(SideBar->itemListWidget->currentRow() == 0 )
	{
	MapView->map()->city()->setName(text);
	}
	else if(SideBar->itemListWidget->currentRow() > 4)
	{
// 	MapView->activeItem->setData(MapFrame::Tooltip, text);
//  	MapView->activeItem->setToolTip(text);
	}
  }
}


void MainWindow::nameLineEditHandler(const QString &text)
{
  if(SETTINGS->oldlayout())
  {
    m_autoSaved = false;
	if(SideBar->itemListWidget->currentRow() == 0 )
	{
	MapView->map()->setName(text);
	}
	else if(SideBar->itemListWidget->currentRow() > 4 && !QString(text).trimmed().simplified().isEmpty())
	{
// 	MapView->activeItem->setData(MapFrame::Name, text);
// 	SideBar->itemListWidget->currentItem()->setText(text);
	}
  }
}

void MainWindow::markListItem(int a_objectid)
{
  if(SETTINGS->oldlayout())
  {
    qWarning() << "void MainWindow::markListItem(int a_objectid)";
    if(a_objectid > 0)
    {
      for(int i = 0; i < SideBar->itemListWidget->count(); i++)
      {
	qWarning() << SideBar->itemListWidget->item(i)->data(Qt::UserRole);
	if(SideBar->itemListWidget->item(i)->data(Qt::UserRole).toInt() == a_objectid)
	{
	  SideBar->itemListWidget->setCurrentRow(i);
	  i = SideBar->itemListWidget->count();
	}
      }
    }
    else if(a_objectid == -1)
    {
      SideBar->itemListWidget->setCurrentRow(0);
    }
  }
}


void MainWindow::markListItem(QGraphicsItem *selectedItem)
{
  if(SETTINGS->oldlayout())
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
	  if(SideBar->itemListWidget->item(i)->data(Qt::UserRole).toInt() == selectedItem->data(MapFrame::ID))
	  {
		SideBar->itemListWidget->setCurrentRow(i);
		break;
	  }
	}
      }
  }
}

void MainWindow::sidebarHandler()
{
  if(SETTINGS->oldlayout())
  {
    int selectedItemRow = SideBar->currentRowData();

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
		SideBar->fileView->setText(MapView->map()->background());
		SideBar->nameLineEdit->setEnabled(true);
		//SideBar->editToolTip->setToolTip(tr("Comment"));
// 		SideBar->editToolTip->setEnabled(false);
// 		SideBar->editToolTip->setText(QString());
// 		if(MapView->maptyp == (Map::Land ^ Map::Citymap) || MapView->maptyp == (Map::Coast ^ Map::Citymap))
		if(MapView->map()->isCity())
		{
			SideBar->editToolTip->setEnabled(true);
			SideBar->editToolTip->setText(MapView->map()->city()->name());
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
// 			int MapTypeRow = MapView->maptyp;
// 			qWarning() << MapTypeRow;
// 			SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->findText(MapView->maptypename));
// 			SideBar->itemTyp->setCurrentIndex(MapTypeRow);
		}
	  SideBar->itemTyp->setCurrentIndex(SideBar->itemTyp->findData(QVariant(MapView->map()->type())));
		SideBar->nameLineEdit->setText(MapView->map()->filename());
		SideBar->ZBox->setEnabled(false);
		SideBar->YBox->setEnabled(true);
		SideBar->XBox->setEnabled(true);
		SideBar->XBox->setValue(MapView->map()->size().width());
		SideBar->YBox->setValue(MapView->map()->size().height());
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
		SideBar->fileView->setText(MapView->map()->mapnorth());
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
		SideBar->fileView->setText(MapView->map()->mapwest());
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
		SideBar->fileView->setText(MapView->map()->mapsouth());
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
		SideBar->fileView->setText(MapView->map()->mapeast());
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
///		foreach(MapView->activeItem, MapView->scene()->items())
/*		{
			qWarning() << "Item-ID:" << MapView->activeItem->data(Qt::UserRole).toInt();
			if(MapView->activeItem->data(Qt::UserRole).toInt() == SideBar->itemListWidget->currentItem()->data(Qt::UserRole).toInt())
			{
				qWarning() << MapView->activeItem->data(Qt::UserRole);
				break;
			}
		}*/ ///*****************************************************************************/////
//  		MapView->activeItem = MapView->itemMapList.value(SideBar->itemListWidget->currentItem()->text());
		
// 		MapView->activeItem->setPos(10000,1000);
// 				qWarning() << "DebugMeldung";
		
		SideBar->XBox->setValue(MapView->activeItem()->x());
		SideBar->YBox->setValue(MapView->activeItem()->y());

		SideBar->ZBox->setValue(MapView->activeItem()->zValue());
		
		
		int obj_id = MapView->activeItem()->data(MapFrame::Function).toInt();
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
		SideBar->editToolTip->setText(MapView->activeItem()->data(MapFrame::Tooltip).toString());
		SideBar->nameLineEdit->setText(MapView->activeItem()->data(MapFrame::Name).toString());
		
		
// 		if(MapView->activeItem->data(MapFrame::Filename).toString().size() > 43)
// 		{
// 			SideBar->fileView->setText(QString(MapView->activeItem->data(MapFrame::Filename).toString().right(39).prepend("...")));
// 			
// 		}
// 		else
		{
			SideBar->fileView->setText(MapView->activeItem()->data(MapFrame::Filename).toString());
		}
		
		break;
	}
      } // end switch-case
    } // endif selectedRow >= 0
  } // endif oldlayout
}

void MainWindow::deleteCurrentObject()
{ 
  qWarning() << "MainWindow::deleteCurrentObject()" << MapView->activeItem()->data(MapFrame::Filename).toString();
  m_autoSaved = false;
  if(SETTINGS->oldlayout())
  {
    int currentListRow = SideBar->itemListWidget->currentRow();

    if(!MapView->scene()->items().isEmpty() && currentListRow > 4)
    {
      //int itemID = MapView->activeItem->data(17).toInt();

      // MapView->itemMapList.remove(SideBar->itemListWidget->currentItem()->text());


      // SideBar->itemListWidget->removeItemWidget(SideBar->itemListWidget->item(currentListRow));
      SideBar->itemListWidget->setCurrentRow(0);
      SideBar->itemListWidget->takeItem(currentListRow);

      // qWarning() << MapView->itemList.size();
      // MapView->ogilist.removeOne(MapView->activeItem);

      qWarning() << "Deleting Item";
      MapView->removeCurrentItem();

 
      //  if(!ogilist.isEmpty())
      //  {
	//  activeItem = ogilist.first();
	//  }
	// MapView->szene->items

    }
    else
      setStatusTip(tr("Error: There aren't any objects to delete!"));
  }
  else if(!MapView->scene()->items().isEmpty())
  {
    MapView->removeCurrentItem();
  }
}

void MainWindow::changeViewSlot()
{
  m_ChangePerspectiveButton->setDown(MapView->changePerspective());
}
