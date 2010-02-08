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


#include "gameparameter.h"
#include "goods.h"
// #include "building.h"
#include "settings.h"

GameParameter * GameParameter::m_instance;


GameParameter * GameParameter::instance()
{
  if(m_instance == NULL)
  {
    m_instance = new GameParameter();
  }
  return m_instance;
}


GameParameter::GameParameter()
{
// GoodLabelHash.insert();
m_GoodLabelHash.insert(Goods::Trunks, tr("trunks"));		//Baumstaemme
m_GoodLabelHash.insert(Goods::Shelves,tr("shelves"));		//Holzbretter
m_GoodLabelHash.insert(Goods::Charcoal, tr("charcoal"));	//Holzkohle
m_GoodLabelHash.insert(Goods::Pitch, tr("pitch"));		//Pech
m_GoodLabelHash.insert(Goods::Bricks, tr("bricks"));		//Steine/Ziegel
m_GoodLabelHash.insert(Goods::IronOre, tr("iron ore"));		//Eisenerz
m_GoodLabelHash.insert(Goods::MalleableIron, tr("malleable iron"));	//Schmiedeeisen
m_GoodLabelHash.insert(Goods::Tools, tr("tools"));		//Werkzeuge
m_GoodLabelHash.insert(Goods::Leather, tr("leather"));		//Leder
m_GoodLabelHash.insert(Goods::Wool, tr("wool"));		//Wolle
m_GoodLabelHash.insert(Goods::Cloth, tr("cloth"));		//Stoff
m_GoodLabelHash.insert(Goods::Hemp, tr("hemp"));		//Hanf

m_GoodLabelHash.insert(Goods::Beer, tr("beer"));	//Bier
m_GoodLabelHash.insert(Goods::Cereal, tr("cereal"));	//Getreide
m_GoodLabelHash.insert(Goods::Bread, tr("bread"));	//Brot
m_GoodLabelHash.insert(Goods::Salt, tr("salt"));	//Salz
m_GoodLabelHash.insert(Goods::Meat, tr("meat"));	//Fleisch
m_GoodLabelHash.insert(Goods::Fish, tr("fish"));	//Fisch
m_GoodLabelHash.insert(Goods::Cheese, tr("cheese"));	//Kaese
m_GoodLabelHash.insert(Goods::Wine, tr("wine"));	//Wein
m_GoodLabelHash.insert(Goods::Spicery, tr("spicery"));	//Gewuerze
m_GoodLabelHash.insert(Goods::Gold, tr("gold"));	//Gold
m_GoodLabelHash.insert(Goods::Jewellery, tr("jewellery"));//Schmuck


m_BuildingLabelHash[Building::Factory] = tr("Factory");
m_BuildingLabelHash[Building::Townhall] = tr("Townhall");
// m_BuildingLabelHash[]
}


