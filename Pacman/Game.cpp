#include <SFML/Graphics.hpp>
#include <memory>
#include <iostream>
#include "Globals.h"
#include "Game.h"
#include "ImageManager.h"
#include "Entity.h"
#include "Map.h"
#include "Tile.h"
#include "Pacman.h"
#include "Blinky.h"
#include "Pinky.h"
#include "Inky.h"
#include "Clyde.h"

Game::Game() : inkyHasStartedMoving{ false }, clydeHasStartedMoving{ false }
{
	window = std::unique_ptr<sf::RenderWindow>(new sf::RenderWindow(sf::VideoMode(globals::SCREEN_WIDTH, globals::SCREEN_HEIGHT), "Pacman", sf::Style::Close));
	window->setFramerateLimit(60);
	window->setKeyRepeatEnabled(true);

	imageManager = std::make_shared<ImageManager>(ImageManager());

	loadImages();
}

void Game::init()
{
	dotsLeft = 0;

	createTexts();

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
		return;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/blueImage.png"))
	{
		std::cout << "Failed to load blue image!" << std::endl;
		return;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/lightBlueImage.png"))
	{
		std::cout << "Failed to load lightblue image!" << std::endl;
		return;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/yellowImage.png"))
	{
		std::cout << "Failed to load yellow image!" << std::endl;
		return;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/orangeImage.png"))
	{
		std::cout << "Failed to load orange image!" << std::endl;
		return;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/purpleImage.png"))
	{
		std::cout << "Failed to load purple image!" << std::endl;
		return;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/redImage.png"))
	{
		std::cout << "Failed to load red image!" << std::endl;
		return;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/pinkImage.png"))
	{
		std::cout << "Failed to load pink image!" << std::endl;
		return;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/dotImage.png"))
	{
		std::cout << "Failed to load dot image!" << std::endl;
		return;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/whiteImage.png"))
	{
		std::cout << "Failed to load white image!" << std::endl;
		return;
	}
	imageManager->addImage(image);
	if (!image.loadFromFile("images/pelletImage.png"))
	{
		std::cout << "Failed to load pelletImage image!" << std::endl;
		return;
	}
	imageManager->addImage(image);
}

void Game::createTexts()
{
	font = std::make_unique<sf::Font>(sf::Font());
	pauseText = std::make_unique<sf::Text>(sf::Text());
	dotsText = std::make_unique<sf::Text>(sf::Text());
	resetText = std::make_unique<sf::Text>(sf::Text());
	showPathText = std::make_unique<sf::Text>(sf::Text());

	if (!font->loadFromFile("font/arial.ttf"))
	{
		std::cout << "Could not load font from file!\n";
		return;
	}
	pauseText->setFont(*font);
	pauseText->setString("Press spacebar to toggle pause");
	pauseText->setPosition(globals::SCREEN_WIDTH - 427 , globals::SCREEN_HEIGHT - 564);
	pauseText->setOutlineThickness(2);

	dotsText->setFont(*font);
	dotsText->setString("Dots left: " + std::to_string(dotsLeft));
	dotsText->setCharacterSize(25);
	dotsText->setPosition((float)(globals::SCREEN_WIDTH - 170), globals::SCREEN_HEIGHT-45);

	showPathText->setFont(*font);
	showPathText->setString("Press 'F' to show enemy paths");
	showPathText->setCharacterSize(18);
	showPathText->setPosition(globals::SCREEN_WIDTH - 422, globals::SCREEN_HEIGHT - 28);
	showPathText->setOutlineThickness(2);

	resetText->setFont(*font);
	resetText->setString("Press 'R' to reset game");
	resetText->setCharacterSize(18);
	resetText->setPosition(globals::SCREEN_WIDTH - 422, globals::SCREEN_HEIGHT - 45);
	resetText->setOutlineThickness(2);
}

void Game::beginPlay()
{
	map->loadMap();

	createEnemies();

	sf::Time timeSinceLastUpdate = sf::Time::Zero;

	render();

	gameLoop();
}

void Game::createEnemies()
{
	pacman = std::make_shared<Pacman>(imageManager->getImage(3), map->getPlayerSpawnTile(), map, *this);
	pacman->SetIsControllable(true);
	allEntities.push_back(pacman);

	blinky = std::make_shared<Blinky>(imageManager->getImage(6), map->getEnemy1Spawnpoint(), map->getEnemy1ScatterTile(), map, *this, false);
	allEntities.push_back(blinky);
	blinky->startMoving();

	pinky = std::make_shared<Pinky>(imageManager->getImage(7), map->getEnemy2Spawnpoint(), map->getEnemy2ScatterTile(), map, *this, false);
	allEntities.push_back(pinky);
	pinky->startMoving();

	inky = std::make_shared<Inky>(imageManager->getImage(2), map->getEnemy3Spawnpoint(), map->getEnemy3ScatterTile(), map, *this, false);
	allEntities.push_back(inky);

	clyde = std::make_shared<Clyde>(imageManager->getImage(3), map->getEnemy4Spawnpoint(), map->getEnemy4ScatterTile(), map, *this, true);
	allEntities.push_back(clyde);
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
			{
				gamePaused = true;
			}
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
		else if (event.type == sf::Event::KeyPressed && event.key.code == sf::Keyboard::F)
		{
			toggleAIShowPath();
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

		if (dotsLeft <= 267)
		{
			if(inky.get())
			if (!inkyHasStartedMoving)
			{
				std::cout << "Inky joined!\n";
				inky->startMoving();
				inkyHasStartedMoving = true;
			}
		}
		if (dotsLeft <= 198)
		{
			if (clyde.get())
			{
				if (!clydeHasStartedMoving)
				{
					std::cout << "Clyde joined!\n";
					clyde->startMoving();
					clydeHasStartedMoving = true;
				}
			}
		}

		for (std::shared_ptr<Entity>& entity : allEntities)
			entity->tick(timePerFrame.asSeconds());

	}
}

void Game::render()
{
	window->clear(sf::Color::Black);

	map->drawMap(*window);

	for (std::shared_ptr<Entity>& entity : allEntities)
		entity->render(*window);

	if (gamePaused)
		window->draw(*pauseText);

	dotsText->setString("Dots left: " + std::to_string(dotsLeft));
	window->draw(*dotsText);
	window->draw(*resetText);
	window->draw(*showPathText);

	window->display();
}

void Game::toggleAIShowPath()
{
	if(blinky.get())
		blinky->toggleShowPath();
	if(pinky.get())
		pinky->toggleShowPath();
	if(inky.get())
		inky->toggleShowPath();
	if(clyde.get())
		clyde->toggleShowPath();
}

void Game::triggerGhostMode()
{
	std::cout << "Triggered ghost mode!\n";
	if (blinky.get())
		blinky->triggerGhostMode();
	if (pinky.get())
		pinky->triggerGhostMode();
	if (inky.get())
		inky->triggerGhostMode();
	if (clyde.get())
		clyde->triggerGhostMode();
}

// TODO: ???
void Game::killEnemy()
{
	if (blinky->getPos() == pacman->getPos())
	{
		blinky->destroyEntity();
		blinky.reset();
		allEntities.erase(std::remove(allEntities.begin(), allEntities.end(), blinky), allEntities.end());
	}
	if (pinky->getPos() == pacman->getPos())
	{
		pinky->destroyEntity();
		pinky.reset();
		allEntities.erase(std::remove(allEntities.begin(), allEntities.end(), pinky), allEntities.end());
	}
	if (inky->getPos() == pacman->getPos())
	{
		inky->destroyEntity();
		inky.reset();
		allEntities.erase(std::remove(allEntities.begin(), allEntities.end(), inky), allEntities.end());
	}
	if (clyde->getPos() == pacman->getPos())
	{
		clyde->destroyEntity();
		clyde.reset();
		allEntities.erase(std::remove(allEntities.begin(), allEntities.end(), clyde), allEntities.end());
	}
}

void Game::resetGame()
{
	// Reset pointers and dots left, then re-run init()
	for (auto entity : allEntities)
	{
		entity.reset();
	}
	allEntities.clear();

	inkyHasStartedMoving = false;
	clydeHasStartedMoving = false;
	dotsLeft = 0;
	map.reset();
	init();
}

