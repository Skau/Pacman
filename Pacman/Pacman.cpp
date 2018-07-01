#include "Pacman.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Globals.h"
#include "Dot.h"

Pacman::Pacman()
{
}

Pacman::~Pacman()
{
}

void Pacman::init()
{
	if (!texture->loadFromFile("images/pacman.png"))
	{
		std::cerr << "Failed to load texture for entity!\n";
	}
	texture->setSmooth(true);
	sprite->setTexture(*texture);

	auto p = texture->getSize();
	auto pp = sf::Vector2f(p);
	sprite->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));
	pos = sprite->getPosition();
	spawnpoint = pos;
	colBox->setSize(pp);
	colBox->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));

	colBox->setFillColor(sf::Color::Transparent);
	colBox->setOutlineThickness(2.5f);
	colBox->setOutlineColor(sf::Color::Red);


}

void Pacman::handleEvent(sf::Event & event)
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

void Pacman::tick(float deltaTime)
{
	//intersects();
	move(deltaTime);
}

//bool Pacman::intersects()
//{
//	int index = 0;
//	for (auto& entity : allEntities)
//	{
//		if (colBox->getGlobalBounds().intersects(entity->getColBox().getGlobalBounds()))
//		{
//			auto d = std::dynamic_pointer_cast<Dot>(entity);
//			if (d)
//			{
//				d->setIsDestroyed(true);
//				allEntities.erase(allEntities.begin() + index);
//			}
//			else
//			{
//				pos = spawnpoint;
//				sprite->setPosition(pos);
//				colBox->setPosition(pos);
//				sf::sleep(sf::seconds(0.1f));
//			}
//			return true;
//		}
//		index++;
//	}
//	return false;
//}

void Pacman::move(float deltaTime)
{
	pos.x += (vel.x * deltaTime);
	pos.y += (vel.y * deltaTime);
	sprite->setPosition(pos);
	colBox->setPosition(pos);

	// Too far left
	if (pos.x < 0 + (colBox->getLocalBounds().width / 2))
	{
		pos.x = 0 + (colBox->getLocalBounds().width / 2);
		colBox->setPosition(pos);
	}

	// Too far right
	if (pos.x > globals::SCREEN_WIDHT - (colBox->getLocalBounds().width / 2))
	{
		pos.x = globals::SCREEN_WIDHT - (colBox->getLocalBounds().width / 2);
		colBox->setPosition(pos);
	}

	// Too far up
	if (pos.y < 0 + (colBox->getLocalBounds().height / 2))
	{
		pos.y = 0 + (colBox->getLocalBounds().height / 2);
		colBox->setPosition(pos);
	}

	// Too far own
	if (pos.y > globals::SCREEN_HEIGHT - (colBox->getLocalBounds().height / 2))
	{
		pos.y = globals::SCREEN_HEIGHT - (colBox->getLocalBounds().height / 2);
		colBox->setPosition(pos);
	}
}

//void Pacman::render(sf::RenderWindow & renderWindow)
//{
//}
