#include "../stdafx.h"
#include "Pinky.h"

Pinky::Pinky(Pacman* pacman, Map* map, sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal, float& dt, const sf::Vector2i& tile_position, const float& velocity, sf::Vector2i scatterDestinationTile)
	: Ghost(pacman, map, texture, textureSize, startingTexturePos, framesTotal, dt, tile_position, velocity, scatterDestinationTile)
{}

Pinky::~Pinky()
{}

void Pinky::chase()
{
	if (!pacman->getDirectionsQueue().empty())
		{
			switch (this->pacman->getFacingDirection())
			{
			case Directions::dir::UP:
				this->setDestination(this->pacman->getTilePosition() + sf::Vector2i(-4, -4));
				break;
			case Directions::dir::DOWN:
				this->setDestination(this->pacman->getTilePosition() + sf::Vector2i(0, 4));
				break;
			case Directions::dir::LEFT:
				this->setDestination(this->pacman->getTilePosition() + sf::Vector2i(-4, 0));
				break;
			case Directions::dir::RIGHT:
				this->setDestination(this->pacman->getTilePosition() + sf::Vector2i(4, 0));
				break;
			}
		}
}
