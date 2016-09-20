#include <iostream>
#include <iomanip>
#include <windows.h>
#include "ship.h"
#include "field.h"
#include "Config.h"
#include "player.h"
#include "Game.h"
using namespace std;
HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);


field::field(STATUS s, ship* owner)
{
	state = s;
	this->owner = owner;
}
ostream& operator<<(ostream& out, field F)
{
	switch (F.state)
	{
		case EMPTY: out << "-"; break;
		case SHIP: out << "0"; break;
		case HIT: out << "#"; break;
		case MISS: out << "*"; break;
		case SINK: out << "@"; break;
		default: out << "!"; break;

	}
	return out;
}
void field::DrawField(bool visibility)
{
	cout << setw(3);
	switch (state)
	{
	case EMPTY: cout << "-"; break;
	case SHIP: 
	{
		if (visibility)
			cout << "0";
		else
			cout << "-"; 
		break;
	}
	case HIT:
	{
		SetConsoleTextAttribute(hOut, 0x0006);
		cout << "#";
		SetConsoleTextAttribute(hOut, 0x0007);
		break;
	}
	case MISS: 
	{
		SetConsoleTextAttribute(hOut, 0x0008);
		cout << "*";
		SetConsoleTextAttribute(hOut, 0x0007);
		break;
	}
	case SINK:
	{
		SetConsoleTextAttribute(hOut, 0x0004);
		cout << "@"; 
		SetConsoleTextAttribute(hOut, 0x0007);
		break;
	}
	default: 
	{
		SetConsoleTextAttribute(hOut, 0x0005);
		cout << "!";
		SetConsoleTextAttribute(hOut, 0x0007);
		break;
	}

	}
}