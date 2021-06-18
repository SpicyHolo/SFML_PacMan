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

	this->pacMan = new PacMan(this->entityTexture, this->dt, 0.1f);

	//this->entities.push_back(new Entity(entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 1), 2, this->dt, sf::Vector2u(13, 14)));

	//this->entities.push_back(new Entity(entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 2), 2, this->dt, sf::Vector2u(13, 17)));

	//this->entities.push_back(new Entity(entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 3), 2, this->dt, sf::Vector2u(11, 17)));

	//this->entities.push_back(new Entity(entityTexture, sf::Vector2u(15, 15), sf::Vector2u(0, 4), 2, this->dt, sf::Vector2u(15, 17)));

	this->map = new Map(this->labyrinthTexture, 8);
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

void Game::updateKeyControl(int keyCode)
{
	switch (keyCode)
	{
	case sf::Keyboard::Up:
		this->pacMan->addDirection(PacMan::direction::UP);
		break;
	case sf::Keyboard::Down:
		this->pacMan->addDirection(PacMan::direction::DOWN);
		break;
	case sf::Keyboard::Left:
		this->pacMan->addDirection(PacMan::direction::LEFT);
		break;
	case sf::Keyboard::Right:
		this->pacMan->addDirection(PacMan::direction::RIGHT);
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
		{
			this->updateKeyControl(this->sfmlEvent.key.code);
		}

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

	window->display();
}

void Game::update()
{
	this->updateSfmlEvents();
	this->dt = this->dtClock.restart().asSeconds();
}

void Game::run()
{
	while (this->isRunning())
	{
		this->update();

		this->render();
	}
}
