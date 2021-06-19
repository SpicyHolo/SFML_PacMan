#include "stdafx.h"
#include "Pacman.h"

Pacman::Pacman(sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal, float& dt, const sf::Vector2i& tile_position, const float& velocity)
	: Entity(texture, textureSize, startingTexturePos, framesTotal, dt, tile_position, velocity)
{
	this->sprite.setTextureRect(this->idleFrame);
	this->queueDirection(Directions::dir::LEFT);
}

void Pacman::queueDirection(const Directions::dir& direction)
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
		this->screenPosition.y += (-1)*this->velocity;
		break;
	case Directions::dir::DOWN:
		this->screenPosition.y += this->velocity;
		break;
	case Directions::dir::LEFT:
		this->screenPosition.x += (-1)*this->velocity;
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

void Pacman::update()
{
	this->animate();
}

const std::queue<Directions::dir> Pacman::getDirectionsQueue() const
{
	return this->directionsQueue;
}

Pacman::~Pacman()
{
}