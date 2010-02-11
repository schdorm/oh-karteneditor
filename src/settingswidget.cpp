/***************************************************************************
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

#include <QtGui/QPushButton>
#include <QtGui/QGridLayout>
#include <QtGui/QLabel>
#include <QtGui/QCheckBox>

#include <QtCore/QFile>
#include <QtXml/QXmlStreamWriter>


#include "settingswidget.h"
#include "settings.h"



Settingswidget::Settingswidget()
{
  m_layout = new QGridLayout(this);

  m_oldviewlabel = new QLabel(tr("Old mapeditor layout"), this);
  m_layout->addWidget(m_oldviewlabel, 1, 1, 1, 1, Qt::AlignHCenter);

  m_oldviewcheckbox = new QCheckBox(this);
  m_layout->addWidget(m_oldviewcheckbox, 1, 2, 1, 1, Qt::AlignHCenter);
  m_oldviewcheckbox->setChecked(SETTINGS->oldlayout());
  
  m_autosaveLabel = new QLabel(tr("Enable Autosave"), this);
  m_layout->addWidget(m_autosaveLabel, 2, 1, 1, 1, Qt::AlignHCenter);

  m_autosaveCheckBox = new QCheckBox(this);
  m_layout->addWidget(m_autosaveCheckBox, 2, 2, 1, 1, Qt::AlignHCenter);
  m_autosaveCheckBox->setChecked(SETTINGS->autosaveEnabled());


  m_accept = new QPushButton (tr("Accept"),this);
  m_layout->addWidget(m_accept, 3, 1, Qt::AlignHCenter);

  m_abort = new QPushButton(tr("Abort"), this);
  m_layout->addWidget(m_abort, 3, 2, Qt::AlignHCenter);
  
  connect(m_accept, SIGNAL(clicked()), this, SLOT(accept()));
  connect(this, SIGNAL(accepted()), this, SLOT(saveSettings()));
  
  connect(m_abort, SIGNAL(clicked()), this, SLOT(reject()));

}


void Settingswidget::saveSettings()
{
  QFile file(SETTINGS->settingsfilepath());
  file.open(QIODevice::WriteOnly);
  QXmlStreamWriter writer(&file);
  writer.setAutoFormatting(true);
  writer.writeStartDocument();
  writer.writeStartElement("settings");
  writer.writeEmptyElement("oldlayout");
  if(m_oldviewcheckbox->isChecked())
    writer.writeAttribute(QXmlStreamAttribute("value","true"));
  else
    writer.writeAttribute(QXmlStreamAttribute("value","false"));
  writer.writeEmptyElement("autosave");
  if(m_autosaveCheckBox->isChecked())
    writer.writeAttribute(QXmlStreamAttribute("value","true"));
  else
    writer.writeAttribute(QXmlStreamAttribute("value","false"));
  writer.writeEndDocument();
  file.close();
  SETTINGS->readSettings();
//   deleteLater();
}

