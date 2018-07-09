#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Globals.h"
#include "Game.h"
#include "Entity.h"
#include "Pacman.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "ImageManager.h"
#include "Tile.h"
#include "Map.h"

Game::Game()
{
	window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT), "Pacman", sf::Style::Close));
	window->setFramerateLimit(120);
	window->setKeyRepeatEnabled(true);

	imageManager = std::make_shared<ImageManager>(ImageManager());

	loadImages();

	createTexts();
}

void Game::init()
{
	map = std::make_shared<Map>(imageManager, *this);

	gamePaused = true;

	beginPlay();
}

void Game::loadImages()
{
	sf::Image image;
	if (!image.loadFromFile("images/blackImage.png"))
	{
		std::cout << "Failed to load black image!" << std::endl;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/blueImage.png"))
	{
		std::cout << "Failed to load blue image!" << std::endl;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/lightBlueImage.png"))
	{
		std::cout << "Failed to load lightblue image!" << std::endl;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/yellowImage.png"))
	{
		std::cout << "Failed to load yellow image!" << std::endl;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/orangeImage.png"))
	{
		std::cout << "Failed to load orange image!" << std::endl;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/purpleImage.png"))
	{
		std::cout << "Failed to load purple image!" << std::endl;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/redImage.png"))
	{
		std::cout << "Failed to load red image!" << std::endl;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/pinkImage.png"))
	{
		std::cout << "Failed to load red image!" << std::endl;
	}
	imageManager->addImage(image);
}

void Game::createTexts()
{
	font = std::make_unique<sf::Font>(sf::Font());
	pauseText = std::make_unique<sf::Text>(sf::Text());
	dotsText = std::make_unique<sf::Text>(sf::Text());
	resetText = std::make_unique<sf::Text>(sf::Text());

	if (!font->loadFromFile("font/arial.ttf"))
	{
		std::cout << "Could not load font from file!\n";
	}
	pauseText->setFont(*font);
	pauseText->setString("Press spacebar to toggle pause");
	pauseText->setOrigin(sf::Vector2f(pauseText->getScale().x / 2, pauseText->getScale().y / 2));
	pauseText->setPosition(5,12);
	pauseText->setOutlineThickness(2);

	dotsLeft = 0;
	dotsText->setFont(*font);
	dotsText->setString("Dots left: " + std::to_string(dotsLeft));
	dotsText->setCharacterSize(25);
	dotsText->setOrigin(sf::Vector2f(dotsText->getScale().x / 2, dotsText->getScale().y / 2));
	dotsText->setPosition((float)(globals::SCREEN_WIDTH / 1.65), globals::SCREEN_HEIGHT-40);

	resetText->setFont(*font);
	resetText->setString("Press 'R' to reset game");
	resetText->setCharacterSize(20);
	resetText->setOrigin(sf::Vector2f(resetText->getScale().x / 2, resetText->getScale().y / 2));
	resetText->setPosition(globals::SCREEN_WIDTH / 14, globals::SCREEN_HEIGHT - 35);
}

void Game::beginPlay()
{
	map->loadMap();

	pacman = std::make_shared<Pacman>(imageManager->getImage(3), map->getPlayerSpawnTile(), *this);
    pacman->SetIsControllable(true);
	pacman->setMap(map);
	allEntities.push_back(pacman);

	blinky = std::make_shared<Blinky>(imageManager->getImage(6), map->getEnemy1Spawnpoint(), map->getEnemy1ScatterTile(), map, *this);
	allEntities.push_back(blinky);
	pinky = std::make_shared<Pinky>(imageManager->getImage(7), map->getEnemy2Spawnpoint(), map->getEnemy2ScatterTile(), map, *this);
	allEntities.push_back(pinky);
	inky = std::make_shared<Inky>(imageManager->getImage(2), map->getEnemy3Spawnpoint(), map->getEnemy3ScatterTile(), map, *this);
	allEntities.push_back(inky);

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	render();

	gameLoop();
}

void Game::gameLoop()
{
	while (window->isOpen())
	{
		handleEvents();

		if(!gamePaused)
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
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::Space)
		{
			if (!gamePaused)
				gamePaused = true;
			else
			{
				gamePaused = false;
				frameClock.restart();
			}
		}
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::R)
		{
			resetGame();
		}
		if(!gamePaused)
		pacman->handleEvent(event);
	}
}

void Game::mainTick()
{
	timeSinceLastUpdate += frameClock.restart();
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

	if (gamePaused)
		window->draw(*pauseText);

	dotsText->setString("Dots left: " + std::to_string(dotsLeft));
	window->draw(*dotsText);

	window->draw(*resetText);

	window->display();
}

void Game::resetGame()
{
	for (auto entity : allEntities)
	{
		entity.reset();
	}
	allEntities.clear();

	dotsLeft = 0;
	map.reset();
	init();
}

