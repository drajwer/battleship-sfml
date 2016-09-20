#include <iostream>
#include "ship.h"
#include "field.h"
#include "Config.h"
#include "player.h"
using namespace std;

bool ship::setShip(int y, int x, int Length, DIRECTION dir, player* p)
{
	ShipDir = dir;
	ship_x = x;
	ship_y = y;
	switch (dir)
	{
	case UP: 
		{
			if (p->CheckFields(y - Length + 1, x - 1, Length, 3)) return false; // checks the rectangle around the ship
			if (p->Playground[y + 1][x].state != EMPTY || p->Playground[y - Length][x].state != EMPTY) // checks the bound points of the ship
				return false;
			if (y - Length + 1 < 0) return false; // checks if the ship fits in the playground
			for (int i = 0; i < Length; i++)
			{
				p->Playground[y - i][x].state = SHIP;
				p->Playground[y - i][x].owner = this;
				this->T[i] = &(p->Playground[y - i][x]);
				this->ShipLength = Length;
			}
			setCurrentLength();
			return true;
		break;
		}	
	

	case DOWN:
	{
		if (p->CheckFields(y, x - 1, Length, 3)) return false;
		if (p->Playground[y - 1][x].state != EMPTY || p->Playground[y + Length][x].state != EMPTY)
			return false;
		if (y + Length - 1> 9) return false;
		for (int i = 0; i < Length; i++)
		{
			p->Playground[y + i][x].state = SHIP;
			p->Playground[y + i][x].owner = this;
			this->T[i] = &(p->Playground[y + i][x]);
			this->ShipLength = Length;
		}
		setCurrentLength();
		return true;
		
	}

	case LEFT:
	{
		if (p->CheckFields(y - 1, x - Length + 1, 3, Length)) return false;
		if (p->Playground[y][x + 1].state != EMPTY || p->Playground[y][x - Length].state != EMPTY)
			return false;
		if (x - Length + 1 < 0) return false;
		for (int i = 0; i < Length; i++)
		{
			p->Playground[y][x - i].state = SHIP;
			p->Playground[y][x - i].owner = this;
			this->T[i] = &(p->Playground[y][x - i]);
			this->ShipLength = Length;
		}
		setCurrentLength();
		return true;
	}

	case RIGHT:
	{
		if (p->CheckFields(y - 1, x, 3, Length)) return false;
		if (p->Playground[y][x - 1].state != EMPTY || p->Playground[y][x + Length].state != EMPTY)
			return false;
		if (y + Length - 1> 0) return false;
		for (int i = 0; i < Length; i++)
		{
			p->Playground[y][x + i].state = SHIP;
			p->Playground[y][x + i].owner = this;
			this->T[i] = &(p->Playground[y][x + i]);
			this->ShipLength = Length;
		}
		setCurrentLength();
		return true;
	}

	}
	ShipDir = NONE;
	ship_x = -1;
	ship_y = -1;
	return false;
}

void ship::SinkShip(player* p)
{
	for (int i = 0; i < ShipLength; i++)
	{
		this->T[i]->state = SINK;
	}
	p->ClearArea(ship_x, ship_y, this->ShipDir, this);
	p->nShips--;
}

ship & ship::operator=(ship &s)
{
	ShipLength = s.ShipLength;
	CurrentShipLength = s.ShipLength;
	ShipDir = s.ShipDir;
	ship_x = s.ship_x;
	ship_y = s.ship_y;
	field* T = new field[SHIP_LENGTH_MAX];
	return *this;
}
sf::Packet& operator <<(sf::Packet& packet, const ship& m)
{
	return packet << m.ship_x << m.ship_y << (int)m.ShipDir << m.CurrentShipLength << m.ShipLength;
}
sf::Packet& operator >>(sf::Packet& packet, ship& m)
{
	int tmp;
	return packet >> m.ship_x >> m.ship_y >> m.tmpdir >> m.CurrentShipLength >> m.ShipLength;
}