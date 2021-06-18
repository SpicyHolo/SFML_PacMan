#include "stdafx.h"
#include "Game.h"

int main()
{
	//Seed
	srand(time(NULL));

	//Ignore joystick error
	sf::err().rdbuf(NULL);

	//Create game class object
	Game game;

	//Game loop
	game.run();

	return 0;
}

