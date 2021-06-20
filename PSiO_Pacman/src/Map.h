#pragma once
class Map
{
private:

	//Tiles
	std::vector < std::vector<int>> tiles;

	//Intersections
	std::vector<std::pair<int, int>> intersections;

	//Graphics
	sf::Texture &texture;
	std::vector<std::vector<sf::Sprite*>> sprites;
	std::vector<sf::Sprite*> pieces;
	unsigned mapTileSize;


public:
	//Constructor and destructor
	Map(sf::Texture& texture, const unsigned &map_tile_size);
	virtual ~Map();

	//Initializer functions
	void initSprites();
	void initMap();
	void initJunctions();

	//Accessors
	int getTile(const int &x, const int& y);

	bool checkEntityBlock(const int& x, const int& y);
	bool checkGhostBlock(const int& x, const int& y);

	bool isJunction(const int& x, const int& y);

	bool removeTictac(const sf::Vector2i& tile);
	bool removePowerUp(const sf::Vector2i& tile);

	//Update
	void update();

	//Render
	void render(sf::RenderTarget& target);
};

