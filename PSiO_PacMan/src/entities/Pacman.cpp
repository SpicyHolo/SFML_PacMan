#include "../stdafx.h"
#include "Pacman.h"

//Constructor and destructor
Pacman::Pacman(Map* map, sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal, float& dt, const sf::Vector2i& tile_position, const float& velocity)
	: Entity(map, texture, textureSize, startingTexturePos, framesTotal, dt, tile_position, velocity)
{
	//Health points and score
	this->score = 0;
	this->healthPoints = 3;
	this->sprite.setTextureRect(this->idleFrame);
	this->queueDirection(Directions::dir::LEFT);
}

Pacman::~Pacman() {}

//Movement
void Pacman::queueDirection(const Directions::dir& direction) //Adds direction to queue
{
	//You can only queue two directions
	if (!this->directionsQueue.empty())
	{
		if (direction == -this->directionsQueue.front())
		{
			this->directionsQueue = {}; //Clear queue
		}
	}

	if (this->directionsQueue.size() < 2)
		this->directionsQueue.push(direction);
}

//Called when pacman is blocked by a tile or gets to a Junction
//Moves the second element in the queue to the front (if it exists)

void Pacman::stop()
{
	if (this->directionsQueue.size() > 1)
	{
		if (static_cast<int>(this->screenPosition.x + 8) % 16 == 0 && static_cast<int>(this->screenPosition.y + 8) % 16 == 0)
		{
			switch (this->directionsQueue.front())
			{
			case Directions::dir::UP:
				this->directionsQueue.pop();
				break;
			case Directions::dir::DOWN:
				this->directionsQueue.pop();
				break;
			case Directions::dir::LEFT:
				this->directionsQueue.pop();
				break;
			case Directions::dir::RIGHT:
				this->directionsQueue.pop();
				break;
			}
		}
	}
}

void Pacman::move()
{
	if(!this->directionsQueue.empty())
		this->facingDirection = this->directionsQueue.front();

	switch (this->facingDirection)
	{
	case Directions::dir::UP:
		this->screenPosition.y -= this->velocity;
		break;
	case Directions::dir::DOWN:
		this->screenPosition.y += this->velocity;
		break;
	case Directions::dir::LEFT:
		this->screenPosition.x -= this->velocity;
		break;
	case Directions::dir::RIGHT:
		this->screenPosition.x += this->velocity;
		break;
	}

	if (static_cast<int>(this->screenPosition.x + 8) % 16 == 0 && static_cast<int>(this->screenPosition.y + 8) % 16 == 0)
	{
		this->tilePosition = sf::Vector2i(
			static_cast<int>(round((this->screenPosition.x - 8) / 16)),
			static_cast<int>(round((this->screenPosition.y - 8) / 16))
		);
	}

	this->sprite.setPosition(this->screenPosition);
}

bool Pacman::canMove()
{
	if (!this->getDirectionsQueue().empty())
	{
		switch (this->getDirectionsQueue().front())
		{
		case Directions::dir::UP:
			return !this->map->isSolid(this->getTilePosition().x, this->getTilePosition().y - 1);
			break;
		case Directions::dir::DOWN:
			return !this->map->isSolid(this->getTilePosition().x, this->getTilePosition().y + 1);
			break;
		case Directions::dir::LEFT:
			return !this->map->isSolid(this->getTilePosition().x - 1, this->getTilePosition().y);
			break;
		case Directions::dir::RIGHT:
			return !this->map->isSolid(this->getTilePosition().x + 1, this->getTilePosition().y);
			break;
		}
	}
	return true;
}

//Update
void Pacman::update()
{
	if (this->canMove())
		this->move();
	else
		this->stop();

	if (this->map->isJunction(this->getTilePosition().x, this->getTilePosition().y))
		this->stop();

	this->teleportTunnels();

	this->animate();
	this->globalBounds = this->sprite.getGlobalBounds();
}

void Pacman::removeHealthPoints()
{
	if (this->healthPoints > 0)
	{
		this->healthPoints--;
	}
}

void Pacman::addScore(const int& score)
{
	this->score += score;
}

void Pacman::setHealthPoints(const int& hp)
{
	this->healthPoints = hp;
}

const int Pacman::getHealthPoints() const
{
	return this->healthPoints;
}

const int Pacman::getScore() const
{
	return this->score;
}

//Accessors
const std::queue<Directions::dir> Pacman::getDirectionsQueue() const
{
	return this->directionsQueue;
}

