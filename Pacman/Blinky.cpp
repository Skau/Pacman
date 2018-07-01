#include "Blinky.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"

Blinky::Blinky()
{
}


Blinky::~Blinky()
{
}

void Blinky::init()
{
	if (!texture->loadFromFile("images/blinky.png"))
	{
		std::cerr << "Failed to load texture for entity!\n";
	}
	texture->setSmooth(true);
	sprite->setTexture(*texture);

	auto p = texture->getSize();
	auto pp = sf::Vector2f(p);
	sprite->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));
	pos = sprite->getPosition();

	colBox->setSize(pp);
	colBox->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));

	colBox->setFillColor(sf::Color::Transparent);
	colBox->setOutlineThickness(2.5f);
	colBox->setOutlineColor(sf::Color::Red);
}

void Blinky::tick(float deltaTime)
{
	move(deltaTime);
}

void Blinky::move(float deltaTime)
{
	if (pos.x < 300)
	{
		vel.x = 100;
	}
	else if (pos.x > 500)
	{
		vel.x = -100;
	}

	pos.x += (vel.x * deltaTime);
	sprite->setPosition(pos);
	colBox->setPosition(pos);
}
