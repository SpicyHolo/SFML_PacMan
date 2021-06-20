#pragma once
#include "Entity.h"
#include "Map.h"
#include "Pacman.h"
#include "UI.h"
#include "Ghost.h"

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
	std::map<std::string, Ghost*> ghosts;

	//UI
	UI* ui;

	//Timers
	float scatterTimer;
	float maxScatterTimer;

	//Vertexes
	sf::Vertex blinky[2];
	sf::Vertex pinky[2];
	sf::Vertex inky[2];
	sf::Vertex clyde[2];

public:
	enum MODE { SCATTER = 0, PURSUIT, FRIGHTENED, EATEN };

	//Constructor and Destructor
	Game();
	virtual ~Game();

	//Initializer functions
	void init();

	void loadTextures();

	//Entities functions
	bool canPacmanMove();
	void teleportTunnels(Entity* entity);

	//Manage key presses
	void keyPressed(int keyCode);

	//Sfml window stuff
	void updateSfmlEvents();
	bool isRunning();

	//Update
	void updatePacman();
	void ghostDecision(Ghost* ghost);
	void updateGhost();
	void update();

	//Render
	void render();

	//Run engine
	void run();
};


