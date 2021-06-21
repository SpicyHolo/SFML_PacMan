#include "../stdafx.h"
#include "Clyde.h"

Clyde::Clyde(Pacman* pacman, Map* map, sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal, float& dt, const sf::Vector2i& tile_position, const float& velocity, sf::Vector2i scatterDestinationTile)
	: Ghost(pacman, map, texture, textureSize, startingTexturePos, framesTotal, dt, tile_position, velocity, scatterDestinationTile)
{}

Clyde::~Clyde()
{}

void Clyde::chase()
{
	if (!this->pacman->getDirectionsQueue().empty())
	{
		if (this->calcualteDistance(this->getTilePosition(), this->pacman->getTilePosition()) >= 8)
			this->setDestination(this->pacman->getTilePosition());
		else
			this->setDestination(this->getScatterTile());
	}
}
