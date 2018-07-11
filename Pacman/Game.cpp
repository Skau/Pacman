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
}

void Game::init()
{
	dotsLeft = 0;

	createTexts();

	map = std::make_shared<Map>(imageManager, *this);

	gamePaused = true;

	beginPlay();
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
	pauseText->setPosition(globals::SCREEN_WIDTH - 427  , globals::SCREEN_HEIGHT - 564);
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
	pacman = std::make_shared<Pacman>(imageManager->getImage(0), map->getPlayerSpawnTile(), map, *this);
	pacman->SetIsControllable(true);
	allEntities.push_back(pacman);

	blinky = std::make_shared<Blinky>(imageManager->getImage(3), map->getEnemy1Spawnpoint(), map->getEnemy1ScatterTile(), map, *this, false);
	allEntities.push_back(blinky);
	blinky->startMoving();

	pinky = std::make_shared<Pinky>(imageManager->getImage(4), map->getEnemy2Spawnpoint(), map->getEnemy2ScatterTile(), map, *this, false);
	allEntities.push_back(pinky);
	pinky->startMoving();

	inky = std::make_shared<Inky>(imageManager->getImage(6), map->getEnemy3Spawnpoint(), map->getEnemy3ScatterTile(), map, *this, false);
	allEntities.push_back(inky);

	clyde = std::make_shared<Clyde>(imageManager->getImage(1), map->getEnemy4Spawnpoint(), map->getEnemy4ScatterTile(), map, *this, true);
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

		if (allEntities.size() == 1)
			resetGame();

		for(std::shared_ptr<Entity> entity : allEntities)
		{
			if (entity.get())
				if (entity->getIsDead())
				{
					destroyEntity(entity);
				}
				else 
				{
					entity->tick(timePerFrame.asSeconds());
				}
		}
	}
}

void Game::render()
{
	window->clear(sf::Color::Black);

	map->drawMap(*window);

	for (std::shared_ptr<Entity>& entity : allEntities)
		if(entity.get())
		entity->render(*window);

	dotsText->setString("Dots left: " + std::to_string(dotsLeft));
	window->draw(*dotsText);
	window->draw(*resetText);
	window->draw(*pauseText);
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

void Game::destroyEntity(std::shared_ptr<Entity>& entity)
{
	entity->destroyEntity();
	allEntities.erase(std::remove(allEntities.begin(), allEntities.end(), entity), allEntities.end());
	entity.reset();
}

void Game::triggerFrightenedMode()
{
	std::cout << "Triggered frightened mode!\n";
	if (blinky.get())
		blinky->triggerFrightenedMode();
	if (pinky.get())
		pinky->triggerFrightenedMode();
	if (inky.get())
		inky->triggerFrightenedMode();
	if (clyde.get())
		clyde->triggerFrightenedMode();
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

