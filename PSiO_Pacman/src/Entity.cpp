#include "stdafx.h"
#include "Entity.h"

Entity::Entity(sf::Texture& texture, const sf::Vector2u& texture_size, const sf::Vector2u& starting_texture_pos, const unsigned& frames_total , float &dt, const sf::Vector2i &tile_position, const float& velocity)
	: texture(texture), textureSize(texture_size), startingTexturePos(starting_texture_pos), framesTotal(frames_total), dt(dt), velocity(velocity),
	animationTimer(20.f), timer(0.f), lastFrame(framesTotal - 1), currentFrame(0), direction(UP)
{

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
}

Entity::~Entity()
{
}

void Entity::update()
{
	this->sprite.setPosition(this->screenPosition);
	this->animate();
}

void Entity::render(sf::RenderTarget &target)
{
	target.draw(this->sprite);
}

void Entity::initAnimation()
{
	for (size_t i = 0; i < 4; i++)
	{
		//enum UP = 1, DOWN = -1, LEFT = 2, RIGHT = -2
		//map i = [1;4] o this enum

		int dir_array[4] = { UP, DOWN, LEFT, RIGHT };
		int dir = dir_array[i];

		//Loads texture's intRects from texture grid
		for (size_t j = 0; j < this->framesTotal; j++)
		{
			unsigned left = (this->startingTexturePos.x + j + (i * framesTotal)) * this->textureSize.x;
			unsigned top = (this->startingTexturePos.y * this->textureSize.y);
			unsigned width = textureSize.x;
			unsigned height = textureSize.y;
			this->animations[dir].push_back(sf::IntRect(left, top, width, height));
		}
	}
}
void Entity::animate()
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
		sf::IntRect textureRect = this->animations.at(this->direction)[currentFrame];
		this->sprite.setTextureRect(textureRect);
	}
}

void Entity::setDirection(const int& dir)
{
	this->direction = dir;
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


}

void Entity::changeTile(const sf::Vector2i& tile)
{
	this->tilePosition = tile;
	this->screenPosition = sf::Vector2f(
		this->tilePosition.x * 16.f + 8.f,
		this->tilePosition.y * 16.f + 8.f
	);
}

sf::Vector2i Entity::getTilePosition()
{
	return this->tilePosition;
}

sf::Vector2f Entity::getScreenPosition()
{
	return this->screenPosition;
}
