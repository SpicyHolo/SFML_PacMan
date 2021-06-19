#pragma once
class Entity
{
protected:
	//dt
	float& dt;

	//Graphics
	sf::Sprite sprite;
	sf::Texture& texture;
	sf::Vector2u textureSize;
	sf::Vector2u startingTexturePos;

	//Animations
	std::map<Directions::dir, std::vector<sf::IntRect>> frames;
	sf::IntRect idleFrame;

	int framesTotal;
	float animationTimer;
	float timer;

	int currentFrame;
	int lastFrame;

	//Movement
	Directions::dir facingDirection;
	float velocity;

	//Tile position
	sf::Vector2i tilePosition;
	sf::Vector2f screenPosition;


public:
	

	//Constructor
	Entity(sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u &startingTexturePos, const unsigned &framesTotal, float &dt, const sf::Vector2i& tile_position, const float &velocity);
	virtual ~Entity();

	//Update and Render
	virtual void update();
	virtual void render(sf::RenderTarget& target);

	//Animation
	void initAnimation();
	virtual void animate();

	//Movement and location
	void setFacingDirection(const Directions::dir &dir);
	virtual void move(const sf::Vector2f& offset);
	void setTile(const sf::Vector2i& tile);


	//Accessors
	sf::Vector2i getTilePosition();
	sf::Vector2f getScreenPosition();
};

