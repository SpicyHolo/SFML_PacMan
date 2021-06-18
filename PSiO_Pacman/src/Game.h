#pragma once
#include "Entity.h"
#include "Map.h"
#include "Pacman.h"

class Game
{
private:

	//SFML
	sf::RenderWindow* window;
	sf::Event sfmlEvent;

	//Textures
	sf::Texture labyrinthTexture;
	sf::Texture entityTexture;

	// dt clock
	sf::Clock dtClock;
	float dt;

	//Map
	Map* map;

	//Entities
	PacMan* pacMan;

public:
	Game();
	virtual ~Game();

	void init();

	void loadTextures();

	//Manage key presses
	void updateKeyControl(int keyCode);

	//Sfml window stuff
	void updateSfmlEvents();
	bool isRunning();

	//Update
	void update();

	//Render
	void render();

	void run();
};


