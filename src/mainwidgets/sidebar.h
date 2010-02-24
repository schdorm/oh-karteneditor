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

#ifndef _SIDEBAR_H
#define _SIDEBAR_H

#include <QtGui/QWidget>
#include <QtCore/QHash>


class QListWidget;
class QLineEdit;
class QLabel;
class QPushButton;
class QSpinBox;
class QComboBox;
class MainWindow;
class QDoubleSpinBox;
class QGraphicsItem;

class MapObject;

namespace SidebarList
{
  enum
  {
    Item_BG = ~1,
    Item_MN = ~2,
    Item_ME = ~3,
    Item_MS = ~4,
    Item_MW = ~5
  };
}
    
class SideBarClass : public QWidget
{
  Q_OBJECT
  public:
    
    enum Filetypes
    {
      OpenHanseMap,
      ItemImage,
      Backgroundbrush
    };
    
    
    void addStaticListItems();
    void fillList(QList<QGraphicsItem*>);
    void fillList(const QList<MapObject> &itemlist);

    int currentRow() const;
    int currentRowData	() ;
    
    QStringList staticListEntries;
    
    
    bool CB_mapprops;
    
    SideBarClass(const MainWindow *);
    QString maptypelabel(int) const;
    
    QComboBox *itemTyp;
    QListWidget *itemListWidget;
    QPushButton *selectFileButton;
    QLineEdit *fileView;
    QLineEdit *nameLineEdit;
    QLineEdit *editToolTip;
    QSpinBox *XBox, *YBox;
    QDoubleSpinBox *ZBox;
    //void initMapEntriesList();
    const QHash<int, QString> &functionlabels()	const	{	return functionLabels;	}
    const QHash<int, QString> &maptypelabels()	const	{	return maptypeLabels;	}
    
  private slots:
    void selectFile();
    
  protected:
    
    QHash<int, QString> functionLabels;
    QHash<int, QString> maptypeLabels;
    
    const MainWindow *m_parent;
    void keyPressEvent(QKeyEvent*);
    

    
  signals:
    void SIG_deleteObject();
    void dataChanged();
    
    
};
#endif
