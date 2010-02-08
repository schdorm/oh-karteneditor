/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel                              *
 *   schdorm@googlemail.com                                                *
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

#ifndef _SettingsWidget_H
#define _SettingsWidget_H

#include <QtGui/QDialog>

class QCheckBox;
class QLabel;
class QGridLayout;

class Settingswidget : public QDialog
{
Q_OBJECT
public:
  Settingswidget();
  
signals:
  
  
private slots:
  void saveSettings();
  
private:
  QGridLayout *m_layout;
  QLabel *m_oldviewlabel;
  QCheckBox *m_oldviewcheckbox;
  QPushButton *m_accept;
  QPushButton *m_abort;


protected:

};

#endif
