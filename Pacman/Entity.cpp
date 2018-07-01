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

void Entity::init()
{
	if (!texture->loadFromFile("images/pacman3.png"))
	{
		std::cerr << "Failed to load texture for entity!\n";
	}
	texture->setSmooth(true);
	sprite->setTexture(*texture);

	auto p = texture->getSize();
	auto pp = sf::Vector2f(p);
	sprite->setOrigin(sf::Vector2f(pp.x/2, pp.y/2)); 
	sprite->setPosition(globals::SCREEN_WIDHT/2, globals::SCREEN_HEIGHT/2);
	pos = sprite->getPosition();

	colBox->setSize(pp);
	colBox->setOrigin(sf::Vector2f(pp.x/2, pp.y/2));
	colBox->setPosition(globals::SCREEN_WIDHT / 2, globals::SCREEN_HEIGHT / 2);

	colBox->setFillColor(sf::Color::Transparent);
	colBox->setOutlineThickness(2.5f);
	colBox->setOutlineColor(sf::Color::Red);
}

void Entity::handleEvent(sf::Event& event)
{
	if (isControllable)
	{
		if (event.type == sf::Event::KeyPressed)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W: vel.y -= VELOCITY; vel.x = 0; sprite->setRotation(-90); break;
			case sf::Keyboard::S: vel.y += VELOCITY; vel.x = 0; sprite->setRotation(90); break;
			case sf::Keyboard::A: vel.x -= VELOCITY; vel.y = 0; sprite->setRotation(180); break;
			case sf::Keyboard::D: vel.x += VELOCITY; vel.y = 0; sprite->setRotation(0); break;
			default: break;
			}
		}
		else if (event.type == sf::Event::KeyReleased)
		{
			switch (event.key.code)
			{
			case sf::Keyboard::W: vel.y = 0; break;
			case sf::Keyboard::S: vel.y = 0; break;
			case sf::Keyboard::A: vel.x = 0; break;
			case sf::Keyboard::D: vel.x = 0; break;
			default: break;
			}
		}
	}
}

void Entity::tick(float deltaTime)
{
	move(deltaTime);
}

void Entity::render(sf::RenderWindow& renderWindow)
{
	renderWindow.draw(*sprite);
	renderWindow.draw(*colBox);
}

void Entity::move(float deltaTime)
{
	pos.x += (vel.x * deltaTime);
	pos.y += (vel.y * deltaTime);
	sprite->setPosition(pos.x, pos.y);
	colBox->setPosition(pos.x, pos.y);

	// Too far left
	if (pos.x < 0 + (colBox->getLocalBounds().width / 2))
	{
		pos.x = 0 + (colBox->getLocalBounds().width / 2);
		colBox->setPosition(pos.x, pos.y);
	}

	// Too far right
	if (pos.x > globals::SCREEN_WIDHT - (colBox->getLocalBounds().width / 2))
	{
		pos.x = globals::SCREEN_WIDHT - (colBox->getLocalBounds().width / 2);
		colBox->setPosition(pos.x, pos.y);
	}

	// Too far up
	if (pos.y < 0 + (colBox->getLocalBounds().height / 2))
	{
		pos.y = 0 + (colBox->getLocalBounds().height / 2);
		colBox->setPosition(pos.x, pos.y);
	}

	// Too far own
	if (pos.y > globals::SCREEN_HEIGHT - (colBox->getLocalBounds().height / 2))
	{
		pos.y = globals::SCREEN_HEIGHT - (colBox->getLocalBounds().height / 2);
		colBox->setPosition(pos.x, pos.y);
	}
}

