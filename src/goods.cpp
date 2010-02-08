/***************************************************************************
 *   Copyright (C) 2009 by Christian Doerffel                              *
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

#include "goods.h"

Goods::Goods()
{
  m_GoodHash[Taler] = 0;
  m_taler = &m_GoodHash[Taler];
  
  m_GoodHash[Capacity] = 0;
  m_GoodHash[Fill] = 0;
  m_GoodHash[ExchangeVolume] = 0;
  
  m_capacity = &m_GoodHash[Capacity];
  m_filling = &m_GoodHash[Fill];
  m_exchange_volume = &m_GoodHash[ExchangeVolume]; 
}

Goods::Goods(const Goods &other) :
  m_GoodHash (other.m_GoodHash)/*,
  m_WeaponHash (other.m_WeaponHash)*/
{
  m_taler = &m_GoodHash[Taler];
  
  m_capacity = &m_GoodHash[Capacity];
  m_filling = &m_GoodHash[Fill];
  m_exchange_volume = &m_GoodHash[ExchangeVolume]; 
}

void Goods::setGood(int key, int value)
{
	m_GoodHash[key] = value;
}

void Goods::addGood(int key, int value)
{
	m_GoodHash[key] += value;
}
// void Goods::setWeapon(int key, int value)
// {
// 	m_WeaponHash[key] = value;
// }

void Goods::setFilling(int filling)
{
  m_GoodHash[Fill] = filling;
// 	m_filling = param_filling;
}

void Goods::setCapacity(int capacity)
{
  m_GoodHash[Capacity] = capacity;
// 	m_capacity = param_capacity;
}

void Goods::setTaler(int taler)
{
// 	m_taler = param_taler;
	m_GoodHash[Taler] = taler;
}

void Goods::setExchangeVolume(int volume)
{
// 	m_exchange_volume = param_volume;
	m_GoodHash[ExchangeVolume] = volume;

}

QList<int> Goods::keys(int filter) const
{
  QList<int> keylist = m_GoodHash.keys();
  if(filter == TradeGoods)
  {
    int it;
    foreach(it, keylist)
    {
      if(it >= 99)
      {
	keylist.removeAll(it);
      }
    }
    return keylist;
  }
  else if(filter == WeaponGoods)
  {
    int it;
    foreach(it, keylist)
    {
      if(it < 100 || it > 199)
      {
	keylist.removeAll(it);
      }
    }
    return keylist;
  }
  else
    return keylist;
  
}

