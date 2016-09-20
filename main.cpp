#include <Windows.h>
#include <SFML/Graphics.hpp>
#include <SFML/Main.hpp>
#include "Game.h"
#include <time.h>



int main()
{
	srand(time(NULL));

	Game game;
	game.runGame();
	return EXIT_SUCCESS;
}