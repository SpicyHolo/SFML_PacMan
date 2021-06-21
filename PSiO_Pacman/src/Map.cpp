#include "stdafx.h"
#include "Map.h"

//Constructor and Destructor
Map::Map(sf::Texture& texture, const unsigned& map_tile_size)
	: texture(texture), mapTileSize(map_tile_size)
{
	this->initMap();
	this->initJunctions();
	this->initSprites();
}

Map::~Map()
{
	for (auto &sprite : pieces)
	{
		delete sprite;
	}

	for (auto& vec : sprites)
	{
		for (auto& sprite : vec)
		{
			delete sprite;
		}
	}
}

//Initalizer funcctions
void Map::initSprites() //Creates sprites for every type of labirynth piece
{
	this->pieces.resize(32);

	unsigned textureIndex = 0;
	for (size_t i = 0; i < 8; i++)
	{
		for (size_t j = 0; j < 4; j++)
		{
			this->pieces[textureIndex] = new sf::Sprite(this->texture, sf::IntRect(8 * i, 8 * j, 8, 8));
			this->pieces[textureIndex]->setScale(2.f, 2.f);
			textureIndex++;
		}
	}
}

//Read map configuration from a config file
void Map::initMap()
{
	std::ifstream file("config/map.ini");
	std::string str;

	while (std::getline(file, str))
	{
		std::vector<int> vec;

		std::string seperator = " ";
		size_t pos = 0;
		std::string token;
		while ((pos = str.find(seperator)) != std::string::npos)
		{
			token = str.substr(0, pos);
			vec.push_back(std::stoi(token));
			str.erase(0, pos + seperator.length());
		}
		vec.push_back(abs(std::stoi(str)));
		this->tiles.push_back(vec);
	}


}

//Read Junctions in a map from a config file
void Map::initJunctions()
{
	std::ifstream file("config/junctions.ini");
	std::string str;

	while (std::getline(file, str))
	{
		std::pair<int, int> pair;

		std::string seperator = " ";
		size_t pos = 0;
		std::string token;

		pos = str.find(seperator);
		token = str.substr(0, pos);
		pair.first = stoi(token);
		str.erase(0, pos + seperator.length());
		pair.second = stoi(str);

		this->intersections.push_back(pair);
	}
}

//Accessors
int Map::getTile(const int& x, const int& y)
{	
	return this->tiles[x][y];
}

bool Map::isSolid(const int& x, const int& y)
{
	if (x > tiles.size() || x < 0 || y > tiles[0].size() || y < 0)
	{
		return true;
	}
	else
		return tiles[x][y] != 30 && tiles[x][y] != 26 && tiles[x][y] != 27;
}

bool Map::isJunction(const int& x, const int& y)
{
	for (auto& p : intersections)
	{
		if (x == p.first && y == p.second)
			return true;
	}
	return false;
}

bool Map::removeTictac(const sf::Vector2i& tile)
{
	if (tiles[tile.x][tile.y] == 26)
	{
		tiles[tile.x][tile.y] = 30;
		return true;
	}
	return false;
}

bool Map::removePowerUp(const sf::Vector2i& tile)
{
	if (tiles[tile.x][tile.y] == 27)
	{
		tiles[tile.x][tile.y] = 30;
		return true;
	}
	return false;
}

//Update
void Map::update()
{
}

//Render
void Map::render(sf::RenderTarget& target)
{
	for (size_t i = 0; i < this->tiles.size(); i++)
	{
		for (size_t j = 0; j < this->tiles[0].size(); j++)
		{
			this->pieces[this->getTile(i, j)]->setPosition(i * 16.0f, j * 16.0f);
			target.draw(*pieces[this->getTile(i, j)]);
		}
	}
}
