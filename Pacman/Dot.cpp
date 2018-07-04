#include "Dot.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"

Dot::Dot(sf::Image& image) : Entity{ image }
{
}

Dot::Dot(sf::Image & image, std::shared_ptr<Tile> SpawnTile) : Entity{ image }
{
	if (!texture->loadFromImage(image))
	{
		std::cerr << "Failed to load texture for entity!\n";
	}
	texture->setSmooth(true);
	sprite->setTexture(*texture);

	auto p = texture->getSize();
	auto pp = sf::Vector2f(p);
	sprite->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));
	if (SpawnTile)
	{
		CurrentTile = SpawnTile;
	}
	spawnpoint = pos;
	sprite->setPosition(pos);
	colBox->setSize(pp);
	colBox->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));

	colBox->setFillColor(sf::Color::Transparent);
	colBox->setPosition(pos);

	std::cout << "Dot pos: " << pos.x << ", " << pos.y << std::endl;
}

Dot::~Dot()
{
}

void Dot::init(std::shared_ptr<Tile>& CurrentTileIn)
{
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
