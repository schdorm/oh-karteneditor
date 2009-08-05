/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel and Joerg Thalheim  	   *
 *   oh.devs@googlemail.com 						   *
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
 
 #include <QtGui/QVBoxLayout>
 #include <QtGui/QHBoxLayout>
 #include <QtGui/QFrame>
 #include <QtGui/QKeyEvent>

 
 SideBarClass::SideBarClass()
 {
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
	
	QWidget *wid = new QWidget(this);
	SideBarLayout->addWidget(wid);

	QHBoxLayout *objectFileLayout = new QHBoxLayout(wid);

	fileView = new QLabel(this);
	fileView->setFrameShape(QFrame::Box);
	objectFileLayout->addWidget(fileView);

	selectFileButton = new QPushButton ("...", this);
	objectFileLayout->addWidget(selectFileButton);
	
	editToolTip = new QLineEdit(this);
	editToolTip->setMaxLength(30);
	SideBarLayout->addWidget(editToolTip );

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


//	SideBarLayout->addLayout(objectFileLayout);

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

