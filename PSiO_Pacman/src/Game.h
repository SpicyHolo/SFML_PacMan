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
	Pacman* pacman;
	
public:
	Game();
	virtual ~Game();

	void init();

	void loadTextures();

	//Entities
	bool canPacmanMove();
	void teleportTunnels(Entity* entity);

	//Manage key presses
	void keyPressed(int keyCode);

	//Sfml window stuff
	void updateSfmlEvents();
	bool isRunning();

	//Update
	void updatePacman();
	void update();

	//Render
	void render();

	//Run engine
	void run();
};


