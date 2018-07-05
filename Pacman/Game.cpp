#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Globals.h"
#include "Game.h"
#include "Entity.h"
#include "Pacman.h"
#include "Blinky.h"
#include "picojson.h"
#include "ImageManager.h"
#include "Tile.h"
#include "Map.h"
void Game::init()
{
	window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(globals::SCREEN_WIDHT, globals::SCREEN_HEIGHT), "Pacman", sf::Style::Close));
	window->setFramerateLimit(120);
	window->setKeyRepeatEnabled(true);
	
	imageManager = std::shared_ptr<ImageManager>(new ImageManager());

	map = std::shared_ptr<Map>(new Map(imageManager));

	loadImages();

	beginPlay();
}

void Game::loadImages()
{
	sf::Image sprite;
	if (!sprite.loadFromFile("images/blackImage.png"))
	{
		std::cout << "Failed to load black image!" << std::endl;
	}
	imageManager->addImage(sprite);
	if (!sprite.loadFromFile("images/blueImage.png"))
	{
		std::cout << "Failed to load blue image!" << std::endl;
	}
	imageManager->addImage(sprite);
	if (!sprite.loadFromFile("images/lightBlueImage.png"))
	{
		std::cout << "Failed to load lightblue image!" << std::endl;
	}
	imageManager->addImage(sprite);
	if (!sprite.loadFromFile("images/yellowImage.png"))
	{
		std::cout << "Failed to load yellow image!" << std::endl;
	}
	imageManager->addImage(sprite);
	if (!sprite.loadFromFile("images/orangeImage.png"))
	{
		std::cout << "Failed to load orange image!" << std::endl;
	}
	imageManager->addImage(sprite);
	if (!sprite.loadFromFile("images/purpleImage.png"))
	{
		std::cout << "Failed to load purple image!" << std::endl;
	}
	imageManager->addImage(sprite);
	if (!sprite.loadFromFile("images/redImage.png"))
	{
		std::cout << "Failed to load red image!" << std::endl;
	}
	imageManager->addImage(sprite);
}

void Game::beginPlay()
{
	map->loadMap();

	pacman = std::shared_ptr<Pacman>(new Pacman(imageManager->getImage(3), map->getPlayerSpawnTile()));
    pacman->SetIsControllable(true);
	pacman->setMap(map);
	allEntities.push_back(pacman);

	allEntities.push_back(std::shared_ptr<Blinky>(new Blinky(imageManager->getImage(6), map->getEnemy1Spawnpoint(), map)));

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
		pacman->handleEvent(event);
	}
}

void Game::mainTick()
{
	while (timeSinceLastUpdate > timePerFrame)
	{
		timeSinceLastUpdate -= timePerFrame;

		for (auto& entity : allEntities)
			entity->tick(timePerFrame.asSeconds());

	}
}

void Game::render()
{
	window->clear(sf::Color::Black);

	map->drawMap(*window);

	for (auto& entity : allEntities)
		entity->render(*window);

	window->display();
}

