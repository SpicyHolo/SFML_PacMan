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
	Map(sf::Texture& texture, const unsigned &map_tile_size);

	void initSprites();
	void initMap();
	void initJunctions();

	unsigned getTile(const unsigned &x, const unsigned& y);

	bool checkEntityBlock(const unsigned& x, const unsigned& y);
	bool isJunction(const unsigned& x, const unsigned& y);

	void update();
	void render(sf::RenderTarget& target);
};

