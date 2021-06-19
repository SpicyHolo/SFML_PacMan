#include "stdafx.h"
#include "Game.h"

Game::Game()
{
	this->init();
}

Game::~Game()
{
	delete this->window;
}

void Game::init()
{
	//Initialize variables
	this->dt = 0.f;

	//Load Textures
	this->loadTextures();

	//Initialize renderWindow
	this->window = new sf::RenderWindow(sf::VideoMode(448, 596), "PacMan");

	//Initialize entities
	this->pacman = new Pacman(this->entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 0), 3, this->dt, sf::Vector2i(13, 26), 0.4f);

	//this->entities.push_back(new Entity(entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 1), 2, this->dt, sf::Vector2u(13, 14)));

	//this->entities.push_back(new Entity(entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 2), 2, this->dt, sf::Vector2u(13, 17)));

	//this->entities.push_back(new Entity(entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 3), 2, this->dt, sf::Vector2u(11, 17)));

	//this->entities.push_back(new Entity(entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 4), 2, this->dt, sf::Vector2u(15, 17)));

	this->map = new Map(this->labyrinthTexture, 8);
}

//Load textures
void Game::loadTextures()
{
	if (!this->labyrinthTexture.loadFromFile("resources/labyrinth.png"))
	{
		std::cout << "COULD NOT LOAD TEXTURE";
		//TODO FIX ERROR CATCHING
	}

	this->entityTexture.loadFromFile("resources/things.png");
}

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

void Game::render()
{
	window->clear();
	
	//Render stuff here
	this->map->render(*this->window);
	this->pacman->render(*this->window);

	window->display();
}

void Game::updatePacman()
{
	if (this->canPacmanMove()) 
		this->pacman->move();
	else
		this->pacman->stop();
	
	if (this->map->isJunction(this->pacman->getTilePosition().x, this->pacman->getTilePosition().y))
		this->pacman->stop();

	this->teleportTunnels(pacman);
	this->pacman->update();
	this->map->removeTictac(this->pacman->getTilePosition());
}

void Game::update()
{
	this->updateSfmlEvents();
	this->dt = this->dtClock.restart().asSeconds();
	this->map->update();
	this->updatePacman();

}

void Game::run()
{
	while (this->isRunning())
	{
		this->update();

		this->render();
	}
}
