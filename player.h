#pragma once
#include <iostream>
#include "ship.h"
#include "field.h"
#include "Config.h"
//#include "Game.h"
using namespace std;
class player
{
	field Playground[SIZE_OF_PG][SIZE_OF_PG]; // player's board (with ships)
	ship Fleet[NUMBER_OF_SHIPS];
	int nShips;

public:
	STATUS WhatState(int, int);
	bool NotShot(int x, int y) { return WhatState(x, y) == SHIP || WhatState(x, y) == EMPTY; }
	//void DrawPlayground(bool, sf::Sprite** tab);
	bool SetShips();// returns 1 if set the ship, else returns 0
	bool CheckFields(int y,int x, int height, int width); // returns true if find any ship fields, else false
	field& WhatField(int x, int y);
	friend bool ship::setShip(int y, int x, int Length, DIRECTION dir, player* p);
	friend void ship::SinkShip(player*);
	friend class ship;
	ship* Shoot(int i, int j); // returns 1 if hit the target, else returns 0
	void ClearArea(int x, int y, DIRECTION dir, ship* SunkShip)
	{
		int length = SunkShip->getFirstLength();
		switch (dir)
		{
		case UP:
			for (int i = 0; i < length; i++)
			{
				if (x - 1 >= 0)
					Playground[y - i][x - 1].state = MISS;
				if (x + 1 < 10)
					Playground[y - i][x + 1].state = MISS;

			}
			if (y - length >= 0)
				Playground[y - length][x] = MISS;
			if (y + 1 < 10)
				Playground[y + 1][x] = MISS;
			break;
		case LEFT:
			for (int i = 0; i < length; i++)
			{
				if (y - 1 >= 0)
					Playground[y - 1][x - i].state = MISS;
				if (y + 1 < 10)
					Playground[y + 1][x - i].state = MISS;

			}
			if (x - length >= 0)
				Playground[y][x - length] = MISS;
			if (x + 1 < 10)
				Playground[y][x + 1] = MISS;
			break;
		case DOWN:
			for (int i = 0; i < length; i++)
			{
				if (x - 1 >= 0)
					Playground[y + i][x - 1].state = MISS;
				if (x + 1 < 10)
					Playground[y + i][x + 1].state = MISS;

			}
			if (y + length < 10)
				Playground[y + length][x] = MISS;
			if (y - 1 >= 0)
				Playground[y - 1][x] = MISS;
			break;
		case RIGHT:
			for (int i = 0; i < length; i++)
			{
				if (y - 1 >= 0)
					Playground[y - 1][x + i].state = MISS;
				if (y + 1 < 10)
					Playground[y + 1][x + i].state = MISS;

			}
			if (x + length < 10)
				Playground[y][x + length] = MISS;
			if (x - 1 >= 0)
				Playground[y][x - 1] = MISS;
			break;
		case NONE:
			cout << "SOMETHING WENT WRONG";
			break;
		default:
			break;
		}
	}
	int CountShips() { return nShips; }

	// TEMP
	void DrawShips() { for (int i = 0; i < nShips; i++) Fleet[i].DrawShip(); }

};

class ComputerAlgorithm
{
	player* opponent;
	int prev_x, prev_y;
	DIRECTION prev_dir;
	ship* prev_ship;
	bool IsShipHit;
	bool IsDirSet;
	bool LastHit; // true if last shoot was successful
public:
	ComputerAlgorithm(player* _opponent, int _x = -1, int _y = -1, DIRECTION _dir = NONE, bool IsShipHit = false): opponent(_opponent),prev_x(_x),prev_y(_y),prev_dir(_dir),IsShipHit(IsShipHit),IsDirSet(false) {}
	
	bool CheckField(field f);
	ship* ShootOpponent();
	friend class field;
};