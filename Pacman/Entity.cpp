#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Entity.h"
#include "Globals.h"
#include "Tile.h"

Entity::Entity(sf::Image& image, std::weak_ptr<Tile> SpawnTile, Game& g) : pos{0,0}, vel{0,0}, game{ &g }
{
	texture = std::unique_ptr<sf::Texture>(new sf::Texture);
	sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite);
	colBox = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape);

	if (!texture->loadFromImage(image))
	{
		std::cerr << "Failed to load texture for entity!\n";
	}
	texture->setSmooth(true);
	sprite->setTexture(*texture);
	auto st = SpawnTile.lock();
	if (st)
	{
		CurrentTile = st;
		pos = CurrentTile->getPos();
	}
	auto p = texture->getSize();
	auto pp = sf::Vector2f(p);
	sprite->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));

	sprite->setPosition(pos);
	colBox->setSize(pp);
	colBox->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));

	colBox->setFillColor(sf::Color::Transparent);
	colBox->setPosition(pos);

}

void Entity::render(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(*sprite);
	renderWindow.draw(*colBox);
}

void Entity::setPos(sf::Vector2f posIn)
{
	pos = posIn;
	sprite->setPosition(pos);
	colBox->setPosition(pos);
}

