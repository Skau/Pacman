#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Globals.h"
#include "Game.h"
#include "Entity.h"
#include "Pacman.h"
#include "Blinky.h"
#include "Dot.h"
#include "picojson.h"

void Game::init()
{
	sf::ContextSettings settings;
	settings.antialiasingLevel = 8;

	window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(globals::SCREEN_WIDHT, globals::SCREEN_HEIGHT), "lol", sf::Style::Close));
	window->setFramerateLimit(60);
	window->setKeyRepeatEnabled(false);

	beginPlay();
}

void Game::beginPlay()
{
	pacman = std::shared_ptr<Pacman>(new Pacman);
	pacman->init();
	pacman->setPos(sf::Vector2f(100, 100));
	pacman->SetIsControllable(true);

	std::shared_ptr<Blinky> b = std::shared_ptr<Blinky>(new Blinky);
	b->init();
	b->setPos(sf::Vector2f(700, 100));
	allEntities.push_back(b);
	
	for (int i = 100; i < 600; i+=100)
	{
		allEntities.push_back(std::shared_ptr<Dot>(new Dot));
		allEntities.back()->init();
		allEntities.back()->setPos(sf::Vector2f((float)i, 450));
	}

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	gameLoop();
}

void Game::gameLoop()
{
	while (window->isOpen())
	{
		timeSinceLastUpdate += frameClock.restart();
		
		handleEvents();

		mainTick();

		int index = 0;
		for (auto& entity : allEntities)
		{
			if (entity->getIsDestroyed())
			{
				allEntities.erase(allEntities.begin() + index);
			}
			index++;
		}

		render();
	}
}

void Game::handleEvents()
{
	sf::Event event;
	while (window->pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			window->close();
		}
		pacman->handleEvent(event);
	}
}

void Game::mainTick()
{
	while (timeSinceLastUpdate > timePerFrame)
	{
		timeSinceLastUpdate -= timePerFrame;

		pacman->tick(timePerFrame.asSeconds());

		for (auto& entity : allEntities)
		{
			checkIntersect(entity->getColBox());
		}

		for (auto& entity : allEntities)
		{

			entity->tick(timePerFrame.asSeconds());
		}
	}
}

void Game::render()
{
	window->clear(sf::Color::Black);




	pacman->render(*window);

	for (auto& entity : allEntities)
	{
		entity->render(*window);
	}

	window->display();
}

bool Game::checkIntersect(sf::RectangleShape& other)
{
	int index = 0;
	for (auto& entity : allEntities)
	{
		if (pacman->getColBox().getGlobalBounds().intersects(entity->getColBox().getGlobalBounds()))
		{
			auto d = std::dynamic_pointer_cast<Dot>(entity);
			if (d)
			{
				d->setIsDestroyed(true);
				allEntities.erase(allEntities.begin() + index);
			}
			else
			{
				pacman->setPos(sf::Vector2f(400,300));
			}
			return true;
		}
		index++;
	}
	return false;
}

