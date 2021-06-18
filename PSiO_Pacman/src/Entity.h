#pragma once
class Entity
{
protected:
	//dt
	float& dt;

	//Graphics
	sf::Sprite sprite;
	sf::Texture& texture;

	//Animation
	std::map<int, std::vector<sf::IntRect>> animations;

	sf::Vector2u textureSize;
	sf::Vector2u startingTexturePos;
	unsigned framesTotal;
	
	float animationTimer;
	float timer;

	int currentFrame;
	int lastFrame;

	//Movement
	int direction;
	float velocity;

	//Tile position
	sf::Vector2i tilePosition;
	sf::Vector2f screenPosition;


public:
	enum direction { IDLE = 0, UP = 1, DOWN = -1, LEFT = 2, RIGHT = -2 };

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
	void setDirection(const int& dir);
	void move(const sf::Vector2f& offset);
	void changeTile(const sf::Vector2i& tile);

	//Direcitons

	//Accessors
	sf::Vector2i getTilePosition();
	sf::Vector2f getScreenPosition();
};

