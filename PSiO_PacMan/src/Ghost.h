#pragma once
#include "Entity.h"

class Ghost
	: public Entity
{
private:
	enum MODE {SCATTER = 0, PURSUIT, FRIGHTENED, EATEN};

	int mode;
	sf::Vector2i destinationTile;
	sf::Vector2i scatterDestinationTile;

	bool decisionMade;

public:
	Ghost(sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal, 
		float& dt, const sf::Vector2i& tile_position, const float& velocity, sf::Vector2i scatterDestinationTile);
	virtual ~Ghost();

	void move();
	float calcualteDistance(const sf::Vector2i& tile1, const sf::Vector2i& tile2);

	void setDestination(sf::Vector2i destination);
	void setMode(const int &mode);
	void setDecisionMade(const bool &value);

	const int getMode() const;
	const sf::Vector2i getDestinationTile() const;
	const sf::Vector2i getScatterTile() const;
	const bool getDecisionMade() const;
};

