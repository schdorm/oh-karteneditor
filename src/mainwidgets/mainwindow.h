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

#ifndef _MAINWINDOW_H
#define _MAINWINDOW_H


#include <QtGui/QMainWindow>
 
//  #include <QtGui/QAction>
//  #include <QtGui/QMenu>
 
 class QMenu;
 class QAction;
 class MapFrame;
 class SideBarClass;
 class QGraphicsItem;
 class QTimer;
 class QSignalMapper;
//  #include "mapframe.h"
//  #include "sidebar.h"


class MainWindow : public QMainWindow
{
  Q_OBJECT
 /*
  protected:
     void contextMenuEvent(QContextMenuEvent *event);
 */
 public:
   MainWindow();
   ~MainWindow();
   SideBarClass *SideBar;
   bool itemAdded;
   
//   public slots:
//     void closeMapSettingsDialog(int);
//     void applyMapSettings();
    
    
    
  private slots:
    void newMap();
    //     void open();
    void loadMap();
    //     void savef();
    
    void saveMap();
    void saveMapAs();
    
    
    void autoSave();
    void loadAutoSave();
    
    void addNewObjectToList(QGraphicsItem *);
    
    void updateItemList(int);
    void updateSpinbox();
    
    void nameLineEditHandler(const QString &);
    void lineEditHandler(const QString &);
    void sideBar_SelectFile();
    void sideBar_FileSelected();
    void spinboxHandler();
    void typeComboBoxHandler(int);
    void markListItem(QGraphicsItem *);
    
    void deleteCurrentObject();
    
//     void mapPropertiesDialog();
    
    void preferences(); 
    void applySettings();
    
  private:
    void createActions();
    void createMenus();
    void createOldLayout();

enum itemListItemNames{
en_mapproperties,
en_mapnorth,
en_mapwest,
en_mapsouth,
en_mapeast
};


  MapFrame *MapView;

 
 QMenu *fileMenu;
 QMenu *editMenu;
 QMenu *helpMenu;

 QAction *newAct;
 QAction *openAct;
 QAction *saveAct;
 QAction *saveAsAct;
 
 QAction *loadAutoSaveAct;
 QAction *autoSaveAct;
 
 QSignalMapper *m_AS_loadMapper;
 QSignalMapper *m_AS_saveMapper;

 //  QAction *disableAutoSaveAct;
 
 QAction *quitAct;
 
 QAction *newObjectAct;
 QAction *rmcurrentObjectAct;
 
 QAction *m_MapPropertiesAction;
 
 QAction *m_PreferencesAction;
 QAction *m_AboutAction;
 
//  QTimer *autoSaveTimer;
 
//  QString mapfilename;
 bool existingMapFile;		// if true, no request for mapfile name
 bool autoSaved;
//  bool activeMapView;
 };
 #endif
