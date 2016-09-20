#pragma once
#include "field.h"
#include "Config.h"
#include "SFML\Network.hpp"

using namespace std;
enum DIRECTION { UP, LEFT, DOWN, RIGHT, NONE};
class ship
{
	int ShipLength;
	int CurrentShipLength;
	DIRECTION ShipDir;
	int ship_x;
	int ship_y;
	int tmpdir;
	field* T[SHIP_LENGTH_MAX];

public:
	bool setShip(int, int, int, DIRECTION, player*);
	int getX() { return ship_x; }
	int getY() { return ship_y; }
	void setCurrentLength() { CurrentShipLength = ShipLength; }
	int& RefShipLength() { return CurrentShipLength;}
	int getFirstLength() { return ShipLength; }
	void SinkShip(player*);
	ship& operator=(ship&); // ACHTUNG BEZ TABLICY FIELDOW !!!!
	friend sf::Packet& operator <<(sf::Packet& packet, const ship& m);
	friend sf::Packet& operator >>(sf::Packet& packet, ship& m);
	friend void ClearArea(ship&, field**);

	//TEMP

	void DrawShip() { cout << ship_x << " " << ship_y << " " << ShipDir << " " << ShipLength << endl; }

};

