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
    
    void autoSaveMap();
    void autoSave();
    void loadAutoSave();
    
    void addNewObjectToList(QGraphicsItem *);
    
    void updateData();
    void updateSideBar(int);
    void updateSpinbox();
    
    void markListItem(int);	// argument is object-/item-id
    
    void sidebarHandler();
    
    void nameLineEditHandler(const QString &);
    void lineEditHandler(const QString &);
//     void sideBar_SelectFile();
//     void sideBar_FileSelected();
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
      en_mapproperties = 0,
      en_mapnorth = 1,
      en_mapwest = 2,
      en_mapsouth = 3,
      en_mapeast = 4
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
    
    
    bool existingMapFile;		// if true, no request for mapfile name
    bool m_autoSaved;
    //  bool activeMapView;
};


 #endif

