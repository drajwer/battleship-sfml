#pragma once
#include <string.h>
#include <fstream>

const int SHIP_LENGTH_MAX = 5; // maximum length of the ship
const int SIZE_OF_PG = 10; // size of the playground
const int NUMBER_OF_SHIPS = 15; // amount of player's ships on the start
const int NUMBER_OF_KINDS = 5; // amount of kinds of ships
const int KINDS_OF_SHIPS[5] = { 1,2,3,4,5 }; // array of amounts of ships of NUMBER_OF_KINDS - i squares
const bool SHIP_VISIBILITY = false;
namespace cfg
{
	const int WINDOW_WIDTH = 800;
	const int WINDOW_HEIGHT = 600;
	//const char* TITLE = "Battleship";
	const bool my_ship_visibility = false;
	const int game_window_width = 1000;
	const int game_window_height = 600;
	const double scale = 0.4;
	const int first_pg_x = game_window_width/2 - (2300*scale)/2;
	const int first_pg_y = 100;
	const int second_pg_x = 300*scale + first_pg_x + scale * 1000;
	const int second_pg_y = first_pg_y;

	const int fps = 30;

	//NETWORK
	//unsigned short port_server = 50001;
	//unsigned short port_client = 50002;

}
