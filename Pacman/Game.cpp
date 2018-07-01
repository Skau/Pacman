#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Globals.h"
#include "Game.h"
#include "Entity.h"


void Game::init()
{
	window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(globals::SCREEN_WIDHT, globals::SCREEN_HEIGHT), "lol", sf::Style::Close));
	window->setFramerateLimit(60);
	window->setKeyRepeatEnabled(false);

	beginPlay();
}

void Game::beginPlay()
{
	entity = std::shared_ptr<Entity>(new Entity);
	entity->init();
	entity->SetIsControllable(true);

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
		entity->handleEvent(event);
	}
}

void Game::mainTick()
{
	while (timeSinceLastUpdate > timePerFrame)
	{
		timeSinceLastUpdate -= timePerFrame;

		entity->tick(timePerFrame.asSeconds());
	}
}

void Game::render()
{
	window->clear(sf::Color::Black);

	entity->render(*window);

	window->display();
}
