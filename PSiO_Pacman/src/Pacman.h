#pragma once
#include "Entity.h"

class Pacman :
    public Entity
{
private:
    std::queue<Directions::dir> directionsQueue;

public:
    Pacman(sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal, float& dt, const sf::Vector2i& tile_position, const float& velocity);
    virtual ~Pacman();

    void queueDirection(const Directions::dir &direction);
    void stop();

    void move();

    void update();

    const std::queue<Directions::dir> getDirectionsQueue() const;
};

