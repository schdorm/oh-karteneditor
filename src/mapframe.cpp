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

//  #include <QtGui/QPixmap>
 
 
 void MapFrame::initMap()
 {
 qWarning() << "MapFrame::initMap()";
 szene = new QGraphicsScene();
 setScene(szene);
 newMap();
 ot_townhall = tr("Rathaus");
 ot_market = tr("Markt");
 ot_church = tr("Kirche");
 ot_port = tr("Hafen");
 ot_office = tr("Kontor");
 ot_bank = tr("Darlehensgeber");
 ot_tavern = tr("Kneipe");
 ot_land = tr("Landflaechen");
 ot_land2 = tr("\"boese\" Untiefen"); // Objekte, bei denen eine Kollision mit dem Schiff Schaden am Schiff verursacht.
 ot_mapdecoration = tr("Mapdeko");
 ObjectTypeEntries << ot_townhall << ot_market << ot_church << ot_port << ot_office << ot_bank << ot_tavern << ot_land << ot_land2 << ot_mapdecoration;
 
 mt_sea = tr("See/Meer");
 mt_coast = tr("Kueste");
 mt_land = tr("Land");
 mt_coast_city = tr("Kueste (Stadt)");
 mt_land_city = tr("Land (Stadt)");
 MapTypeEntries << mt_sea  << mt_coast << mt_land << mt_coast_city << mt_land_city;
 
 
 }

 void MapFrame::newMap()
 {
 maptyp = MapType::sea;
 szene->clear();
 mapSize = QSize(1000,1000);
 setSceneRect(0,0,1000,1000);
 itemSelected = false;
 szene->setBackgroundBrush(QBrush());
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
			qWarning() << "Aborted";
			}
			qWarning() << "Schleife 1";
		}
		save_bgi_filename = save_bgi_filename.right(y).prepend("img");
		n = 0;
		y = 0;
		}
		while(n < 1)
		{
			y++;
			n = save_dir_string.right(y).count("/");
			
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
	if(!bgi_filename.isEmpty())
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
	qWarning() << "No map background setted";
	QFile savefile(save_filename);
	savefile.open(QIODevice::WriteOnly);
	QTextStream savestream(&savefile);
	savestream << "<map>\n";

  savestream << "<maphintergrund>";
  savestream << save_bgi_filename;
  savestream << "</maphintergrund>\n";
  if(!cityname.isEmpty() && (maptyp == MapType::coast_city || maptyp == MapType::land_city))
  {
  savestream << "<cityname>";
  savestream << cityname;
  savestream << "</cityname>\n";
  }
  savestream << "<mapbreite>";
  savestream << mapSize.width();
  savestream << "</mapbreite>\n";
  savestream << "<maphoehe>";
  savestream << mapSize.height();
  savestream << "</maphoehe>\n";
  savestream << "<maptype>";
  savestream << maptyp;
  savestream << "</maptype>\n";
  
  //hier die Mapprops reinschreiben (Groesse, File, etc.)
  QGraphicsItem *saveitem;
  
//   foreach(saveitem, itemList)
foreach(QString foritstring, itemMapList.keys())
 {
	  saveitem = itemMapList.value(foritstring);
	 savestream << "<objekt>\n";
	 savestream << "<objektfunktion>";
	 savestream << saveitem->data(0).toString();
	 savestream << "</objektfunktion>\n";
	 if(!saveitem->data(1).toString().isEmpty())
	 {
		 savestream << "<objekttooltip>";
		 savestream << saveitem->data(1).toString();
		 savestream << "</objekttooltip>\n";
 	 }
	  {
		QString img_filename = saveitem->data(2).toString();
		int n = 0;
		int y = 0;
		while(n < 1)
		{
			y++;
			n = img_filename.right(y).count("/");
		}
		img_filename = img_filename.right(y).prepend("img");

		savestream << "<objektdatei>";
		savestream << img_filename;
		savestream << "</objektdatei>\n";
		
		//if(! QFile(img_filename).exists())
		//{
		qWarning() << "Copying Object File:" << saveitem->data(2).toString() << QString(img_filename).prepend("/").prepend(save_dir_string);
		if(QFile(saveitem->data(2).toString()).copy(QString(img_filename).prepend("/").prepend(save_dir_string)))
		{
		qWarning() << "Success!";
		}
		else qWarning() << "Failed!";
		//}
		
	  }
	 


  	savestream << "<objektpositionx>";
 	savestream << saveitem->x();
 	savestream << "</objektpositionx>\n";

 	savestream << "<objektpositiony>";
 	savestream << saveitem->y();
 	savestream << "</objektpositiony>\n";
 	
 	savestream << "<objekthoehe>";
 	savestream << saveitem->zValue();
 	savestream << "</objekthoehe>\n";
 	savestream << "</objekt>\n";
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


	QGraphicsScene *tempsc = new QGraphicsScene;
	setScene(tempsc);

	szene->clear();
	delete szene;
	qWarning() << "Szene geloescht";


	szene = new QGraphicsScene();


	QFile file(load_filename);		//Map-XML-Lesen
	if(file.exists())
	{
		qWarning() << "Datei existiert" ;
		enum stati	{
				null,
				objekt,
				o_fkt,
				o_tooltip,
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
		
		int oposx = -1;				//x-Koordinate
		int oposy = -1;				//y-Koordinate
		double ohoehe;
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
				if(reader.qualifiedName().toString() =="maphintergrund")
				{
// 				qWarning() << "Start: mapprops";
				status=m_img;
				break;
				}
				if(reader.qualifiedName().toString() =="mapnord")
				{
// 				qWarning() << "Start: mapprops";
				status=m_nord;
				break;
				}
				if(reader.qualifiedName().toString() =="mapost")
				{
// 				qWarning() << "Start: mapprops";
				status=m_ost;
				break;
				}
				if(reader.qualifiedName().toString() =="mapsued")
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
				if(reader.qualifiedName().toString() =="mapbreite")
				{
// 				qWarning() << "Start: mapprops";
				status=m_grx;
				break;
				}
				if(reader.qualifiedName().toString() =="maphoehe")
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

				if(reader.qualifiedName().toString() =="objekt")
				{
// 				qWarning() << "objekt";
				status=objekt;
				break;
				}

				if(reader.qualifiedName().toString() == "objektfunktion")
				{
// 				qWarning() << "o_fkt";
				status = o_fkt;
				break;
				}

				if(reader.qualifiedName().toString() == "objekttooltip")
				{
// 				qWarning() << "o_tooltip";
				status = o_tooltip;
				break;
				}

				if(reader.qualifiedName().toString() == "objektdatei")
				{
// 				qWarning() << "o_datei";
				status = o_datei;
				break;
				}

				if(reader.qualifiedName().toString() == "objektpositionx")
				{
// 				qWarning() << "o_posx";
				status = o_posx;
				break;
				}

				if(reader.qualifiedName().toString() == "objektpositiony")
				{
// 				qWarning() << "o_posy";
				status = o_posy;
				break;
				}
				
				if(reader.qualifiedName().toString() == "objekthoehe")
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
					szene->setBackgroundBrush(QBrush(QImage(bgi_filename)));
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
// 				qWarning() << "m_typ";
					if(reader.text().toString().toInt() == 0)
					{
						maptyp = MapType::sea;
					}
					if(reader.text().toString().toInt() == 1)
					{
						maptyp = MapType::coast;
					}
					if(reader.text().toString().toInt() == 2)
					{
						maptyp = MapType::land;
					}
					if(reader.text().toString().toInt() == 3)
					{
						maptyp = MapType::coast_city;
					}
					if(reader.text().toString().toInt() == 4)
					{
						maptyp = MapType::land_city;
					}
// 					qWarning() << maptyp << reader.text().toString();
				}

				case objekt:
					break;
				case o_fkt:
				{
					ofkt = reader.text().toString().toInt();
					qWarning() << "\tObjektfkt" << ofkt;
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
					qWarning() << "\tPosY" << ohoehe;
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
				if(reader.qualifiedName().toString() == "objekt" && ofkt != -1 && !odatei.isEmpty())
	//jetzt zeichnen: habe alle Eigenschaften des Objektes erhalten?
				{
// 						if(!odatei.contains("img"))
// 						{ odatei.prepend(mapdir);
// 						}
// 						else
// 							odatei.prepend(":");
					odatei = odatei.prepend(mapdir);
					qWarning() << odatei << ofkt;
					QFile bild(odatei);
					if(bild.exists())
					{
// 						int static i;
						qWarning() << "Malen ....";
// #ifdef _RELEASE_
// 						bool gemalt = false;
// 						if(ofkt == "Uhr")
// 						{
// 
// 						QGraphicsItem *zb = szene->addPixmap((QPixmap(odatei)));
// 						zb->setPos(oposx,oposy);
// 						zb->setToolTip(otooltip);
// 						zb->setZValue(0.5);
// 						zb->setData(0,QVariant(QString("Ziffernblatt")));
// 						QGraphicsItem *gz = szene-> addPixmap((QPixmap(":/img/objekte/zeiger1.png")));
// 						gz->setPos(oposx+21,oposy+4);
// 						gz->setToolTip(tr("grosser Zeiger"));
// 						gz->setZValue(2);
// 						gz->setData(0,QVariant(QString("grosser Zeiger")));
// 
// 						QGraphicsItem *kz = szene-> addPixmap( (QPixmap(":/img/objekte/zeiger2.png")));
// 						kz->setPos(oposx+23,oposy+9);
// 						kz->setToolTip(tr("kleiner Zeiger"));
// 						kz->setZValue(1);
// 						kz->setData(0,QVariant(QString("kleiner Zeiger")));
// 						gemalt = true;
// 						}


// 						if(!gemalt)
// 						{
// 						QGraphicsItem *geb = szene->addPixmap((QPixmap(odatei)));
// 						geb->setPos(oposx,oposy);
// 						geb->setData(0,QVariant(ofkt));
// 
// 						if(!otooltip.isEmpty())
// 						{
// 							geb->setToolTip(otooltip);
// 						}
// 
// 						ofkt = QString();
// 						otooltip = QString();
// 						odatei = QString();
// 						geb->setZValue(0.1);
						object_typ = ofkt;
						object_filename = odatei;
						object_tooltip = otooltip;
						ziel = QPoint(oposx, oposy);
						createObjectDialog = new QDialog();
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
	setScene(szene);
	delete tempsc;
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

		QComboBox *fkt = new QComboBox(createObjectDialog);
		fkt->addItems(ObjectTypeEntries);
		cODlayout->addWidget(fkt, 1, 1,1,4);

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

		connect(fkt, SIGNAL(currentIndexChanged(QString)), this, SLOT(setObjectType(QString)));
		connect(objToolTip, SIGNAL(textEdited(QString)), this, SLOT(setToolTipString(QString)));
		connect(XBox_MV, SIGNAL(valueChanged(int)), this, SLOT(setXPos(int)));
		connect(YBox_MV, SIGNAL(valueChanged(int)), this, SLOT(setYPos(int)));
		setObjectType(ot_market);
 }
 
 void MapFrame::mousePressEvent(QMouseEvent *event)
 {
 qWarning() << "MapFrame::mousePressEvent(QMouseEvent *event)";
 if(!itemSelected)
 {
 object_tooltip = QString();
 object_filename = QString();
 fd_filename = QString();
 ziel = event->pos();
	QList<QGraphicsItem *> QGIlistAtClick = items(event->pos());
	if(QGIlistAtClick.isEmpty())
	{
		newObjectDialog(ziel);
// 		createObjectDialog = new QDialog();
// 		createObjectDialog->setModal(true);
// 		createObjectDialog->setWindowTitle(tr("Create new Object ..."));
// 
// 		QGridLayout *cODlayout = new QGridLayout(createObjectDialog);
// 
// 		QLabel *labelimg = new QLabel(createObjectDialog);
// 		labelimg->setText(tr("Image File"));
// 		cODlayout->addWidget(labelimg, 0, 0, 1, 1);
// 
// 		QLabel *filelabel = new QLabel(createObjectDialog);
// 		filelabel->setFrameShape(QFrame::Box);
// 		cODlayout->addWidget(filelabel, 0, 1, 1, 3);
// 
// 		QPushButton *selectFileButton = new QPushButton ("...",createObjectDialog);
// 		cODlayout->addWidget(selectFileButton, 0, 4, 1, 1);
// 
// 
// 		QLabel *labelfkt = new QLabel(createObjectDialog);
// 		labelfkt->setText(tr("Object function"));
// 		cODlayout->addWidget(labelfkt, 1, 0, 1, 1);
// 
// 		QComboBox *fkt = new QComboBox(createObjectDialog);
// 
// //		itemfktList << townhall << market << church << port << office << bank << tavern << land << land2 << mapdecoration;
// 		fkt->addItems(itemfktList);
// 		cODlayout->addWidget(fkt, 1, 1,1,4);
// 
// 		QLabel *labeltt = new QLabel(createObjectDialog);
// 		labeltt->setText(tr("Object tooltip (optional)"));
// 		cODlayout->addWidget(labeltt, 2, 0, 1, 1);
// 
// 		QLineEdit *objToolTip = new QLineEdit(createObjectDialog);
// 		objToolTip->setMaxLength(25);
// 		cODlayout->addWidget(objToolTip, 2, 1, 1, 4);
// 		
// 		QSpinBox *XBox_MV = new QSpinBox(createObjectDialog);
// 		XBox_MV->setRange(0, mapSize.width());
// 		XBox_MV->setValue(ziel.x());
// 		
// 		QSpinBox *YBox_MV = new QSpinBox(createObjectDialog);
// 		YBox_MV->setRange(0, mapSize.height());
// 		YBox_MV->setValue(ziel.y());
// 		
// 		QHBoxLayout spinboxlayout;
// //		spinboxlayout.addSpacing(20);
// 		QLabel *XLabel = new QLabel(tr("X-Pos:"),createObjectDialog);
// 		spinboxlayout.addWidget(XLabel);
// 		spinboxlayout.addWidget(XBox_MV);
// //		spinboxlayout.addSpacing(40);
// 		QLabel *YLabel = new QLabel(tr("Y-Pos:"),createObjectDialog);
// 		spinboxlayout.addWidget(YLabel);
// 		spinboxlayout.addWidget(YBox_MV);
// //		spinboxlayout.addSpacing(20);
// 		cODlayout->setRowStretch(3,2);
// 		cODlayout->addLayout(&spinboxlayout, 4, 0, 2, 5);
// 
// 		QPushButton *ok = new QPushButton(tr("Ok"), createObjectDialog);
// 		QPushButton *abort = new QPushButton(tr("Abort"), createObjectDialog);
// 				
// 		QHBoxLayout buttonlayout;
// 		buttonlayout.addSpacing(20);
// 		buttonlayout.addWidget(ok);
// 		buttonlayout.addSpacing(40);
// 		buttonlayout.addWidget(abort);
// 		buttonlayout.addSpacing(20);
// 		cODlayout->setRowStretch(5,2);
// 		cODlayout->addLayout(&buttonlayout, 6, 0, 2, 5);
// 
// 
// 
// 		createObjectDialog->setLayout(cODlayout);
// 		createObjectDialog->show();
// 
// 		connect(ok, SIGNAL(clicked()), this, SLOT(newObject()));
// 		connect(abort, SIGNAL(clicked()), createObjectDialog, SLOT(close()));
// 		connect(abort, SIGNAL(clicked()), createObjectDialog, SLOT(deleteLater()));
// 
// 		QSignalMapper *selectFileMapper = new QSignalMapper(createObjectDialog);
// 		selectFileMapper->setMapping(selectFileButton, NameFilters::Img);
// 		
// 		connect(selectFileButton, SIGNAL(clicked()), selectFileMapper, SLOT(map()));
// 		connect(selectFileMapper,SIGNAL(mapped(int)),this, SLOT(fileDialog(int)));
// 		
// 		connect(this, SIGNAL(fileStringChanged(QString)), filelabel, SLOT(setText(QString)));
// 
// 		connect(fkt, SIGNAL(currentIndexChanged(QString)), this, SLOT(setObjectType(QString)));
// 		connect(objToolTip, SIGNAL(textEdited(QString)), this, SLOT(setToolTipString(QString)));
// 		connect(XBox_MV, SIGNAL(valueChanged(int)), this, SLOT(setXPos(int)));
// 		connect(YBox_MV, SIGNAL(valueChanged(int)), this, SLOT(setYPos(int)));
// 		setObjectType(market);
	}
	else
	{
		if(QGIlistAtClick.size() == 1)
		{
			if(activeItem == QGIlistAtClick.first())
			{
			itemSelected = true;
			
			}
			else
			{
			activeItem = QGIlistAtClick.first();
			emit objectSelected(activeItem);
			}
			
		}


		if(QGIlistAtClick.size() >1)
		{
			QDialog *selectObjectDialog = new QDialog(this);
			selectObjectDialog->setModal(true);
			QComboBox *objectList = new QComboBox(selectObjectDialog);
			QStringList objectNameList;
			foreach(activeItem, QGIlistAtClick)
			{
				objectNameList << QString(activeItem->data(17).toString()).append(QString("; ").append(activeItem->data(1).toString()).append(QString(" ...").append(activeItem->data(2).toString().right(15))));
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
			qgilist = QGIlistAtClick;
		}

	}
}
else
{
	itemSelected = false;
	emit objectMoved();
}
}


void MapFrame::mouseMoveEvent(QMouseEvent *MME)
{
curser = MME->pos();
// setStatusTip(QString("%1 %2").arg(curser.x(), curser.y()));

//qWarning() << "MapFrame::mouseMoveEvent(QMouseEvent *MME)";
static int oldxpos, oldypos;

if(!(oldxpos == 0 && oldypos == 0) && itemSelected /*&& (oldtime == time(NULL) || oldtime == time(NULL) - 1)*/)
{
 	activeItem->moveBy( MME->x() - oldxpos,  MME->y() - oldypos);
	emit objectMoved();

}
oldxpos = MME->x();
oldypos = MME->y();
}


void MapFrame::mouseReleaseEvent(QMouseEvent *event)
{
if(itemSelected)
  {
  itemSelected=false;
  emit objectMoved();
  }
 
}

void MapFrame::getObjectID(QString objname)
{
// QString copy = objname;
int n = 0;
while(!objname.left(n).contains(";"))
{
n++;
}

qWarning() << objname.left(n-1) << n;
objectName = objname.left(n-1);
}

void MapFrame::selectObject()
{
itemSelected = true;
QGraphicsItem *it;
foreach(it, qgilist)
{
if(it->data(17).toString() == objectName)
{
activeItem = it;
return;
}
}


}

void MapFrame::setObjectType(QString text)
{
qWarning() << "MapFrame::setObjectType(QString text)" << text;
if(text == ot_townhall)
{
object_typ = 0;
}
if(text == ot_market)
{
object_typ = 1;
}
if(text == ot_church)
{
object_typ = 2;
}
if(text == ot_port)
{
object_typ = 3;
}
if(text == ot_office)
{
object_typ = 4;
}
if(text == ot_bank)
{
object_typ = 5;
}
if(text == ot_tavern)
{
object_typ = 6;
}
if(text == ot_land)
{
static int idi;
object_typ = idi + 100;
idi++;
}
if(text == ot_land2)
{
static int idi;
object_typ = idi + 500;
idi++;
}
if(text == ot_mapdecoration)
{
static int idi;
object_typ = idi + 1000;
idi++;
}

}

void MapFrame::setMapType(QString text)
{
// maptypename = text;
qWarning() << text;
	if(text == mt_sea)
	{
		maptyp = MapType::sea;
// 		isCity = false;
	}
	if(text == mt_coast)
	{
		maptyp = MapType::coast;
// 		isCity = false;
	}
	if(text == mt_land)
	{
		maptyp = MapType::land;
// 		isCity = false;
	}

	if(text == mt_coast_city)
	{
		maptyp = MapType::coast_city;
// 		isCity = true;
	}

	if(text == mt_land_city)
	{
		maptyp = MapType::land_city;
// 		isCity = true;
	}
}

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
		missingArgs->show();
	}
}


 void MapFrame::createObject()
 {
 qWarning() << "MapFrame::createObject()";
 createObjectDialog->close();
 delete createObjectDialog;
  QGraphicsPixmapItem *itemtoAdd = szene->addPixmap(QPixmap(object_filename));
  itemtoAdd->setPos(ziel);
  qWarning() << "ziel" << ziel;
  itemtoAdd->setToolTip(object_tooltip);
  itemtoAdd->setData(0, QVariant(object_typ));
  itemtoAdd->setData(1, QVariant(object_tooltip));
  itemtoAdd->setData(2, QVariant(object_filename));
  itemtoAdd->setData(3, QVariant(ziel.x()));
  itemtoAdd->setData(4, QVariant(ziel.y()));
  static int objID;
  itemtoAdd->setData(17, QVariant(objID));
  objID++;
  itemtoAdd->setData(18, QVariant(itemtoAdd->data(17).toString().append("; ").append(itemtoAdd->data(1).toString()).append(itemtoAdd->data(2).toString().right(14).left(10))));

  
  pixmapItemList[itemtoAdd->data(18).toString()] = itemtoAdd;
 
//    = itemtoAdd;
    activeItem = itemtoAdd;
  itemMapList[activeItem->data(18).toString()] = activeItem;
  emit newObjectCreated(itemtoAdd);

// QGraphicsItem *blah;
//   
// QGraphicsSvgItem *svgblah = new QGraphicsSvgItem("/home/christian/Dokumente/Physik/LaTeX/absorption_angepasst.svg", blah);
// szene->addItem(svgblah);
// svgblah->setPos(20,20);
// QGraphicsSVGItem *blah = 

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
	if(filterarg == NameFilters::Map)
	{
		filters	<< "OpenHanse Map files (*.ohm)";
//		<< "All files (*)";
	}

	if(filterarg == (NameFilters::Map|NameFilters::Save))
	{
		fd->setAcceptMode(QFileDialog::AcceptSave);
		filters	<< "OpenHanse Map files (*.ohm)";
	}

	fd->setNameFilters(filters);


	fd->show();
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
