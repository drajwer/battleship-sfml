#include <iostream>
#include <iomanip>
#include "ship.h"
#include "field.h"
#include "Config.h"
#include "player.h"
#include "Game.h"

using namespace std;

// Constructor


// Methods

STATUS player::WhatState(int x, int y)
{
	return Playground[y][x].state;
}

ship* player::Shoot(int i, int j)
{
	if (Playground[i][j].state == SHIP)
	{
		Playground[i][j].state = HIT;
		Playground[i][j].owner->RefShipLength() -= 1;
		if (Playground[i][j].owner->RefShipLength() == 0)
			Playground[i][j].owner->SinkShip(this);
		return Playground[i][j].owner;
	}

	else if (Playground[i][j].state == EMPTY)
	{
		Playground[i][j].state = MISS;
	}
	return nullptr;
}

bool player::SetShips()
{
	
	int k = 0;
	int y, x, temp;
	DIRECTION dir;
	for (int i = 0; i < NUMBER_OF_KINDS; i++) // loop of kinds
	{
		for (int j = 0; j < KINDS_OF_SHIPS[i]; j++)
		{
			temp = 0;
			do
			{
				y = rand() % SIZE_OF_PG;
				x = rand() % SIZE_OF_PG;
				dir = (DIRECTION)(rand() % 4);
				if (++temp > 400) return false;
			}while (!(this->Fleet[k].setShip(y, x, NUMBER_OF_KINDS - i, dir, this)));
			k++;
		}
	}
	this->nShips = k;
	return true;
}

bool player::CheckFields(int y, int x, int height, int width)
{
	for (int i = 0; i < height; i++)
		for (int j = 0; j < width; j++)
		{
			if (y + i < 0 || x + j < 0 || x + j > 9 || y + i > 9)
				continue;
			if (Playground[y + i][x + j].state != EMPTY)
				return true;
		}
	return false;
}

field& player::WhatField(int x, int y)
{
	return Playground[y][x];
}

// Computer Algorithm



bool ComputerAlgorithm::CheckField(field f)
{
	if( f.state == HIT || f.state == SINK || f.state == MISS )
		return false;
	return true;
}

ship* ComputerAlgorithm::ShootOpponent()
{
	ship* s;
	static int first_x;
	static int first_y;
	int counter = 0;
	Sleep(500); // computer is faking that it needs long time to think
	if (IsShipHit)
	{
		if (prev_ship->RefShipLength() == 0)
		{
			IsShipHit = false;
			IsDirSet = false;
			LastHit = false;
			opponent->ClearArea(prev_x, prev_y, (DIRECTION)((prev_dir + 2)%4), prev_ship);
			while (prev_x == -1 || !CheckField(opponent->WhatField(prev_x, prev_y)))
			{
				prev_x = rand() % SIZE_OF_PG;
				prev_y = rand() % SIZE_OF_PG;
				prev_dir = (DIRECTION)(rand() % 4);
				if (++counter > 1000)
				{
					MessageBox(NULL, "Too long!", "ERROR", NULL);
					return nullptr;
				}
			}
			prev_ship = opponent->Shoot(prev_y, prev_x);
			s = prev_ship;
			IsShipHit = (s != nullptr);
			if (IsShipHit)
			{
				first_x = prev_x;
				first_y = prev_y;
			}
			return s;
		}
		int tmp_x = prev_x;
		int tmp_y = prev_y;
		while (1)
		{
			switch (prev_dir)
			{
			case UP:
				prev_y--;
				break;
			case DOWN:
				prev_y++;
				break;
			case LEFT:
				prev_x--;
				break;
			case RIGHT:
				prev_x++;
				break;
			case NONE:
				cout << "Blad! NONE kierunek!" << endl;
				break;
			default:
				cout << "Blad! Zly kierunek!" << endl;
				break;
			}
			if (!(prev_x < 0 || prev_x > 9 || prev_y < 0 || prev_y > 9 || !CheckField(opponent->WhatField(prev_x, prev_y))))
				break;
			else
			{
				if (IsDirSet)
				{
					prev_x = first_x;
					prev_y = first_y;
					prev_dir = (DIRECTION)(((int)prev_dir + 2) % 4);
				}
				else
				{
					prev_x = first_x;
					prev_y = first_y;
					prev_dir = (DIRECTION)(((int)prev_dir + 1) % 4);
				}
			}
			if (++counter > 1000)
			{
				MessageBox(NULL, "Too long! Maybe wrong algotithm???", "ERROR", NULL);
				return nullptr;
			}
		}

		s = opponent->Shoot(prev_y, prev_x);
		if (IsDirSet && s == nullptr)
		{
			prev_x = first_x;
			prev_y = first_y;
			prev_dir = (DIRECTION)(((int)prev_dir + 2) % 4);
		}

		if (!IsDirSet && s == nullptr)
		{
			prev_x = first_x;
			prev_y = first_y;
			prev_dir = (DIRECTION)(((int)prev_dir + 1) % 4);
		}
		
		if(!IsDirSet)
			IsDirSet = (s != nullptr);

	}
	else
	{
		while (prev_x == -1 || !CheckField(opponent->WhatField(prev_x, prev_y)))
		{
			prev_x = rand() % SIZE_OF_PG;
			prev_y = rand() % SIZE_OF_PG;
			prev_dir = (DIRECTION)(rand() % 4);
			if (++counter > 1000)
			{
				MessageBox(NULL, "Too long!", "ERROR", NULL);
				return nullptr;
			}
		}

		prev_ship = opponent->Shoot(prev_y, prev_x);
		s = prev_ship;
		IsShipHit = (s != nullptr);
		if (IsShipHit)
		{
			first_x = prev_x;
			first_y = prev_y;
		}
	}
	LastHit = (s != nullptr);



	return s;
}