#pragma once
#include "Entity.h"
#include "Pacman.h"

class Ghost
	: public Entity
{
protected:
	enum MODE {SCATTER, CHASE, FRIGHTENED, EATEN};
	int mode;
	sf::Vector2i destinationTile;
	sf::Vector2i scatterDestinationTile;

	//Animations frightened
	std::vector<sf::IntRect> frightenedFrames;

	//Entity pointers
	Pacman* pacman;

public:
	Ghost(Pacman* pacman, Map* map, sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal,
		float& dt, const sf::Vector2i& tile_position, const float& velocity, sf::Vector2i scatterDestinationTile);
	virtual ~Ghost();

	void move();
	void decision();
	void update();
	void animate();
	void initFrightenedAnimation();
	virtual void chase() = 0;
	virtual void scatter();
	virtual void frightened();
	virtual void manageModes();

	float calcualteDistance(const sf::Vector2i& tile1, const sf::Vector2i& tile2);

	//Setters
	void setDestination(sf::Vector2i destination);
	void setMode(const int &mode);

	//Getters
	const int getMode() const;
	const sf::Vector2i getDestinationTile() const;
	const sf::Vector2i getScatterTile() const;
};

