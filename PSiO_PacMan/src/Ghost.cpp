#include "stdafx.h"
#include "Ghost.h"

Ghost::Ghost(sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal, 
	float& dt, const sf::Vector2i& tile_position, const float& velocity, sf::Vector2i scatterDestinationTile)
	: Entity(texture, textureSize, startingTexturePos, framesTotal, dt, tile_position, velocity), scatterDestinationTile(scatterDestinationTile), mode(Ghost::MODE::SCATTER)
{
	this->facingDirection = Directions::dir::LEFT;
	this->destinationTile = this->scatterDestinationTile;
	this->mode = MODE::SCATTER;
}

Ghost::~Ghost()
{
}

void Ghost::move()
{
	switch (this->facingDirection)
	{
	case Directions::dir::UP:
		Entity::move(sf::Vector2f(0.f, -this->velocity));
		break;
	case Directions::dir::DOWN:
		Entity::move(sf::Vector2f(0.f, this->velocity));
		break;
	case Directions::dir::LEFT:
		Entity::move(sf::Vector2f(-this->velocity, 0.f));
		break;
	case Directions::dir::RIGHT:
		Entity::move(sf::Vector2f(this->velocity, 0.f));
		break;
	}
}

float Ghost::calcualteDistance(const sf::Vector2i& tile1, const sf::Vector2i& tile2)
{
	float dx = static_cast<float>(abs(tile1.x - tile2.x));
	float dy = static_cast<float>(abs(tile1.y - tile2.y));
	return sqrt(dx * dx + dy * dy);
}

void Ghost::setDestination(sf::Vector2i destination)
{
	this->destinationTile = destination;
}

void Ghost::setMode(const int &mode)
{
	this->mode = mode;
}

void Ghost::setDecisionMade(const bool& value)
{
	this->decisionMade = value;
}

const int Ghost::getMode() const
{
	return this->mode;
}

const sf::Vector2i Ghost::getDestinationTile() const
{
	return this->destinationTile;
}

const sf::Vector2i Ghost::getScatterTile() const
{
	return this->scatterDestinationTile;
}

const bool Ghost::getDecisionMade() const
{
	return this->decisionMade;
}
