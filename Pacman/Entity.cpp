#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Entity.h"
#include "Globals.h"

Entity::Entity() : pos{0,0}, vel{0,0}
{
	texture = std::unique_ptr<sf::Texture>(new sf::Texture);
	sprite = std::unique_ptr<sf::Sprite>(new sf::Sprite);
	colBox = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape);
}

Entity::~Entity()
{
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

