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

#ifndef _goods_h
#define _goods_h

#include <QtCore/QHash>

const int const_warenanzahl = 30;
const int const_goodnumber = 30;
// const int const_waffen = 15;

// const int const_grundpreis[const_warenanzahl] = {50, 95, 84, 67, 10000, 350, 950, 168, 270, 400, 107, 60, 130, 210, 30, 50, 300, 190, 3800, 5100, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30};

#define GOODLABEL(x) 


class Goods
{
  public:
    Goods();
    Goods(const Goods *);

    enum GOOD
    {
      Trunks = 0,
      Shelves = 1,
      Charcoal = 2,
      Pitch = 3,
      Bricks = 4,
      IronOre = 5,
      MalleableIron = 6,
      Tools = 7,
      Leather = 8,
      Wool = 9,
      Cloth = 10,
      Hemp = 11,
      _platzhalter12_,
      _platzhalter13_,
      _platzhalter14_,
      
      Beer = 15,
      Cereal = 16,
      Bread = 17,
      Salt = 18,
      Meat = 19,
      Fish = 20,
      Cheese = 21,
      Wine = 22,
      Spicery = 23,
      Gold = 24,
      Jewellery = 25,
      _platzhalter26_gold_,
      _platzhalter27_gold_,
      _platzhalter28_gold_,
      _platzhalter29_gold_,
      Taler = 99,
      
    };
    enum
    {
      ExchangeVolume = 900,
      Charge = 901,
      Fill = Charge,
      Capacity = 902,
      TradeGoods = 910,
      WeaponGoods = 911
      
    };
    
    enum WEAPON
    {
      wp_entermesser = 100,
      wp_kanone = 101,
      wp_balliste = 102,
      wp_blide = 103,
      
      wp_schwert = 110,
      wp_dolch = 111,
      wp_streitaxt = 112,
      wp_lanze = 113,
      
      wp_bogen = 120,
      wp_armbrust = 121,
      wp_muskete = 122,
      
      wp_l_armor = 125,
      wp_h_armor = 126,
      
    };
    enum Productionlevel
    {
      null = 0,
      NoProduction = 255,
      LowProduction = 256,		// 3		-- 186,67% of low production needed for normal demand
      NormalProduction = 260,		// 7		-- goods demand requires 80% of normal Production	
      HighProduction = 264,		// 10		-- 56 % of high production needed for normal demand
    };

void setGood(int, int);
void addGood(int, int);
void setFilling(int);
void setCapacity(int);
void setTaler(int);
void setExchangeVolume(int);

const QHash <int, int> &goods() const	{	return m_GoodHash;		}
int good (int key) 		const	{	return m_GoodHash.value(key);	}
int value(int key) 		const	{	return m_GoodHash.value(key);	}

// int &good (int key)			{	return m_GoodHash[key];	}

int taler	() const	{	return m_GoodHash[Taler];		}
int capacity	() const	{	return m_GoodHash[Capacity];		}
int filling	() const	{	return m_GoodHash[Fill];		}
int charge	() const 	{	return m_GoodHash[Charge];		}
int exchangeVolume() const	{	return m_GoodHash[ExchangeVolume];	}
int &exchangeVolume()		{	return m_GoodHash[ExchangeVolume];	}

// int *exchangeVolume()		{	return m_GoodHash[exchange_volume;	}


int &operator[](int key)
{
  return m_GoodHash[key];
}

int operator[](int key) const
{
  return m_GoodHash.value(key);
}


QList<int> keys(int filter = 0) const;


private:
  int m_id;
QHash<int, int> m_GoodHash;
// int *m_taler;
// int *m_capacity;
// int *m_filling;

// int *m_exchange_volume;
};

#endif

