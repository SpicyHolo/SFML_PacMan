#pragma once
#include "entities/Entity.h"
#include "Map.h"
#include "entities/Pacman.h"
#include "UI.h"

#include "entities/Blinky.h"
#include "entities/Pinky.h"
#include "entities/Inky.h"
#include "entities/Clyde.h"

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
	int eatenDots;

	//UI
	UI* ui;

	//Timers
	float timer;
	int currentPhase;
	std::vector<float> modeSwapTimers;

	float frightenedTimer;

	float keytime;

public:
	enum MODE { SCATTER = 0, CHASE, FRIGHTENED, EATEN };

	//Constructor and Destructor
	Game();
	virtual ~Game();

	//Initializer functions
	void initMap();
	void init();

	void loadTextures();

	//Manage key presses
	void keyPressed(int keyCode);

	//Sfml window stuff
	void updateSfmlEvents();
	bool isRunning();

	//Update
	void updateKeytime();
	void updateFrightenedSwap();
	void updateModeSwaps();
	void updateGhosts();
	void update();

	//Render
	void render();

	//Run engine
	void winGame();
	void reset();
	void run();
};


