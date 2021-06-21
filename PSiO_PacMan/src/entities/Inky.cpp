#include "../stdafx.h"
#include "Inky.h"

Inky::Inky(Blinky* blinky, Pacman* pacman, Map* map, sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal, float& dt, const sf::Vector2i& tile_position, const float& velocity, sf::Vector2i scatterDestinationTile)
	: Ghost(pacman, map, texture, textureSize, startingTexturePos, framesTotal, dt, tile_position, velocity, scatterDestinationTile), blinky(blinky)
{}

Inky::~Inky()
{}

void Inky::chase()
{
	if (!this->pacman->getDirectionsQueue().empty())
	{
		sf::Vector2i pacman_position(0, 0);

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
		this->setDestination(pacman_position - (this->blinky->getTilePosition() -pacman_position));
	}
}

