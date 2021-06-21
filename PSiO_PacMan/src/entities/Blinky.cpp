#include "../stdafx.h"
#include "Blinky.h"

Blinky::Blinky(Pacman* pacman, Map* map, sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal, float& dt, const sf::Vector2i& tile_position, const float& velocity, sf::Vector2i scatterDestinationTile)
	: Ghost(pacman, map, texture, textureSize, startingTexturePos, framesTotal, dt, tile_position, velocity, scatterDestinationTile)
{}

Blinky::~Blinky()
{}

void Blinky::chase()
{
	if (!this->pacman->getDirectionsQueue().empty())
		this->setDestination(this->pacman->getTilePosition());
}


