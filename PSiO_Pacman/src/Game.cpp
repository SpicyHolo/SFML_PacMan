#include "stdafx.h"
#include "Game.h"

//Constructor and Destructor
Game::Game()
{
	this->loadTextures();
	this->initMap();

	//Initialize renderWindow
	this->window = new sf::RenderWindow(sf::VideoMode(448, 596), "PacMan");
	this->window->setFramerateLimit(288);

	this->init();
}

Game::~Game()
{
	delete this-> window;
	delete this->map;
	delete this->pacman;
	delete this->ui;
	for (auto& e : ghosts)
		delete e.second;
}

void Game::initMap()
{
	this->map = new Map(this->labyrinthTexture, 8);
}

//Initalizer functions
void Game::init()
{
	//Initialize variables
	this->dt = 0.f;
	this->timer = 0.f;
	this->frightenedTimer = 0.f;
	this->currentPhase = 0;
	this->modeSwapTimers = { 5.f, 20.f, 5.f, 20.f, 5.f, 1037.14f, 0.01f };
	this->keytime = 0.f;
	
	this->eatenDots = 0;

	//Initialize UI
	this->ui = new UI(this->entityTexture);

	//Initialize entities
	sf::Vector2u texture_size(15, 15);
	this->pacman = new Pacman(this->map, this->entityTexture, texture_size, sf::Vector2u(0, 0), 3, this->dt, sf::Vector2i(13, 26), 0.4f);
	this->ghosts["BLINKY"] = new Blinky(this->pacman, this->map, this->entityTexture, texture_size, sf::Vector2u(0, 1), 2, this->dt, sf::Vector2i(13, 14), 0.3f, sf::Vector2i(23, 4));
	this->ghosts["PINKY"] = new Pinky(this->pacman, this->map, this->entityTexture, texture_size, sf::Vector2u(0, 2), 2, this->dt, sf::Vector2i(13, 14), 0.3f, sf::Vector2i(3, 4));
	this->ghosts["INKY"] = new Inky(dynamic_cast<Blinky*>(this->ghosts["BLINKY"]), this->pacman, this->map, this->entityTexture, texture_size, sf::Vector2u(0, 3), 2, this->dt, sf::Vector2i(13, 14), 0.3f , sf::Vector2i(26, 32));
	this->ghosts["CLYDE"] = new Clyde(this->pacman, this->map, this->entityTexture, texture_size, sf::Vector2u(0, 4), 2, this->dt, sf::Vector2i(13, 14), 0.3f, sf::Vector2i(1, 32));
}

void Game::loadTextures()
{
	this->labyrinthTexture.loadFromFile("resources/labyrinth.png");
	this->entityTexture.loadFromFile("resources/things.png");
}

//Manage key presses
void Game::keyPressed(int keyCode)
{
	if (this->keytime >= 2.f)
	{
		this->keytime = 0.f;
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
}

//Sfml window stuff
void Game::updateKeytime()
{
	if (keytime < 2.f)
		keytime += 10.f * dt;
}

void Game::updateSfmlEvents()
{
	while (this->window->pollEvent(this->sfmlEvent))
	{
		if (this->sfmlEvent.type == sf::Event::Closed)
			this->window->close();

		if (this->sfmlEvent.type == sf::Event::KeyPressed)
			if (this->sfmlEvent.key.code == sf::Keyboard::Key::Escape)
				this->window->close();

			this->keyPressed(this->sfmlEvent.key.code);
	}
}

bool Game::isRunning()
{
	return this->window->isOpen();
}

void Game::updateFrightenedSwap()
{
	if (this->frightenedTimer > 0)
		this->frightenedTimer -= dt;
	else 
	{
		if (this->currentPhase % 2 == 0)
		{
			for (auto& e : ghosts)
			{
				if (e.second->getMode() == 2) //MODE::FRIGHTEND
				{
					e.second->setMode(0); //MODE::SCATTER 
				}
			}
		}
		else
		{
			for (auto& e : ghosts)
			{
				if (e.second->getMode() == 2) //MODE::FRIGHTEND
				{
					e.second->setMode(1); //MODE::SCATTER 
				}
			}
		}
	}
}


void Game::updateModeSwaps()
{
	if (this->currentPhase == this->modeSwapTimers.size() + 1)
	{
		for (auto& e : ghosts)
		{
			if(e.second->getMode() != 2) //Ignore if is in MODE::FRIGHTENED
				e.second->setMode(1); //Set to MODE::CHASE
		}
		return;
	}

	if (this->timer >= this->modeSwapTimers[this->currentPhase])
	{
		this->timer = 0.f;
		this->currentPhase++;
		if (this->currentPhase % 2 == 0)
		{
			for (auto& e : ghosts)
			{
				if (e.second->getMode() != 2) //Ignore if is in MODE::FRIGHTENED
					e.second->setMode(0); //Set to MODE::SCATTER
			}
		}
		else
		{
			for (auto& e : ghosts)
			{
				if (e.second->getMode() != 2) //Ignore if is in MODE::FRIGHTENED
					e.second->setMode(1); //Set to MODE::CHASE
			}
		}
	}

	this->timer += this->dt;
}

void Game::updateGhosts()
{
	for (auto& e : ghosts)
	{
		e.second->update();
		if (e.second->getGlobalBounds().intersects(this->pacman->getGlobalBounds()))
		{
			if (e.second->getMode() == 2) //frightened mode
			{
				e.second->setTile(sf::Vector2i(13, 14));
				//Choose left or right
				float dist_left = e.second->calcualteDistance(e.second->getTilePosition() + sf::Vector2i(-1, 0), this->pacman->getTilePosition());
				float dist_right = e.second->calcualteDistance(e.second->getTilePosition() + sf::Vector2i(1, 0), this->pacman->getTilePosition());
				if (dist_left <= dist_right)
					e.second->setFacingDirection(Directions::dir::LEFT);
				else
					e.second->setFacingDirection(Directions::dir::RIGHT);
			
				e.second->setMode(1);
				this->pacman->addScore(200);
			}
			else if (e.second->getMode() == 0 || e.second->getMode() == 1) //scatter or chase mode
			{
				this->pacman->removeHealthPoints();
				this->window->clear();
				this->reset();
			}
		}
	}
}

void Game::update()
{
	//dt Clock
	this->dt = this->dtClock.restart().asSeconds();

	//Keytime
	this->updateKeytime();

	//SFML Events
	this->updateSfmlEvents();

	//Map
	this->map->update();

	//User Interface
	this->ui->update(this->pacman->getScore(), this->pacman->getHealthPoints());

	//Pacman
	this->pacman->update();
	if (this->map->removeTictac(this->pacman->getTilePosition()))
	{
		this->pacman->addScore(50);
		this->eatenDots++;
		if (this->eatenDots >= 244)
		{
			this->winGame();
		}
	}

	if (this->map->removePowerUp(this->pacman->getTilePosition()))
	{
		this->pacman->addScore(500);
		for (auto& e : this->ghosts)
		{
			e.second->setMode(2); //Frightened mode
		}
		this->frightenedTimer = 8.f;
		this->eatenDots++;
		if (this->eatenDots >= 244)
			this->winGame();
	}

	//Frightened timer
	this->updateFrightenedSwap();

	//Mode swaps timers
	this->updateModeSwaps();

	//Ghosts
	this->updateGhosts();
}


//Render
void Game::render()
{
	window->clear();

	//Render User Interface
	this->ui->render(*this->window);

	//Render Map
	this->map->render(*this->window);

	//Render Ghosts
	for(auto &g : ghosts)
		g.second->render(*this->window);

	//Render Pacman
	this->pacman->render(*this->window);

	window->display();
}



void Game::reset()
{
	//Saving temporary data
	int temp_score = this->pacman->getScore();
	int temp_hp = this->pacman->getHealthPoints();
	int temp_eatenDots = this->eatenDots;

	//Delete dynamic objects

	delete this->ui;
	delete this->pacman;
	for (auto& e : ghosts)
		delete e.second;
	this->init();
	this->pacman->addScore(temp_score);
	this->pacman->setHealthPoints(temp_hp);
	this->eatenDots = temp_eatenDots;
}

//Run engine
void Game::run()
{
	while (this->isRunning())
	{
		if (this->pacman->getHealthPoints() <= 0)
		{
			std::cout << "YOU LOST!" << std::endl;
			this->window->close();
		}
		this->update();
		this->render();
	}

}

void Game::winGame()
{
	std::cout << "YOU HAVE WON!" << std::endl;
	std::cout << "SCORE: " << this->pacman->getScore() << std::endl;
	this->window->close();
}
