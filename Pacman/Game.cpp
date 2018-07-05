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
		std::cout << "Failed to load red image!" << std::endl;
	}
	imageManager->addImage(sprite);
	if (!sprite.loadFromFile("images/blueImage.png"))
	{
		std::cout << "Failed to load blue image!" << std::endl;
	}
	imageManager->addImage(sprite);
	if (!sprite.loadFromFile("images/lightBlueImage.png"))
	{
		std::cout << "Failed to load blue image!" << std::endl;
	}
	imageManager->addImage(sprite);
	if (!sprite.loadFromFile("images/yellowImage.png"))
	{
		std::cout << "Failed to load blue image!" << std::endl;
	}
	imageManager->addImage(sprite);
	if (!sprite.loadFromFile("images/orangeImage.png"))
	{
		std::cout << "Failed to load blue image!" << std::endl;
	}
	imageManager->addImage(sprite);
	if (!sprite.loadFromFile("images/purpleImage.png"))
	{
		std::cout << "Failed to load blue image!" << std::endl;
	}
	imageManager->addImage(sprite);
}

void Game::beginPlay()
{
	map->loadMap();

	pacman = std::shared_ptr<Pacman>(new Pacman(imageManager->getImage(3), map->GetSpawnTile()));
    pacman->SetIsControllable(true);
	pacman->setMap(map);

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

		pacman->tick(timePerFrame.asSeconds());

	}
}

void Game::render()
{
	window->clear(sf::Color::Black);

	map->drawMap(*window);

	pacman->render(*window);

	window->display();
}

bool Game::checkIntersect()
{
	for (auto& Tile : map->getAllTiles())
	{
		if (Tile->getPos() == pacman->getPos())
		{
			std::cout << "Collided with tile: " << Tile->getTileID() << std::endl;
			return true;
		}
	}
	return false;
}

