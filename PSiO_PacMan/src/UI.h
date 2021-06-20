#pragma once
class UI
{
private:
	//Texture
	sf::Texture texture;

	//Text
	sf::Font font;
	sf::Text scoreText;
	std::vector<sf::Sprite*> displayHP;
	int countHP;

public:
	UI(sf::Texture& texture);
	virtual ~UI();

	void update(const int &score, const int &healthPoints);

	void render(sf::RenderTarget& target);
};

