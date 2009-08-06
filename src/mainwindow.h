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

 #ifndef _MAINWINDOW_H
 #define _MAINWINDOW_H


 #include <QtGui/QMainWindow>
 
 #include <QtGui/QAction>
 #include <QtGui/QMenu>

 #include "mapframe.h"
 #include "sidebar.h"


 class MainWindow : public QMainWindow
 {
 Q_OBJECT
 /*
  protected:
     void contextMenuEvent(QContextMenuEvent *event);
 */
 public:
 MapFrame *MapView;

 SideBarClass *SideBar;
 MainWindow();

 bool itemAdded;

 private slots:
 void newMap();
 void open();
 void openMap();
 void saveHandler();
 
 void autoSave();

 void loadAutoSaveMap();
 
 void addNewObjectToList(QGraphicsItem *);

 void updateItemList(int);
 void updateSpinbox();
 
 void lineEditHandler(QString);
 void sideBar_SelectFile();
 void sideBar_FileSelected();
 void spinboxHandler();
 void typeComboBoxHandler(QString);
 void markListItem(QGraphicsItem*);
 
 void deleteCurrentObject();

 private:
 void createActions();
 void createMenus();
 
 void save();
 void savef();
 
 QMenu *fileMenu;
 QMenu *editMenu;
 QMenu *helpMenu;

 QAction *newAct;
 QAction *openAct;
 QAction *saveAct;
 QAction *saveAsAct;
 QAction *loadAutoSaveAct;
 QAction *autoSaveAct;
 QAction *quitAct;
 
 QAction *newObjectAct;
 QAction *rmcurrentObjectAct;
 
 QString mapfilename;
 bool existingMapFile;
 bool autoSaved;
//  bool activeMapView;
 };
 #endif
 