#pragma once
using namespace std;
enum STATUS { EMPTY, SHIP, MISS, HIT, SINK };

class ship;
class field
{
	STATUS state;
	ship* owner;

public:
	
	field(STATUS = EMPTY, ship* = nullptr);
	void DrawField(bool);
	STATUS& refState() { return state; }
	friend ostream& operator<<(ostream&, field);
	friend class player;
	friend class ship;
	friend class ComputerAlgorithm;

};