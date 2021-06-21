#pragma once
#include "Ghost.h"

class Pinky :
    public Ghost
{
public:
    Pinky(Pacman* pacman, Map* map, sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal,
        float& dt, const sf::Vector2i& tile_position, const float& velocity, sf::Vector2i scatterDestinationTile);
    virtual ~Pinky();

    void chase();
};
