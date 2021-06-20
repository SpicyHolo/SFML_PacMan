#include "stdafx.h"
#include "Entity.h"

//Constructor and destructor
Entity::Entity(sf::Texture& texture, const sf::Vector2u& texture_size, const sf::Vector2u& starting_texture_pos, const unsigned& frames_total , float &dt, const sf::Vector2i &tile_position, const float& velocity)
	: texture(texture), textureSize(texture_size), startingTexturePos(starting_texture_pos), framesTotal(frames_total), dt(dt), velocity(velocity)
{

	//init animation variables
	this->animationTimer = 10.f;
	this->timer = 0.f;
	this->lastFrame = frames_total - 1;
	this->currentFrame = 0;

	//init Sprites
	this->initAnimation();
	this->sprite.setTexture(this->texture);
	this->sprite.setScale(2.f, 2.f);
	this->sprite.setOrigin(7.5f, 7.5f);

	//Position
	this->tilePosition = tile_position;
	this->screenPosition = sf::Vector2f(tilePosition.x * 16.f + 16.f, tilePosition.y * 16.f + 8.f);
	this->sprite.setPosition(this->screenPosition);

	//Velocity
	this->velocity = velocity;

	//Global Bounds
	this->globalBounds = this->sprite.getGlobalBounds();

}

Entity::~Entity()
{
}

//Initializer functions
//Loads animation sf::IntRect into a map (for each facing direction)
void Entity::initAnimation()
{
	this->idleFrame = sf::IntRect(this->startingTexturePos.x * this->textureSize.x, this->startingTexturePos.y * this->textureSize.y, this->textureSize.x, this->textureSize.y);

	this->frames[Directions::dir::UP] = std::vector<sf::IntRect>();
	this->frames[Directions::dir::DOWN] = std::vector<sf::IntRect>();
	this->frames[Directions::dir::LEFT] = std::vector<sf::IntRect>();
	this->frames[Directions::dir::RIGHT] = std::vector<sf::IntRect>();

	std::vector<sf::IntRect>* order_array[4] = { &this->frames.at(Directions::dir::UP), &this->frames.at(Directions::dir::DOWN), &this->frames.at(Directions::dir::LEFT), &this->frames.at(Directions::dir::RIGHT) };

	for (unsigned i = 0; i < 4; i++)
	{
		for (unsigned j = 0; j < this->framesTotal; j++)
		{
			int left = (this->startingTexturePos.x + (i * this->framesTotal) + j) * this->textureSize.x;
			int top = (this->startingTexturePos.y * this->textureSize.y); //Each animation is done in one row
			int width = this->textureSize.x;
			int height = this->textureSize.y;

			order_array[i]->push_back(sf::IntRect(left, top, width, height));
		}
	}
}

//Movement and location
void Entity::setFacingDirection(const int &dir)
{
	this->facingDirection = dir;
}

void Entity::move(const sf::Vector2f& offset)
{
	this->screenPosition.x += offset.x;
	this->screenPosition.y += offset.y;

	if (static_cast<int>(this->screenPosition.x + 8) % 16 == 0 && static_cast<int>(this->screenPosition.y + 8) % 16 == 0)
	{
		this->tilePosition = sf::Vector2i(
			static_cast<int>(round((this->screenPosition.x - 8) / 16)),
			static_cast<int>(round((this->screenPosition.y - 8) / 16))
		);
	}

	this->sprite.setPosition(this->screenPosition);
}

//Teleports sprite to a tile
void Entity::setTile(const sf::Vector2i& tile)
{
	this->tilePosition = tile;
	this->screenPosition = sf::Vector2f(
		this->tilePosition.x * 16.f + 8.f,
		this->tilePosition.y * 16.f + 8.f
	);
	this->sprite.setPosition(this->screenPosition);
}

//Update
void Entity::update()
{
	this->animate();
	this->globalBounds = this->sprite.getGlobalBounds();
}

//Animates the sprite based on facing direction
void Entity::animate()
{
	if (!this->facingDirection == Directions::dir::IDLE)
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
			sf::IntRect textureRect = this->frames.at(this->facingDirection)[currentFrame];
			this->sprite.setTextureRect(textureRect);
		}
	}
	else
	{
		this->sprite.setTextureRect(this->idleFrame);
	}
}

//Render
void Entity::render(sf::RenderTarget& target)
{
	target.draw(this->sprite);
}

const int Entity::getFacingDirection() const
{
	return this->facingDirection;
}

//Accessors
const sf::Vector2i Entity::getTilePosition() const
{
	return this->tilePosition;
}

const sf::Vector2f Entity::getScreenPosition() const
{
	return this->screenPosition;
}

sf::FloatRect Entity::getGlobalBounds()
{
	return this->globalBounds;
}
