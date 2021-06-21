#include "stdafx.h"
#include "Game.h"

/*
TODO LIST

Gameplay
- Add multiple levels
- Difficulty scaling with levels
- Ghosts actually spawning inside the base


Graphics and sounds
- Sounds :)
- Death animation
- Flickering of ghosts to white just before changing back from STATE::FRIGHTENED

Misc
- Game Menu
- Keeping High Score
- Global Player Ranking with MongoDB

BUGS
- When ghosts entering MODE::FRIGHTENED they turn 180 degress
	When a wall is behind them during that state, they can glitch into a wall.
	//Temporary fix: During entering the frightened stage - checking if a tile behind them isSolid(), if true don't turn around.
*/


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
	
	std::cin.get();
	return 0;
}

