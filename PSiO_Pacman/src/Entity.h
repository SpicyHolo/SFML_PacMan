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
	std::map<int, std::vector<sf::IntRect>> frames;
	sf::IntRect idleFrame;

	//Animation timer
	int framesTotal;
	float animationTimer;
	float timer;

	//Animation frame
	int currentFrame;
	int lastFrame;

	//Movement and direction
	int facingDirection;
	float velocity;

	//Tile position
	sf::Vector2i tilePosition;
	sf::Vector2f screenPosition;


public:
	
	//Constructor and destructor
	Entity(sf::Texture& texture, const sf::Vector2u& textureSize, const sf::Vector2u &startingTexturePos, const unsigned &framesTotal, float &dt, const sf::Vector2i& tile_position, const float &velocity);
	virtual ~Entity();

	//Initializer functions
	void initAnimation();


	//Movement and location
	void setFacingDirection(const int &dir);
	void move(const sf::Vector2f& offset);
	void setTile(const sf::Vector2i& tile);

	//Update
	virtual void update();
	virtual void animate();

	//Render
	virtual void render(sf::RenderTarget& target);

	//Accessors
	const int getFacingDirection() const;
	const sf::Vector2i getTilePosition() const;
	const sf::Vector2f getScreenPosition() const;
};

