/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel and Joerg Thalheim  	   *
 *   oh.devs@googlemail.com  						   *
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

 #include "mapframe.h"
 #include "mainwindow.h"
 #include "sidebar.h"

 #include <QtCore/QDebug>
 #include <QtCore/QList>
 #include <QtCore/QDir>
 #include <QtCore/QFile>
 #include <QtCore/QTextStream>
 #include <QtGui/QComboBox>
 #include <QtGui/QLabel>
 #include <QtGui/QGridLayout>
 #include <QtGui/QHBoxLayout>
 #include <QtGui/QPushButton>
 #include <QtGui/QLineEdit>
 #include <QtGui/QSpinBox>
 
 #include <QtGui/QScrollBar>
 
 #include <QtGui/QKeyEvent>

 #include <QtCore/QStringList> 
 #include <QtCore/QSignalMapper>

#include <QtCore/QXmlStreamReader>


#include <QtSvg/QGraphicsSvgItem>

//  #include <QtGui/QPixmap>
 
 
 MapFrame::MapFrame( const MainWindow *mainwin)
 {
 m_MainWindow = mainwin;
 activeItem = 0;
 qWarning() << "MapFrame::initMap()";
 QGraphicsScene *szene = new QGraphicsScene();
 setScene(szene);
 newMap();
//  ot_townhall = tr("Rathaus");
//  ot_market = tr("Markt");
//  ot_church = tr("Kirche");
//  ot_port = tr("Hafen");
//  ot_office = tr("Kontor");
//  ot_bank = tr("Darlehensgeber");
//  ot_tavern = tr("Kneipe");
//  ot_land = tr("Landflaechen");
//  ot_land2 = tr("\"boese\" Untiefen"); // Objekte, bei denen eine Kollision mit dem Schiff Schaden am Schiff verursacht.
//  ot_mapdecoration = tr("Mapdeko");
//  ObjectTypeEntries << ot_townhall << ot_market << ot_church << ot_port << ot_office << ot_bank << ot_tavern << ot_land << ot_land2 << ot_mapdecoration;
//  
//  mt_sea = tr("See/Meer");
//  mt_coast = tr("Kueste");
//  mt_land = tr("Land");
//  mt_coast_city = tr("Kueste (Stadt)");
//  mt_land_city = tr("Land (Stadt)");
//  MapTypeEntries << mt_sea  << mt_coast << mt_land << mt_coast_city << mt_land_city;
 
 itemGrabbed = false;
 }

 void MapFrame::newMap()
 {
 qWarning() << "void MapFrame::newMap()";
 maptyp = Seamap;
 bgi_filename = QString();
 mapnorth = QString(); mapwest = QString();  mapsouth = QString(); mapeast = QString();

 scene()->clear();
 mapSize = QSize(1000,1000);
 setSceneRect(0,0,1000,1000);
 itemSelected = false;
 scene()->setBackgroundBrush(QBrush());
 }

void MapFrame::saveMap(QString save_filename)
{
qWarning() << "Saving Map ..." << save_filename;
if(!save_filename.isEmpty())
{
	if(!save_filename.endsWith(".ohm"))
	{
		save_filename.append(".ohm");
	}
//	QList<QGraphicsItem*> savelist = itemList();


	QString save_bgi_filename = bgi_filename;		// Hintergrundfilenamensbearbeitung
	QString save_dir_string = save_filename;

	  {
		int n = 0;
		int y = 0;
		if(!save_bgi_filename.isEmpty())
		{
		while(n < 1)
		{
			y++;
			n = save_bgi_filename.right(y).count("/");
			if(y > save_bgi_filename.size())
			{
			n=1;
			qWarning() << "While 1 Aborted";
			}
// 			qWarning() << "Schleife 1";
		}
		save_bgi_filename = save_bgi_filename.right(y).prepend("img");
		n = 0;
		y = 0;
		}
		while(n < 1)
		{
			y++;
			n = save_dir_string.right(y).count("/");
// 			qWarning() << /**/"while 2";
			if(y > save_dir_string.size())
			{
			qWarning() << "While 2 Aborted";
			return;
			}
		}
		save_dir_string = save_dir_string.left(save_dir_string.size() - y);
	  }
	  qWarning() << "Dir: " << save_dir_string << "\tFile: "<<save_filename;
	QDir save_dir = QDir(save_dir_string);
	if(!save_dir.cd("img"))
	{
		if(!save_dir.mkdir("img"))
		{
			QDialog *saveFailedDialog = new QDialog(this);
			saveFailedDialog->setModal(true);
			saveFailedDialog->setWindowTitle(tr("Fehler!"));
			QHBoxLayout sFDlayout(saveFailedDialog);
			QLabel *sFDLabel = new QLabel(tr("Writing failed! Check your rights for this folder!"),saveFailedDialog);
			sFDlayout.addWidget(sFDLabel);
			QPushButton *ok = new QPushButton ("OK", saveFailedDialog);
			sFDlayout.addWidget(ok);
			connect(ok, SIGNAL(clicked()), saveFailedDialog, SLOT(deleteLater()));
			return;
		}
	}
	
	//if(! QFile(save_bgi_filename).exists())
	//	{
	if(!bgi_filename.isEmpty() && scene()->backgroundBrush() != Qt::NoBrush)
	{
	qWarning() << "Copying Map Background File:" << QString(bgi_filename) << QString(save_bgi_filename).prepend("/").prepend(save_dir_string);
		if(QFile(bgi_filename).copy(QString(save_bgi_filename).prepend("/").prepend(save_dir_string)))
		{
		qWarning() << "Success!";
		}
		else qWarning() << "Failed!!";
	//	}
	}
	else
	qWarning() << "No map background set";
	QFile savefile(save_filename);
	savefile.open(QIODevice::WriteOnly);
	QTextStream savestream(&savefile);
	savestream << "<map>\n";

  savestream << "<mapbackground>";
  savestream << save_bgi_filename;
  savestream << "</mapbackground>\n";
  
    savestream << "<mapname>";
  savestream << mapname;
  savestream << "</mapname>\n";
  
  if(!cityname.isEmpty() && (maptyp == Coastcitymap || maptyp == Landcitymap))
  {
  savestream << "<cityname>";
  savestream << cityname;
  savestream << "</cityname>\n";
  }
  savestream << "<mapwidth>";
  savestream << mapSize.width();
  savestream << "</mapwidth>\n";
  savestream << "<mapheight>";
  savestream << mapSize.height();
  savestream << "</mapheight>\n";
  savestream << "<maptype>";
  savestream << maptyp;
  savestream << "</maptype>\n";
  
  qWarning() << "Mapprops written";
  //hier die Mapprops reinschreiben (Groesse, File, etc.)
//   QGraphicsItem *saveitem;
  
//   foreach(saveitem, itemList)
QList<QGraphicsItem *> ItemList = scene()->items();
if(!ItemList.isEmpty())
{
qWarning() << "List-Size: "<< ItemList.size();
// foreach(QString foritstring, itemMapList.keys())
for(QList<QGraphicsItem*>::iterator saveitem = ItemList.begin(); saveitem != ItemList.end(); ++saveitem)
 {
// 	  saveitem = itemMapList.value(foritstring);
if((*saveitem)->data(Filename).toString().endsWith(".jpg") || (*saveitem)->data(Filename).toString().endsWith(".svg") || (*saveitem)->data(Filename).toString().endsWith(".png"))
	  {
	qWarning() << "Name: " << (*saveitem)->data(Name).toString() << "ID: " << (*saveitem)->data(ID).toInt();

	 savestream << "<object>\n";
	 
	 savestream << "<objectname>";
	 savestream << (*saveitem)->data(Name).toString();
	 savestream << "</objectname>\n";
	 
	 savestream << "<objectfunktion>";
	 savestream << (*saveitem)->data(Function).toInt();
	 savestream << "</objectfunktion>\n";
	 if(!(*saveitem)->data(Tooltip).toString().isEmpty())
	 {
		 savestream << "<objecttooltip>";
		 savestream << (*saveitem)->data(Tooltip).toString();
		 savestream << "</objecttooltip>\n";
 	 }

// 		QString img_filename = (*saveitem)->data(2).toString();
		QString img_filename = (*saveitem)->data(Filename).toString();
		int n = 0;
		int y = 0;
		while(n < 1 && y < img_filename.size())
		{
			y++;
			n = img_filename.right(y).count("/");
			qWarning() << n << y;
		}
		img_filename = img_filename.right(y).prepend("img");
		qWarning() << img_filename;
		savestream << "<objectdatei>";
		savestream << img_filename;
		savestream << "</objectdatei>\n";
		
		//if(! QFile(img_filename).exists())
		//{
		qWarning() << "Copying Object File:" << (*saveitem)->data(Filename).toString() << QString(img_filename).prepend("/").prepend(save_dir_string);
// 		if(QFile((*saveitem)->data(2).toString()).copy(QString(img_filename).prepend("/").prepend(save_dir_string)))
		if(QFile((*saveitem)->data(Filename).toString()).copy(QString(img_filename).prepend("/").prepend( save_dir_string)))
		{
		qWarning() << "Success!";
		}
		else qWarning() << "Failed!";
		//}
		
	  }
	 


  	savestream << "<objectpositionx>";
 	savestream << (*saveitem)->x();
 	savestream << "</objectpositionx>\n";

 	savestream << "<objectpositiony>";
 	savestream << (*saveitem)->y();
 	savestream << "</objectpositiony>\n";
 	
 	savestream << "<objecthoehe>";
 	savestream << (*saveitem)->zValue();
 	savestream << "</objecthoehe>\n";
 	savestream << "</object>\n";
 }
 }
 savestream << "</map>";
 }
 
 }
 
 void MapFrame::loadMap(QString load_filename)
 {
 newMap();
 
	QDir dir;
	dir = QDir().current();

	QString mapdir = QFileInfo(load_filename).path().append("/");


// 	QGraphicsScene *tempsc = new QGraphicsScene;
// 	setScene(tempsc);

	
// 	QGraphicsScene *szene = scene();
// 	szene->clear();
// 	delete szene;
// 	szene = new QGraphicsScene();
	qWarning() << "Szene geloescht";



	QFile file(load_filename);		//Map-XML-Lesen
	if(file.exists())
	{
		qWarning() << "Datei existiert" ;
		enum stati	{
				null,
				objekt,
				o_fkt,
				o_tooltip,
				object_name,
				o_datei,
				o_posx,
				o_posy,
				o_hoehe,
				m_prop,			// Allgemein: Mapeigenschaften
				m_stadtname,
				m_img,
				m_nord,			//noerdliche angrenzende Map
				m_west,			//westliche a M
				m_sued,			//suedliche a M
				m_ost,			//oestliche a M
				m_grx,			//breite
				m_gry,			//hoehe
				m_typ,
				} status = null;	// status als dieses enum: zeigt an, was fuer ein Wert als naechstes ausgelesen wird

		cityname = QString();
		int ofkt = -1;				//Funktion des Objektes
		QString otooltip = QString();				//name/tooltip des objekts
		QString odatei = QString();				//name des Bildes des Objekts
		objectName = QString();
		
		int oposx = -1;				//x-Koordinate
		int oposy = -1;				//y-Koordinate
		double ohoehe = 0;
		bool reading = true;

		file.open(QIODevice::ReadOnly);
		QXmlStreamReader reader(&file);
		while (reading) 
		{
		reader.readNext();
		switch(reader.tokenType())
		{
			case QXmlStreamReader::StartElement:
			{
			qWarning() << "\nStart:\t" <<reader.qualifiedName().toString();
				if(reader.qualifiedName().toString() =="mapprop")
				{
// 				qWarning() << "Start: mapprops";
				status=m_prop;
				break;
				}
				if(reader.qualifiedName().toString() =="cityname")
				{
				status=m_stadtname;
				break;
				}
				if(reader.qualifiedName().toString() =="mapbackground")
				{
// 				qWarning() << "Start: mapprops";
				status=m_img;
				break;
				}
				if(reader.qualifiedName().toString() =="mapnorth")
				{
// 				qWarning() << "Start: mapprops";
				status=m_nord;
				break;
				}
				if(reader.qualifiedName().toString() =="mapeast")
				{
// 				qWarning() << "Start: mapprops";
				status=m_ost;
				break;
				}
				if(reader.qualifiedName().toString() =="mapsouth")
				{
// 				qWarning() << "Start: mapprops";
				status=m_sued;
				break;
				}
				if(reader.qualifiedName().toString() =="mapwest")
				{
// 				qWarning() << "Start: mapprops";
				status=m_west;
				break;
				}
				if(reader.qualifiedName().toString() =="mapwidth")
				{
// 				qWarning() << "Start: mapprops";
				status=m_grx;
				break;
				}
				if(reader.qualifiedName().toString() =="mapheight")
				{
// 				qWarning() << "Start: mapprops";
				status=m_gry;
				break;
				}

				if(reader.qualifiedName().toString() =="maptype")
				{
// 				qWarning() << "Start: mapprops";
				status=m_typ;
				break;
				}
				
				if(reader.qualifiedName().toString() =="object")
				{
// 				qWarning() << "objekt";
				status=objekt;
				break;
				}
				
				if(reader.qualifiedName().toString() =="objectname")
				{
				status = object_name;
				
				}
				if(reader.qualifiedName().toString() == "objectfunktion")
				{
// 				qWarning() << "o_fkt";
				status = o_fkt;
				break;
				}

				if(reader.qualifiedName().toString() == "objecttooltip")
				{
// 				qWarning() << "o_tooltip";
				status = o_tooltip;
				break;
				}

				if(reader.qualifiedName().toString() == "objectdatei")
				{
// 				qWarning() << "o_datei";
				status = o_datei;
				break;
				}

				if(reader.qualifiedName().toString() == "objectpositionx")
				{
// 				qWarning() << "o_posx";
				status = o_posx;
				break;
				}

				if(reader.qualifiedName().toString() == "objectpositiony")
				{
// 				qWarning() << "o_posy";
				status = o_posy;
				break;
				}
				
				if(reader.qualifiedName().toString() == "objecthoehe")
				{
// 				qWarning() << "o_posy";
				status = o_hoehe;
				break;
				}
				break;
			}

			case QXmlStreamReader::Invalid:
			{
			qWarning() << "Error:" << reader.errorString() <<"\nEnde Error" ;
			break;
			}

			case QXmlStreamReader::Characters:
			{
// 				qWarning() << "Chars:" <<reader.text().toString();
				//Tags ohne Inhalt - nur mit Unterkategorien
				switch(status)
				{
				case m_prop:
					break;
				case m_stadtname:
				{
				cityname = reader.text().toString();
				break;
				}
				case m_img:
				{
				bgi_filename = reader.text().toString();
				bgi_filename.prepend(mapdir);
 				QFile mapimgfile(bgi_filename);
 					if(mapimgfile.exists())
 					{
					scene()->setBackgroundBrush(QBrush(QImage(bgi_filename)));
					}
					else
						qWarning() << "Maphintergrund:" << bgi_filename << "nicht gefunden!" << mapdir;
				break;
				}

				case m_nord:
				{
				mapnorth = reader.text().toString();

				qWarning() << "Nord-Map:" << mapnorth;
				if(!QFile(mapnorth).exists())
					{
					qWarning() << mapnorth << "Existiert nicht";
					mapnorth = QString();
					}
				break;
				}

				case m_ost:
				{
				mapeast = reader.text().toString();
				qWarning() << "Ost-Map:" << mapeast;
				if(!QFile(mapeast).exists())
					{
					qWarning() << mapeast << "Existiert nicht";
					mapeast = QString();
					}
				break;
				}

				case m_sued:
				{
				mapsouth = reader.text().toString();
				qWarning() << "Sued-Map:" << mapsouth;
				if(!QFile(mapsouth).exists())
					{
					qWarning() << mapsouth << "Existiert nicht";
					mapsouth = QString();
					}
				break;
				}

				case m_west:
				{
				mapwest = reader.text().toString();
				qWarning() << "West-Map:" << mapwest;
				if(!QFile(mapwest).exists())
					{
					qWarning() << mapwest << "Existiert nicht";
					mapwest = QString();
					}
				break;
				}

				case m_grx:
				{
					mapSize.setWidth(reader.text().toString().toInt());
					break;
				}
				case m_gry:
				{
					mapSize.setHeight(reader.text().toString().toInt());
					break;
				}
				case m_typ:
				{
					maptyp = reader.text().toString().toInt();
					break;
				}

				case objekt:
					break;
				case o_fkt:
				{
					ofkt = reader.text().toString().toInt();
					qWarning() << "\tObjektfkt" << ofkt;
					break;
				}
				case object_name:
				{
					objectName = reader.text().toString();
					break;
				}
				
				case o_tooltip:
				{
					otooltip = reader.text().toString();
					qWarning() << "\tObjekttooltip" << otooltip;
					break;
				}
				case o_datei:
				{
					odatei = reader.text().toString();
					qWarning() << "\tBild:" << odatei;
					break;
				}
				case o_posx:
				{
					oposx = reader.text().toString().toInt();
					qWarning() << "\tPosX:" << oposx;
					break;
				}
				case o_posy:
				{
					oposy = reader.text().toString().toInt();
					qWarning() << "\tPosY" << oposy;
					break;
				}
				case o_hoehe:
				{
					ohoehe = reader.text().toString().toDouble();
					qWarning() << "\tPosZ" << ohoehe;
					break;
				}
				
				default:
					break;
				}
				break;
			}
			case QXmlStreamReader::EndElement:
			{
				qWarning() << "Ende :"<< reader.qualifiedName().toString();
				if(reader.qualifiedName().toString() == "object" && ofkt != -1 && !odatei.isEmpty())
	//jetzt zeichnen: habe alle Eigenschaften des Objektes erhalten?
				{

					odatei = odatei.prepend(mapdir);
					qWarning() << odatei << ofkt;
					QFile bild(odatei);
					if(bild.exists())
					{
						qWarning() << "Malen ....";
// #ifdef _RELEASE_
// 						if(ofkt == "Uhr")
// 						{
// 
// 						QGraphicsItem *zb = scene()->addPixmap((QPixmap(odatei)));
// 						zb->setPos(oposx,oposy);
// 						zb->setToolTip(otooltip);
// 						zb->setZValue(0.5);
// 						zb->setData(0,QVariant(QString("Ziffernblatt")));
// 						QGraphicsItem *gz = scene()-> addPixmap((QPixmap(":/img/objekte/zeiger1.png")));
// 						gz->setPos(oposx+21,oposy+4);
// 						gz->setToolTip(tr("grosser Zeiger"));
// 						gz->setZValue(2);
// 						gz->setData(0,QVariant(QString("grosser Zeiger")));
// 
// 						QGraphicsItem *kz = scene()-> addPixmap( (QPixmap(":/img/objekte/zeiger2.png")));
// 						kz->setPos(oposx+23,oposy+9);
// 						kz->setToolTip(tr("kleiner Zeiger"));
// 						kz->setZValue(1);
// 						kz->setData(0,QVariant(QString("kleiner Zeiger")));
// 						}

						object_typ = ofkt;
						object_ZValue = ohoehe;
						object_filename = odatei;
						object_tooltip = otooltip;
						ziel = QPoint(oposx, oposy);
						createObjectDialog = 0;
						createObject();
// 						}
					}
					else
					{
					qWarning() << "Bild" << ofkt << "nicht gefunden";
					}
				}
				status=null;
				break;

			}
			default:
				break;

		}
		if(reader.atEnd() || (reader.tokenType() == QXmlStreamReader::EndElement && reader.qualifiedName().toString() =="map"))
		{
		reading = false;
		}

		}
	
		if (reader.hasError()) {
		qWarning() << reader.errorString();
	}
// 	setScene(szene);
// 	delete tempsc;
	setSceneRect(0,0,mapSize.width(),mapSize.height());
	
}
 
 
 }
 
 
 void MapFrame::newObjectDialog_ext()
 {
 newObjectDialog(QPoint(100,100));
 }
 
 
 void MapFrame::newObjectDialog(QPoint destination)
 {
 ziel = destination;
 ziel.setX(ziel.x() + horizontalScrollBar()->value());
 ziel.setY(ziel.y() + verticalScrollBar()->value());
 		createObjectDialog = new QDialog();
		createObjectDialog->setModal(true);
		createObjectDialog->setWindowTitle(tr("Create new Object ..."));

		QGridLayout *cODlayout = new QGridLayout(createObjectDialog);

		QLabel *labelimg = new QLabel(createObjectDialog);
		labelimg->setText(tr("Image File"));
		cODlayout->addWidget(labelimg, 0, 0, 1, 1);

		QLabel *filelabel = new QLabel(createObjectDialog);
		filelabel->setFrameShape(QFrame::Box);
		cODlayout->addWidget(filelabel, 0, 1, 1, 3);

		QPushButton *selectFileButton = new QPushButton ("...",createObjectDialog);
		cODlayout->addWidget(selectFileButton, 0, 4, 1, 1);


		QLabel *labelfkt = new QLabel(createObjectDialog);
		labelfkt->setText(tr("Object function"));
		cODlayout->addWidget(labelfkt, 1, 0, 1, 1);

		fktComboBox = new QComboBox(createObjectDialog);
		
		QHashIterator <int, QString> it(m_MainWindow->SideBar->functionlabels());
		while(it.hasNext())
		{
			it.next();
			fktComboBox->addItem(it.value(), it.key());
		}
// 		fkt->addItems(ObjectTypeEntries);
		cODlayout->addWidget(fktComboBox, 1, 1,1,4);

		QLabel *labeltt = new QLabel(createObjectDialog);
		labeltt->setText(tr("Object tooltip (optional)"));
		cODlayout->addWidget(labeltt, 2, 0, 1, 1);

		QLineEdit *objToolTip = new QLineEdit(createObjectDialog);
		objToolTip->setMaxLength(25);
		cODlayout->addWidget(objToolTip, 2, 1, 1, 4);
		
		QSpinBox *XBox_MV = new QSpinBox(createObjectDialog);
		XBox_MV->setRange(0, mapSize.width());
		XBox_MV->setValue(ziel.x());
		
		QSpinBox *YBox_MV = new QSpinBox(createObjectDialog);
		YBox_MV->setRange(0, mapSize.height());
		YBox_MV->setValue(ziel.y());
		
		QHBoxLayout spinboxlayout;
//		spinboxlayout.addSpacing(20);
		QLabel *XLabel = new QLabel(tr("X-Pos:"),createObjectDialog);
		spinboxlayout.addWidget(XLabel);
		spinboxlayout.addWidget(XBox_MV);
//		spinboxlayout.addSpacing(40);
		QLabel *YLabel = new QLabel(tr("Y-Pos:"),createObjectDialog);
		spinboxlayout.addWidget(YLabel);
		spinboxlayout.addWidget(YBox_MV);
//		spinboxlayout.addSpacing(20);
		cODlayout->setRowStretch(3,2);
		cODlayout->addLayout(&spinboxlayout, 4, 0, 2, 5);

		QPushButton *ok = new QPushButton(tr("Ok"), createObjectDialog);
		QPushButton *abort = new QPushButton(tr("Abort"), createObjectDialog);
				
		QHBoxLayout buttonlayout;
		buttonlayout.addSpacing(20);
		buttonlayout.addWidget(ok);
		buttonlayout.addSpacing(40);
		buttonlayout.addWidget(abort);
		buttonlayout.addSpacing(20);
		cODlayout->setRowStretch(5,2);
		cODlayout->addLayout(&buttonlayout, 6, 0, 2, 5);



		createObjectDialog->setLayout(cODlayout);
		createObjectDialog->show();

		connect(ok, SIGNAL(clicked()), this, SLOT(newObject()));
		connect(abort, SIGNAL(clicked()), createObjectDialog, SLOT(close()));
		connect(abort, SIGNAL(clicked()), createObjectDialog, SLOT(deleteLater()));

		QSignalMapper *selectFileMapper = new QSignalMapper(createObjectDialog);
		selectFileMapper->setMapping(selectFileButton, NameFilters::Img);
		
		connect(selectFileButton, SIGNAL(clicked()), selectFileMapper, SLOT(map()));
		connect(selectFileMapper,SIGNAL(mapped(int)),this, SLOT(fileDialog(int)));
		
		connect(this, SIGNAL(fileStringChanged(QString)), filelabel, SLOT(setText(QString)));

		connect(fktComboBox, SIGNAL(currentIndexChanged(int)), this, SLOT(setObjectType(int)));
		connect(objToolTip, SIGNAL(textEdited(QString)), this, SLOT(setToolTipString(QString)));
		connect(XBox_MV, SIGNAL(valueChanged(int)), this, SLOT(setXPos(int)));
		connect(YBox_MV, SIGNAL(valueChanged(int)), this, SLOT(setYPos(int)));
// 		setObjectType(ot_market);
 }
 
 void MapFrame::mousePressEvent(QMouseEvent *event)
 {
 qWarning() << "MapFrame::mousePressEvent(QMouseEvent *event)";
 if(event->buttons() == Qt::LeftButton)
 {
 if(!itemGrabbed)
 {
 ziel = event->pos();
	QList<QGraphicsItem *> QGIlistAtClick = items(event->pos());
	QList<QGraphicsItem *> &gilist = QGIlistAtClick;
	
// 		 QGraphicsItem *Cit = 0;
// 	 QGIlistAtClick.clear();
// 	 QGIlistAtClick = scene()->items();
// 		foreach(Cit, QGIlistAtClick)
// 		{
// 		qWarning() << "CIT-Data(0)"<< Cit->data(0).toInt();
// 		}
	
	
	qWarning() << "items(event->pos()); - Anzahl:" <<  QGIlistAtClick.size();
	if(QGIlistAtClick.isEmpty())
	{
		object_tooltip = QString();
		object_filename = QString();
		fd_filename = QString();
		newObjectDialog(ziel);
	}
	else
	{
// 		ObjectGraphicsItem *tempogi = 0;
/*		QList<ObjectGraphicsItem *> matchingOGI;
		for(QList<QGraphicsItem *>::iterator Cit = QGIlistAtClick.begin(); Cit != QGIlistAtClick.end(); ++Cit)
// QGraphicsItem *Cit = 0;
// 		foreach(Cit, QGIlistAtClick)
		{
// 		qWarning() << "test";
 			for(QList<ObjectGraphicsItem*>::iterator it = ogilist.begin(); it != ogilist.end(); ++it)
			{
			qWarning() << "test2" << (*it)->data(0).toInt() << (*Cit)->data(0).toInt() << (*it)->pos() << (*Cit)->pos();
//  				if((*it)->pos() == (*Cit)->pos())
 				{
// 				qWarning() << "test 3";
 					if((*it)->data(0).toInt() == (*Cit)->data(0).toInt() *//*&& (*it)->toolTip() == (*Cit)->toolTip()*//*)
 					{
						qWarning() << "test 4";
 						matchingOGI << *it;
 						break;
 					}
 				}
			}
// 			tempogi = qgraphicsitem_cast<ObjectGraphicsItem*>(Cit);
// 			tempogi = dynamic_cast<ObjectGraphicsItem*>(Cit);
// 			qWarning() << "gecastet";
// 			if(tempogi != 0)
// 			{
// 			matchingOGI << tempogi;
// 			qWarning() << "added To List";
// 			}
		}
				qWarning() << "OGI-Size:" << matchingOGI.size();
		*/
		
		// ---> dynamic cast // qgraphicsitem_cast(qgi)
// 		ObjectGraphicsItem *OGI;

		if(gilist.size() == 1)
		{
			if(activeItem == gilist.first())
			{
			emit objectSelected(activeItem);
			itemGrabbed = true;
			}
			else
			{
// 			ObjectGraphicsItem *matchingOGI;
// 			for(QList<ObjectGraphicsItem>::iterator it = ogilist.begin; it != ogilist.end(); ++it)
// 			{
// 			if(it->pos() == QGIlistAtClick.first()->pos())
// 			{
// 			if(it->boundingRect() == QGIlistAtClick.first()->boundingRect() && it->toolTip() == QGIlistAtClick.first()->toolTip())
// 			{
// 			matchingOGI = &*it;
// 			break;
// 			}
// 			}
			
//  			}
			activeItem = QGIlistAtClick.first();
			
// 			activeItem = matchingOGI.first();
			emit objectSelected(activeItem);
			itemSelected = true;
			qWarning() << "Move-Item-ID "<< activeItem->data(ID).toInt();
			}
// 			QGIlistAtClick = scene()->items(curser);
// 		for(QList<QGraphicsItem *>::iterator Cit = QGIlistAtClick.begin(); Cit != QGIlistAtClick.end(); ++Cit)
// 		{
// 			qWarning() << "test2" <<  (*Cit)->data(0).toInt()  << (*Cit)->pos();
// 		}
// 		for(QList<ObjectGraphicsItem*>::iterator it = ogilist.begin(); it != ogilist.end(); ++it)
// 			{
// 				qWarning() << "test 3" << (*it)->data(0).toInt() << (*it)->pos();
// 			}

			
		}


		else if(QGIlistAtClick.size() >1)
		{
			if(gilist.contains(activeItem) && itemSelected)
			{
				itemGrabbed = true;
				
			}
			else
			{
			QDialog *selectObjectDialog = new QDialog(this);
			selectObjectDialog->setModal(true);
			
			QComboBox *objectList = new QComboBox(selectObjectDialog);
			
			QStringList objectNameList;	QGraphicsItem *GIit;
// 			ObjectGraphicsItem *ogiit;	//ObjectGraphicsItemIterator
// 			foreach(ogiit, matchingOGI)
// 			{
// 				objectNameList << QString(qgiit->data(17).toString()).append(QString("; ").append(qgiit->data(1).toString()).append(QString(" ...").append(qgiit->data(2).toString().right(15))));
// 				objectNameList << ogiit->data(Name).toString();
//  			}
			foreach(GIit, gilist)
			{
				objectNameList << GIit->data(Name).toString();
			}


			objectList->addItems(objectNameList) ;
			QPushButton *ok = new QPushButton ("Ok", selectObjectDialog);
			QHBoxLayout *sodlayout = new QHBoxLayout(selectObjectDialog);
			sodlayout->addWidget(objectList);
			sodlayout->addWidget(ok);
			selectObjectDialog->setLayout(sodlayout);
			selectObjectDialog -> show();
			connect(ok, SIGNAL(clicked()), selectObjectDialog, SLOT(deleteLater()));
			connect(ok, SIGNAL(clicked()), this, SLOT(selectObject()));
			connect(objectList, SIGNAL(activated(QString)), this, SLOT(getObjectID(QString)));
			getObjectID(objectList->currentText());
// 			qgilist = QGIlistAtClick;
			}
		}

	}
}
	else
	{
	itemGrabbed = true;
// 	itemSelected = false;
// 	emit objectMoved();
	}
}
else if(event->button() == Qt::RightButton)
{
activeItem = 0;
itemSelected = false;
}
}


void MapFrame::mouseMoveEvent(QMouseEvent *event)
{
// qWarning() << "void MapFrame::mouseMoveEvent(QMouseEvent *event)";

curser = event->pos();
// setStatusTip(QString("%1 %2").arg(curser.x(), curser.y()));

static int oldxpos, oldypos;

if(!(oldxpos == 0 && oldypos == 0) && itemGrabbed /*&& (oldtime == time(NULL) || oldtime == time(NULL) - 1)*/)
{
 	activeItem->moveBy( event->x() - oldxpos,  event->y() - oldypos);
	emit objectMoved();

}
oldxpos = event->x();
oldypos = event->y();
}


void MapFrame::mouseReleaseEvent(QMouseEvent *event)
{
if(itemGrabbed)
  {
  itemGrabbed = false;
//   itemSelected=false;
  emit objectMoved();
  }
 if(event->button() == Qt::RightButton)
 {
 itemSelected = false;
 }
}

void MapFrame::getObjectID(const QString &objname)
{
// QString copy = objname;
// int n = 0;
// while(!objname.left(n).contains(";"))
// {
// n++;
// }

qWarning() << objname/*.left(n-1)<< n*/;
objectName = objname/*.left(n-1)*/;
 QGraphicsItem *it;

 foreach(it, scene()->items())
{
	if(it->data(Name).toString() == objectName)
	{
	qWarning() << "ActiveItem->ID before" << activeItem->data(ID).toInt();
		activeItem = it;
	qWarning() << "ActiveItem->ID new" << activeItem->data(ID).toInt();
		return;
	}
}


}

void MapFrame::selectObject()
{
itemSelected = true;
 QGraphicsItem *it;
 foreach(it, scene()->items())
{
	if(it->data(Name).toString() == objectName)
	{
		activeItem = it;
		emit objectSelected(activeItem);
		return;
	}
}


// for(QList<ObjectGraphicsItem*>::iterator it = ogilist.begin(); it != ogilist.end(); ++it)
// {
// 	if((*it)->data(Name).toString() == objectName)
// 	{
// 		activeItem = *it;
// 	}
// }


}

void MapFrame::setObjectType(int typ)
{
qWarning() << "void MapFrame::setObjectType(int typ)";
object_typ = fktComboBox->itemData(typ).toInt();
}


/*void MapFrame::setObjectType(QString text)
{
qWarning() << "MapFrame::setObjectType(QString text)" << text;
if(text == ot_townhall)
{
	object_typ = 0;
}

else if(text == ot_market)
{
	object_typ = 1;
}

else if(text == ot_church)
{
	object_typ = 2;
}

else if(text == ot_port)
{
	object_typ = 3;
}

else if(text == ot_office)
{
	object_typ = 4;
}

else if(text == ot_bank)
{
	object_typ = 5;
}

else if(text == ot_tavern)
{
	object_typ = 6;
}

else if(text == ot_land)
{
	static int idi;
	object_typ = idi + 100;
	idi++;
}

else if(text == ot_land2)
{
	static int idi;
	object_typ = idi + 500;
	idi++;
}
else if(text == ot_mapdecoration)
{
	static int idi;
	object_typ = idi + 1000;
	idi++;
}
}*/

/*void MapFrame::setMapType(QString text)
{
// maptypename = text;
qWarning() << text;
	if(text == mt_sea)
	{
		maptyp = Seamap;
// 		isCity = false;
	}
	else if(text == mt_coast)
	{
		maptyp = Coastmap;
// 		isCity = false;
	}
	else if(text == mt_land)
	{
		maptyp = Landmap;
// 		isCity = false;
	}
	else if(text == mt_coast_city)
	{
		maptyp = Coastcitymap;
// 		isCity = true;
	}
	else if(text == mt_land_city)
	{
		maptyp = Landcitymap;
// 		isCity = true;
	}
}*/

void MapFrame::setXPos(int xpos)
{
qWarning() << "MapFrame::setXPos(int xpos)" << xpos;
ziel.setX(xpos);
}

void MapFrame::setYPos(int ypos)
{
qWarning() << "MapFrame::setYPos(int ypos)" << ypos;
ziel.setY(ypos);
}

void MapFrame::setToolTipString(QString ttstring)
{
qWarning() << "setToolTipString(QString ttstring)" << ttstring;
object_tooltip = ttstring;
}


void MapFrame::newObject()
{
qWarning() << "MapFrame::newObject()";
object_filename = fd_filename;
	if(!object_filename.isEmpty())
	{
		object_ZValue = 0;
		createObject();
	}
	else
	{
		QDialog *missingArgs = new QDialog(this);
		missingArgs->setModal(true);
		missingArgs->setWindowTitle(tr("Fehler!"));
		QHBoxLayout mAlayout(missingArgs);
		QLabel *mALabel = new QLabel(tr("Bitte Objekteigenschaften: \"Funktion\" und \"Bilddatei\" angeben!\n (Funktionsbox bitte einmal anklicken, um Auswahl zu treffen.)"), missingArgs);
		mAlayout.addWidget(mALabel);
		QPushButton *ok = new QPushButton ("OK", missingArgs);
		mAlayout.addWidget(ok);
		connect(ok, SIGNAL(clicked()), missingArgs, SLOT(deleteLater()));
		connect(missingArgs, SIGNAL(finished(int)), missingArgs, SLOT(deleteLater()));
		missingArgs->show();
		missingArgs->setFocus();
		missingArgs->raise();
	}
}


 void MapFrame::createObject()
 {
 qWarning() << "MapFrame::createObject()" << object_filename;
   qWarning() << "ziel" << ziel;
	delete createObjectDialog;
	
	const QString &paramstring = object_filename;
	if(QFile(paramstring).exists())
	{
		if(paramstring.endsWith(".png") || paramstring.endsWith(".jpg") || paramstring.endsWith(".jpeg") || paramstring.endsWith(".gif"))
		{
			QGraphicsPixmapItem *m_pixmapitem = scene()->addPixmap(QPixmap(paramstring));
			setGraphicsItemProperties(m_pixmapitem);
// 			QGraphicsItem *tempobj = m_pixmapitem;
// 			emit newObjectCreated(tempobj);

		}
		else if(paramstring.endsWith(".svg"))
		{
			QGraphicsSvgItem *m_svgitem = new QGraphicsSvgItem(paramstring);
			scene()->addItem(m_svgitem);
			setGraphicsItemProperties(m_svgitem);
// 			QGraphicsItem *tempobj = m_svgitem;
// 			emit newObjectCreated(tempobj);

		}
	}
	
	
	
/*	if(object_filename.endsWith(".jpg") || object_filename.endsWith(".svg") || object_filename.endsWith(".png"))
	{
	ObjectGraphicsItem *itemToAdd = new ObjectGraphicsItem(object_filename);
//   QGraphicsPixmapItem *itemtoAdd = scene()->addPixmap(QPixmap(object_filename));
		itemToAdd->setPos(ziel);
		itemToAdd->setZValue(object_ZValue);

	itemToAdd->setToolTip(object_tooltip);
	itemToAdd->setFunction(object_typ);
	static int objID;
	itemToAdd->setId(objID);
	itemToAdd->setData(0, objID);
	objID++;
	qWarning() << "SetName";
	itemToAdd->setName(QString("%1").arg(itemToAdd->data(ID).toInt()).append("; ").append(itemToAdd->data(Tooltip).toString()).append(itemToAdd->data(Filename).toString().right(14).left(10)));

	qWarning() << "Add to Scene";
	scene()->addItem(itemToAdd);
	qWarning() << "Set ActiveItem";
	activeItem = itemToAdd;
	ogilist << itemToAdd;
	qWarning() << "add To List";
	emit newObjectCreated(itemToAdd);


// QGraphicsSvgItem *svgblah = new QGraphicsSvgItem("/home/christian/Dokumente/Physik/LaTeX/absorption_angepasst.svg", blah);
// scene()->addItem(svgblah);
// svgblah->setPos(20,20);
// QGraphicsSVGItem *blah = 
	}*/
 }
 
 void MapFrame::setGraphicsItemProperties(QGraphicsItem *paramitem)
 {
 #define setData(x, y) setData(x, QVariant(y))
 static int idcounter;
  idcounter ++;
 paramitem->setData(ID, idcounter);
 paramitem->setData(Function, object_typ);
 paramitem->setData(Tooltip, object_tooltip);

 paramitem->setData(Filename, object_filename);
 paramitem->setPos(ziel);
//  paramitem->setData(

if(objectName.isEmpty())
{
objectName = QString("%1 ").arg(idcounter).append(object_tooltip).append(" ").append(QString(object_filename).right(9));
}
 paramitem->setData(Name, objectName);
	emit newObjectCreated(paramitem);

objectName = QString();
object_tooltip = QString();
object_filename = QString();

 #undef setData
 }



// void MapFrame::fileDialog(NameFilters::NFs filterarg)
void MapFrame::fileDialog(int filterarg)
{
qWarning() << "MapFrame::fileDialog(int filterarg)" << filterarg;
	fd = new QFileDialog(this, Qt::Dialog);
//	fd->setFilter(filter);
	fd->setModal(true);

	QStringList filters;
	if(filterarg == NameFilters::Img)
	{

		filters	<< "Image files (*.png *.jpg)";
//		<< "All files (*)";
	}
	else if(filterarg == NameFilters::Map)
	{
		filters	<< "OpenHanse Map files (*.ohm)";
//		<< "All files (*)";
	}
	else if(filterarg == (NameFilters::Map|NameFilters::Save))
	{
		fd->setAcceptMode(QFileDialog::AcceptSave);
		filters	<< "OpenHanse Map files (*.ohm)";
	}

	fd->setNameFilters(filters);
	fd->show();
	fd->setFocus();
	fd->raise();
	connect(fd,SIGNAL(fileSelected(QString)),this,SLOT(setFileString(QString)));
	connect(fd,SIGNAL(finished(int)),fd,SLOT(deleteLater()));
}

void MapFrame::setFileString(QString fileString)
{
qWarning() << "MapFrame::setFileString(QString fileString)" << fileString;
fd_filename = fileString;
emit fileStringChanged(fileString);
}


void MapFrame::keyPressEvent(QKeyEvent *event)
{
  if(event->key() == Qt::Key_Delete)
  {
  emit SIG_deleteObject();
  }
}
