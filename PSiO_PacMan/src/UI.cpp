#include "stdafx.h"
#include "UI.h"

UI::UI(sf::Texture &texture)
	: texture(texture)
{

	this->font.loadFromFile("resources/ArcadepixPlus.ttf");

	//SCORE
	this->scoreText.setFont(this->font);
	this->scoreText.setFillColor(sf::Color::White);
	this->scoreText.setCharacterSize(30);
	this->scoreText.setPosition(50.f, 10.f);
	this->scoreText.setString("0");
	
	//HP
	//(448, 596)
	this->countHP = 0;
	this->displayHP.resize(3);
	for (size_t i = 0; i<this->displayHP.size(); i++)
	{
		this->displayHP[i] = new sf::Sprite(this->texture, sf::IntRect(15*7, 0, 15, 15));
		this->displayHP[i]->setScale(2.f, 2.f);
		this->displayHP[i]->setPosition(50.f + (15.f * this->displayHP[i]->getScale().x) * i, 596.f - 40.f);
	}

}

UI::~UI()
{
	for (auto& s : displayHP)
	{
		delete s;
	}
}

void UI::update(const int& score, const int& healthPoints)
{
	//Score
	this->scoreText.setString(std::to_string(score));

	//HP
	this->countHP = healthPoints;
}

void UI::render(sf::RenderTarget &target)
{
	target.draw(this->scoreText);

	for (size_t i = 0; i < this->countHP; i++)
	{
		target.draw(*this->displayHP[i]);
	}
}
