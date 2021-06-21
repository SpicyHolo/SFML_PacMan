#pragma once
#include "Entity.h"

class Pacman :
    public Entity
{
private:
    //Variables
    int healthPoints;
    int score;
    std::queue<Directions::dir> directionsQueue;

public:
    //Constructor and destructor
    Pacman(Map* map, sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal, float& dt, const sf::Vector2i& tile_position, const float& velocity);
    virtual ~Pacman();

    //Movement and direction
    void queueDirection(const Directions::dir &direction);
    void stop();
    void move();
    bool canMove();

    //Update
    void update();

    //Setters
    void removeHealthPoints();
    void addScore(const int& score);
    void setHealthPoints(const int& hp);

    //Accessorss
    const int getHealthPoints() const;
    const int getScore() const;
    const std::queue<Directions::dir> getDirectionsQueue() const;
};

