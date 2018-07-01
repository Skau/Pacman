#include "Dot.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"

Dot::Dot()
{
}

Dot::~Dot()
{
}

void Dot::init()
{
	if (!texture->loadFromFile("images/dot.png"))
	{
		std::cerr << "Failed to load texture for entity!\n";
	}
	texture->setSmooth(true);
	sprite->setTexture(*texture);

	auto p = texture->getSize();
	auto pp = sf::Vector2f(p);
	sprite->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));

	colBox->setSize(pp);
	colBox->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));

	colBox->setFillColor(sf::Color::Transparent);
	colBox->setOutlineThickness(2.5f);
	colBox->setOutlineColor(sf::Color::Red);
}

void Dot::tick(float deltaTime)
{
}
