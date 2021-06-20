#include "stdafx.h"
#include "Game.h"

//Constructor and Destructor
Game::Game()
{
	this->init();
	this->scatterTimer = 0.f;
	this->maxScatterTimer = 300.f;
}

Game::~Game()
{
	delete this->window;
	delete this->map;
	delete this->pacman;
	delete this->ui;
}

//Initalizer functions
void Game::init()
{
	//Initialize variables
	this->dt = 0.f;

	//Load Textures
	this->loadTextures();

	//Initialize renderWindow
	this->window = new sf::RenderWindow(sf::VideoMode(448, 596), "PacMan");

	//Initialize entities
	this->pacman = new Pacman(this->entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 0), 3, this->dt, sf::Vector2i(13, 26), 0.3f);
	
	this->ghosts["BLINKY"] = new Ghost(this->entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 1), 2, this->dt, sf::Vector2i(13, 14), 0.2f, sf::Vector2i(23, 4));

	this->ghosts["PINKY"] = new Ghost(this->entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 2), 2, this->dt, sf::Vector2i(13, 14), 0.2f, sf::Vector2i(3, 4));

	this->ghosts["INKY"] = new Ghost(this->entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 3), 2, this->dt, sf::Vector2i(13, 14), 0.2f, sf::Vector2i(26, 32));

	this->ghosts["CLYDE"] = new Ghost(this->entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 4), 2, this->dt, sf::Vector2i(13, 14), 0.2f, sf::Vector2i(1, 32));

	//Initalize map
	this->map = new Map(this->labyrinthTexture, 8);

	//Initialize UI
	this->ui = new UI(this->entityTexture);
}

void Game::loadTextures()
{
	if (!this->labyrinthTexture.loadFromFile("resources/labyrinth.png"))
	{
		std::cout << "COULD NOT LOAD TEXTURE";
		//TODO FIX ERROR CATCHING
	}

	this->entityTexture.loadFromFile("resources/things.png");
}

//Entities functions
bool Game::canPacmanMove()
{
	if (!this->pacman->getDirectionsQueue().empty())
	{
		switch (this->pacman->getDirectionsQueue().front())
		{
		case Directions::dir::UP:
			return !this->map->checkEntityBlock(this->pacman->getTilePosition().x, this->pacman->getTilePosition().y - 1);
			break;
		case Directions::dir::DOWN:
			return !this->map->checkEntityBlock(this->pacman->getTilePosition().x, this->pacman->getTilePosition().y + 1);
			break;
		case Directions::dir::LEFT:
			return !this->map->checkEntityBlock(this->pacman->getTilePosition().x - 1, this->pacman->getTilePosition().y);
			break;
		case Directions::dir::RIGHT:
			return !this->map->checkEntityBlock(this->pacman->getTilePosition().x + 1, this->pacman->getTilePosition().y);
			break;
		}
	}
	return true;
}

void Game::teleportTunnels(Entity* entity)
{
	if (entity->getTilePosition().x == 0 && entity->getTilePosition().y == 17)
		entity->setTile(sf::Vector2i(26, 17));
	else if (entity->getTilePosition().x == 27 && entity->getTilePosition().y == 17)
		entity->setTile(sf::Vector2i(1, 17));
}

//Manage key presses
void Game::keyPressed(int keyCode)
{
	switch (keyCode)
	{
	case sf::Keyboard::Up:
		this->pacman->queueDirection(Directions::dir::UP);
		break;
	case sf::Keyboard::Down:
		this->pacman->queueDirection(Directions::dir::DOWN);
		break;
	case sf::Keyboard::Left:
		this->pacman->queueDirection(Directions::dir::LEFT);
		break;
	case sf::Keyboard::Right:
		this->pacman->queueDirection(Directions::dir::RIGHT);
		break;
	}
}

//Sfml window stuff
void Game::updateSfmlEvents()
{
	while (this->window->pollEvent(this->sfmlEvent))
	{
		if (this->sfmlEvent.type == sf::Event::Closed)
			window->close();

		if (this->sfmlEvent.type == sf::Event::KeyPressed)
			this->keyPressed(this->sfmlEvent.key.code);
	}
}

bool Game::isRunning()
{
	return this->window->isOpen();
}

//Update
void Game::updatePacman()
{
	if (this->canPacmanMove()) 
		this->pacman->move();
	else
		this->pacman->stop();
	
	if (this->map->isJunction(this->pacman->getTilePosition().x, this->pacman->getTilePosition().y))
		this->pacman->stop();

	this->pacman->update();
	this->teleportTunnels(pacman);

	if (this->map->removeTictac(this->pacman->getTilePosition()))
		this->pacman->addScore(50);

	if (this->map->removePowerUp(this->pacman->getTilePosition()))
		this->pacman->addScore(500);
}

void Game::ghostDecision(Ghost* ghost)
{

	if (static_cast<int>(ghost->getScreenPosition().x + 8) % 16 == 0 && static_cast<int>(ghost->getScreenPosition().y + 8) % 16 == 0)
		{ 
		std::map<int, bool> options = 
		{ 
			{Directions::dir::UP, true},{Directions::dir::DOWN, true},{Directions::dir::LEFT, true},{Directions::dir::RIGHT, true} 
		};
	
		//Ghosts don't turn back in scatter/pursue mode
		switch (ghost->getFacingDirection())
		{
		case Directions::dir::UP:
			options[Directions::dir::DOWN] = false;
			break;
		case Directions::dir::DOWN:
			options[Directions::dir::UP] = false;
			break;
		case Directions::dir::LEFT:
			options[Directions::dir::RIGHT] = false;
			break;
		case Directions::dir::RIGHT:
			options[Directions::dir::LEFT] = false;
			break;
		}

		//Check for block on all sides
		if (this->map->checkEntityBlock(ghost->getTilePosition().x, ghost->getTilePosition().y - 1))
			options[Directions::dir::UP] = false;
		if (this->map->checkEntityBlock(ghost->getTilePosition().x, ghost->getTilePosition().y + 1))
			options[Directions::dir::DOWN] = false;
		if (this->map->checkEntityBlock(ghost->getTilePosition().x - 1, ghost->getTilePosition().y))
			options[Directions::dir::LEFT] = false;
		if (this->map->checkEntityBlock(ghost->getTilePosition().x + 1, ghost->getTilePosition().y))
			options[Directions::dir::RIGHT] = false;

		//Calculate distance
		std::map <int, float> distance = 
		{
		{Directions::dir::UP, 999},{Directions::dir::DOWN, 999},{Directions::dir::LEFT, 999},{Directions::dir::RIGHT, 999}
		};

		if (options[Directions::dir::UP])
			distance[Directions::dir::UP] = ghost->calcualteDistance(ghost->getTilePosition() + sf::Vector2i(0, -1), ghost->getDestinationTile());
		if (options[Directions::dir::DOWN])
			distance[Directions::dir::DOWN] = ghost->calcualteDistance(ghost->getTilePosition() + sf::Vector2i(0, 1), ghost->getDestinationTile());
		if (options[Directions::dir::LEFT])
			distance[Directions::dir::LEFT] = ghost->calcualteDistance(ghost->getTilePosition() + sf::Vector2i(-1, 0), ghost->getDestinationTile());
		if (options[Directions::dir::RIGHT])
			distance[Directions::dir::RIGHT] = ghost->calcualteDistance(ghost->getTilePosition() + sf::Vector2i(1, 0), ghost->getDestinationTile());
	
		float smallest_distance = 998;
		int smallest_distance_key = 0;
		for (auto& d : distance)
		{
			//DEBUG
			/*
			switch (d.first)
			{
			case Directions::dir::UP:
				std::cout << "UP  : ";
				break;
			case Directions::dir::DOWN:
				options[Directions::dir::UP] = false;
				std::cout << "DOWN: ";
				break;
			case Directions::dir::LEFT:
				options[Directions::dir::RIGHT] = false;
				std::cout << "LEFT: ";
				break;
			case Directions::dir::RIGHT:
				options[Directions::dir::LEFT] = false;
				std::cout << "RIGHT: ";
				break;
			}
			std::cout << d.second << std::endl;
			*/

			if (d.second < smallest_distance)
			{
				smallest_distance = d.second;
				smallest_distance_key = d.first;
			}
		}

		int top_priority_key = smallest_distance_key;
		for (auto& d : distance)
		{
			if (d.first == smallest_distance_key)
			{
				if (d.first < top_priority_key)
				{
					top_priority_key = d.first;
				}
			}
		}
		ghost->setFacingDirection(top_priority_key);
	}
}

void Game::updateGhost()
{
	if (!pacman->getDirectionsQueue().empty())
	{
		if (this->ghosts["BLINKY"]->getMode() == MODE::PURSUIT)
		{
			this->ghosts["BLINKY"]->setDestination(this->pacman->getTilePosition());
		}

		if (this->ghosts["PINKY"]->getMode() == MODE::PURSUIT)
		{
			switch (this->pacman->getFacingDirection())
			{
			case Directions::dir::UP:
				this->ghosts["PINKY"]->setDestination(this->pacman->getTilePosition() + sf::Vector2i(-4, -4));
				break;
			case Directions::dir::DOWN:
				this->ghosts["PINKY"]->setDestination(this->pacman->getTilePosition() + sf::Vector2i(0, 4));
				break;
			case Directions::dir::LEFT:
				this->ghosts["PINKY"]->setDestination(this->pacman->getTilePosition() + sf::Vector2i(-4, 0));
				break;
			case Directions::dir::RIGHT:
				this->ghosts["PINKY"]->setDestination(this->pacman->getTilePosition() + sf::Vector2i(4, 0));
				break;
			}
		}

		//INKY DESTINATION 
		//Vector from pacman to blinky rotated by 180 degress

		if (this->ghosts["INKY"]->getMode() == MODE::PURSUIT)
		{
			sf::Vector2i inky_destination(0,0), 
				pacman_to_blinky(0, 0), 
				pacman_position(0, 0), 
				blinky_position(this->ghosts["BLINKY"]->getTilePosition());

			switch (this->pacman->getFacingDirection())
			{
			case Directions::dir::UP:
				pacman_position = this->pacman->getTilePosition() + sf::Vector2i(-2, -2);
				break;
			case Directions::dir::DOWN:
				pacman_position = this->pacman->getTilePosition() + sf::Vector2i(0, 2);
				break;
			case Directions::dir::LEFT:
				pacman_position = this->pacman->getTilePosition() + sf::Vector2i(-2, 0);
				break;
			case Directions::dir::RIGHT:
				pacman_position = this->pacman->getTilePosition() + sf::Vector2i(2, 0);
				break;
			}

			pacman_to_blinky =  blinky_position - pacman_position;
			inky_destination = pacman_position - pacman_to_blinky;
			//Rotate by 180deg
			this->ghosts["INKY"]->setDestination(inky_destination);
			std::cout << "x: " << inky_destination.x << " y: " << inky_destination.y << std::endl;
		}

		if (this->ghosts["CLYDE"]->getMode() == MODE::PURSUIT)
		{
			if (this->ghosts["CLYDE"]->calcualteDistance(this->ghosts["CLYDE"]->getTilePosition(), this->pacman->getTilePosition()) >= 8)
				this->ghosts["CLYDE"]->setDestination(this->pacman->getTilePosition());
			else
				this->ghosts["CLYDE"]->setDestination(this->ghosts["CLYDE"]->getScatterTile());
		}
	}
	for (auto& g : ghosts)
	{
		this->ghostDecision(g.second);
		g.second->update();
		g.second->move();
		this->teleportTunnels(g.second);
	}
}

void Game::update()
{
	this->updateSfmlEvents();
	this->dt = this->dtClock.restart().asSeconds();
	this->map->update();
	this->ui->update(this->pacman->getScore(), this->pacman->getHealthPoints());
	this->updatePacman();

	if (this->scatterTimer >= this->maxScatterTimer)
		for (auto& g : ghosts)
		{
			g.second->setMode(MODE::PURSUIT);
			this->maxScatterTimer = -1;
			this->scatterTimer = 0;
		}
	else
	{
		if (this->scatterTimer >= 0)
			this->scatterTimer += 100.f * this->dt;
	}

	this->updateGhost();
	blinky[0].position = sf::Vector2f(this->ghosts["BLINKY"]->getTilePosition().x *16.f , this->ghosts["BLINKY"]->getTilePosition().y *16.f);
	blinky[0].color = sf::Color::Red;

	blinky[1].position = sf::Vector2f(this->ghosts["BLINKY"]->getDestinationTile().x * 16.f, this->ghosts["BLINKY"]->getDestinationTile().y * 16.f);
	blinky[1].color = sf::Color::Red;

	pinky[0].position = sf::Vector2f(this->ghosts["PINKY"]->getTilePosition().x * 16.f, this->ghosts["PINKY"]->getTilePosition().y * 16.f);
	pinky[0].color = sf::Color(255, 105, 180);

	pinky[1].position = sf::Vector2f(this->ghosts["PINKY"]->getDestinationTile().x * 16.f, this->ghosts["PINKY"]->getDestinationTile().y * 16.f);
	pinky[0].color = sf::Color(255, 105, 180);

	inky[0].position = sf::Vector2f(this->ghosts["INKY"]->getTilePosition().x * 16.f, this->ghosts["INKY"]->getTilePosition().y * 16.f);
	inky[0].color = sf::Color::Cyan;

	inky[1].position = sf::Vector2f(this->ghosts["INKY"]->getDestinationTile().x * 16.f, this->ghosts["INKY"]->getDestinationTile().y * 16.f);
	inky[1].color = sf::Color::Cyan;

	clyde[0].position = sf::Vector2f(this->ghosts["CLYDE"]->getTilePosition().x * 16.f, this->ghosts["CLYDE"]->getTilePosition().y * 16.f);
	clyde[0].color = sf::Color(255, 165, 0);

	clyde[1].position = sf::Vector2f(this->ghosts["CLYDE"]->getDestinationTile().x * 16.f, this->ghosts["CLYDE"]->getDestinationTile().y * 16.f);
	clyde[1].color = sf::Color(255, 165, 0);
}

//Render
void Game::render()
{
	window->clear();

	//Render stuff here
	this->ui->render(*this->window);
	this->map->render(*this->window);
	this->pacman->render(*this->window);

	for(auto &g : ghosts)
		g.second->render(*this->window);
	//window->draw(this->blinky, 2, sf::Lines);
	//window->draw(this->pinky, 2, sf::Lines);
	//window->draw(this->inky, 2, sf::Lines);
	window->draw(this->clyde, 2, sf::Lines);

	window->display();
}

//Run engine
void Game::run()
{
	while (this->isRunning())
	{
		this->update();

		this->render();
	}
}
