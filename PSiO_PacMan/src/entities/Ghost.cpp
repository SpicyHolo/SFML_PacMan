#include "../stdafx.h"
#include "Ghost.h"

Ghost::Ghost(Pacman* pacman, Map* map, sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u& startingTexturePos, const unsigned& framesTotal,
	float& dt, const sf::Vector2i& tile_position, const float& velocity, sf::Vector2i scatterDestinationTile)
	: Entity(map, texture, textureSize, startingTexturePos, framesTotal, dt, tile_position, velocity), scatterDestinationTile(scatterDestinationTile), pacman(pacman)
{
	this->scatterDestinationTile = scatterDestinationTile;
	this->facingDirection = Directions::dir::LEFT;
	this->destinationTile = this->scatterDestinationTile;
	this->mode = MODE::SCATTER;

	this->initFrightenedAnimation();
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

void Ghost::decision()
{
	if (static_cast<int>(this->getScreenPosition().x + 8) % 16 == 0 && static_cast<int>(this->getScreenPosition().y + 8) % 16 == 0)
	{
		std::map<int, bool> options =
		{
			{Directions::dir::UP, true},{Directions::dir::DOWN, true},{Directions::dir::LEFT, true},{Directions::dir::RIGHT, true}
		};

		//Ghosts don't turn back in scatter/pursue mode
		switch (this->getFacingDirection())
		{
		case Directions::dir::UP:
			options[Directions::dir::DOWN] = false;
			break;
		case Directions::dir::DOWN:
			options[Directions::dir::UP] = false;
			break;
		case Directions::dir::LEFT:
			options[Directions::dir::RIGHT] = false;
			break;
		case Directions::dir::RIGHT:
			options[Directions::dir::LEFT] = false;
			break;
		}

		//Check for block on all sides
		if (this->map->isSolid(this->getTilePosition().x, this->getTilePosition().y - 1))
			options[Directions::dir::UP] = false;
		if (this->map->isSolid(this->getTilePosition().x, this->getTilePosition().y + 1))
			options[Directions::dir::DOWN] = false;
		if (this->map->isSolid(this->getTilePosition().x - 1, this->getTilePosition().y))
			options[Directions::dir::LEFT] = false;
		if (this->map->isSolid(this->getTilePosition().x + 1, this->getTilePosition().y))
			options[Directions::dir::RIGHT] = false;

		//Calculate distance
		std::map <int, float> distance =
		{
		{Directions::dir::UP, 999},{Directions::dir::DOWN, 999},{Directions::dir::LEFT, 999},{Directions::dir::RIGHT, 999}
		};

		if (options[Directions::dir::UP])
			distance[Directions::dir::UP] = this->calcualteDistance(this->getTilePosition() + sf::Vector2i(0, -1), this->getDestinationTile());
		if (options[Directions::dir::DOWN])
			distance[Directions::dir::DOWN] = this->calcualteDistance(this->getTilePosition() + sf::Vector2i(0, 1), this->getDestinationTile());
		if (options[Directions::dir::LEFT])
			distance[Directions::dir::LEFT] = this->calcualteDistance(this->getTilePosition() + sf::Vector2i(-1, 0), this->getDestinationTile());
		if (options[Directions::dir::RIGHT])
			distance[Directions::dir::RIGHT] = this->calcualteDistance(this->getTilePosition() + sf::Vector2i(1, 0), this->getDestinationTile());

		float smallest_distance = 998;
		int smallest_distance_key = 0;
		for (auto& d : distance)
		{
			if (d.second < smallest_distance)
			{
				smallest_distance = d.second;
				smallest_distance_key = d.first;
			}
		}

		int top_priority_key = smallest_distance_key;
		for (auto& d : distance)
		{
			if (d.first == smallest_distance_key)
			{
				if (d.first < top_priority_key)
				{
					top_priority_key = d.first;
				}
			}
		}
		this->setFacingDirection(top_priority_key);
	}
}

void Ghost::update()
{
	this->manageModes();

	//If on a new tile, make decision about direction
	if (static_cast<int>(this->getScreenPosition().x + 8) % 16 == 0 && static_cast<int>(this->getScreenPosition().x + 8) % 16 == 0 && this->changedTile)
	{
		this->changedTile = false;

		if (this->getMode() == MODE::FRIGHTENED)
		{
			this->frightened();
		}
		else
			this->decision();
	}
	this->move();
	this->teleportTunnels();

	this->animate();
	this->globalBounds = this->sprite.getGlobalBounds();
}

void Ghost::animate()
{
	if (!this->facingDirection == Directions::dir::IDLE || this->getMode() == MODE::FRIGHTENED)
	{
		this->timer += 100.f * this->dt;
		if (this->timer >= this->animationTimer)
		{
			//Reset timer
			this->timer = 0.f;

			//Animate
			if (this->currentFrame != this->lastFrame)
			{
				this->currentFrame++;
			}
			else
			{
				this->currentFrame = 0;
			}

			sf::IntRect textureRect;
			if (this->getMode() == MODE::FRIGHTENED)
				textureRect = this->frightenedFrames[currentFrame];
			else
				textureRect = this->frames.at(this->facingDirection)[currentFrame];

			this->sprite.setTextureRect(textureRect);
		}
	}
	else
	{
		this->sprite.setTextureRect(this->idleFrame);
	}
}
void Ghost::initFrightenedAnimation()
{
	sf::Vector2i starting_texture_pos(6, 1);

	for (int i = 0; i < this->framesTotal; i++)
	{
		int left = (starting_texture_pos.x + (this->framesTotal) + i) * this->textureSize.x;
		int top = (starting_texture_pos.y * this->textureSize.y); //Each animation is done in one row
		int width = this->textureSize.x;
		int height = this->textureSize.y;

		this->frightenedFrames.push_back(sf::IntRect(left, top, width, height));
	}
}

void Ghost::scatter()
{
	this->setDestination(this->scatterDestinationTile);
}

void Ghost::frightened()
{
	if (static_cast<int>(this->getScreenPosition().x + 8) % 16 == 0 && static_cast<int>(this->getScreenPosition().y + 8) % 16 == 0)
	{
		std::map<int, bool> options =
		{
			{Directions::dir::UP, true},{Directions::dir::DOWN, true},{Directions::dir::LEFT, true},{Directions::dir::RIGHT, true}
		};

		//Ghosts don't turn back in scatter/pursue mode
		switch (this->getFacingDirection())
		{
		case Directions::dir::UP:
			options[Directions::dir::DOWN] = false;
			break;
		case Directions::dir::DOWN:
			options[Directions::dir::UP] = false;
			break;
		case Directions::dir::LEFT:
			options[Directions::dir::RIGHT] = false;
			break;
		case Directions::dir::RIGHT:
			options[Directions::dir::LEFT] = false;
			break;
		}

		//Check for block on all sides
		if (this->map->isSolid(this->getTilePosition().x, this->getTilePosition().y - 1))
			options[Directions::dir::UP] = false;
		if (this->map->isSolid(this->getTilePosition().x, this->getTilePosition().y + 1))
			options[Directions::dir::DOWN] = false;
		if (this->map->isSolid(this->getTilePosition().x - 1, this->getTilePosition().y))
			options[Directions::dir::LEFT] = false;
		if (this->map->isSolid(this->getTilePosition().x + 1, this->getTilePosition().y))
			options[Directions::dir::RIGHT] = false;

		//Choose direction randomly
		//Random number between -2 and 2 (that corresponds to a direction) excluding not viable directions and 0 (which is not a direction)
		std::vector<int> random_options;
		for (auto& dir : options)
		{
			if (dir.second)
				random_options.push_back(dir.first);
		}
		
		if(random_options.size() > 0)
		{
			int random = rand() % random_options.size();

			this->setFacingDirection(random_options[random]);
		}
	}
}

void Ghost::manageModes()
{
	if (this->getMode() == MODE::SCATTER)
	{
		this->scatter();
	}
	else if (this->getMode() == MODE::CHASE)
	{
		this->chase();
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
	if (mode == MODE::FRIGHTENED) //When entering scatter mode turn 180 deg
	{
		switch (-this->getFacingDirection())
		{
		case Directions::dir::UP:
			if (!this->map->isSolid(this->getTilePosition().x, this->getTilePosition().y + 1))
				this->setFacingDirection(Directions::dir::DOWN);
			break;
		case Directions::dir::DOWN:
			if (!this->map->isSolid(this->getTilePosition().x, this->getTilePosition().y - 1))
				this->setFacingDirection(Directions::dir::UP);
			break;
		case Directions::dir::LEFT:
			if (!this->map->isSolid(this->getTilePosition().x + 1, this->getTilePosition().y))
				this->setFacingDirection(Directions::dir::RIGHT);
			break;
		case Directions::dir::RIGHT:
			if (!this->map->isSolid(this->getTilePosition().x - 1, this->getTilePosition().y))
				this->setFacingDirection(Directions::dir::LEFT);
			break;
		}
	}

	this->mode = mode;
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
