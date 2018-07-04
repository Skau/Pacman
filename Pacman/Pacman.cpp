#include "Pacman.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include "Globals.h"
#include "Dot.h"
#include "Tile.h"
#include "Map.h"
Pacman::Pacman(sf::Image& image) : Entity{ image }
{
}

Pacman::Pacman(sf::Image & image, std::shared_ptr<Tile> SpawnTile) : Entity{ image }
{
	if (!texture->loadFromImage(image))
	{
		std::cerr << "Failed to load texture for entity!\n";
	}
	texture->setSmooth(true);
	sprite->setTexture(*texture);

	auto p = texture->getSize();
	auto pp = sf::Vector2f(p);
	sprite->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));
	if (SpawnTile)
	{
		pos = SpawnTile->getPos();
		CurrentTile = SpawnTile;
	}
	spawnpoint = pos;
	sprite->setPosition(pos);
	colBox->setSize(pp);
	colBox->setOrigin(sf::Vector2f(pp.x / 2, pp.y / 2));

	colBox->setFillColor(sf::Color::Transparent);
	colBox->setPosition(pos);

	std::cout << "Player pos: " << pos.x << ", " << pos.y << std::endl;
}

void Pacman::init(std::shared_ptr<Tile>& CurrentTileIn)
{
}

void Pacman::handleEvent(sf::Event & event)
{
	if (map)
	{
		if (isControllable)
		{
			if (event.type == sf::Event::KeyPressed)
			{
				switch (event.key.code)
				{
				case sf::Keyboard::W:
				{
					isMovingUp = true;
					isMovingDown = false;
					isMovingRight = false;
					isMovingLeft = false;
					break;
				}
				case sf::Keyboard::S: {
					isMovingUp = false;
					isMovingDown = true;
					isMovingRight = false;
					isMovingLeft = false;
					break;
				}
				case sf::Keyboard::D:
				{ 
					isMovingUp = false;
					isMovingDown = false;
					isMovingRight = true;
					isMovingLeft = false;
					break;
				}
				case sf::Keyboard::A:
				{
					isMovingUp = false;
					isMovingDown = false;
					isMovingRight = false;
					isMovingLeft = true;
					break;
				}
				default: break;
				}
			}
		}
	}
}

void Pacman::tick(float deltaTime)
{
	move(deltaTime);
}

void Pacman::move(float deltaTime)
{
	if (map)
	{
		if (isMovingUp)
		{
			std::cout << "IsMovingUp\n";

			if (CurrentTile->GetTileUp())
			{
				if (CurrentTile->GetTileUp()->getIsWalkable())
				{
					std::cout << "Tile " << CurrentTile->GetTileUp()->getTileID() << " is walkable above!\n";
					pos = CurrentTile->GetTileUp()->getPos();
					CurrentTile = CurrentTile->GetTileUp();
					if (CurrentTile->getHasDot())
					{
						CurrentTile->destroyDot();
					}

				}
				else
				{
					std::cout << "No walkable tile above!\n";
					isMovingUp = false;
				}
			}
			else
			{
				std::cout << "No walkable tile above!\n";
				isMovingUp = false;
			}
		}
		else if (isMovingDown)
		{
			if (CurrentTile->getTileDown())
			{
				if (CurrentTile->getTileDown()->getIsWalkable())
				{
					std::cout << "Tile " << CurrentTile->getTileDown()->getTileID() << " is walkable below!\n";
					pos = CurrentTile->getTileDown()->getPos();
					CurrentTile = CurrentTile->getTileDown();
					if (CurrentTile->getHasDot())
					{
						CurrentTile->destroyDot();
					}
				}
				else
				{
					std::cout << "No walkable tile below!\n";
					isMovingDown = false;
				}
			}
			else
			{
				std::cout << "No walkable tile below!\n";
				isMovingDown = false;
			}
		}
		else if (isMovingRight)
		{
			if (CurrentTile->getTileRight())
			{
				if (CurrentTile->getTileRight()->getIsTeleporter())
				{
					std::cout << "Tile " << CurrentTile->getTileRight()->getTileID() << " is teleporter to the right!\n";
					if (CurrentTile->getTileRight()->getHasDot())
					{
						CurrentTile->destroyDot();
					}
					for (auto& tile : map->getAllTiles())
					{
						if (tile->getIsTeleporter() && tile != CurrentTile->getTileRight())
						{
							pos = tile->getPos();
							CurrentTile = tile;
							if (CurrentTile->getHasDot())
							{
								CurrentTile->destroyDot();
							}
							break;
						}
					}
				}
				else
				{
					if (CurrentTile->getTileRight()->getIsWalkable())
					{
						std::cout << "Tile " << CurrentTile->getTileRight()->getTileID() << " is walkable to the right!\n";
						pos = CurrentTile->getTileRight()->getPos();
						CurrentTile = CurrentTile->getTileRight();
						if (CurrentTile->getHasDot())
						{
							CurrentTile->destroyDot();
						}
					}
					else
					{
						std::cout << "No walkable tile to the right!\n";
						isMovingRight = false;
					}
				}
			}
			else
			{
				std::cout << "No walkable tile to the right!\n";
				isMovingRight = false;
			}
		}
		else if (isMovingLeft)
		{
			if (CurrentTile->getTileLeft())
			{
				if (CurrentTile->getTileLeft()->getIsTeleporter())
				{
					std::cout << "Tile " << CurrentTile->getTileLeft()->getTileID() << " is teleporter to the left!\n";
					if (CurrentTile->getTileLeft()->getHasDot())
					{
						CurrentTile->destroyDot();
					}
					for (auto& tile : map->getAllTiles())
					{
						if (tile->getIsTeleporter() && tile != CurrentTile->getTileLeft())
						{
							pos = tile->getPos();
							CurrentTile = tile;
							if (CurrentTile->getHasDot())
							{
								CurrentTile->destroyDot();
							}
							break;
						}
					}
				}
				else
				{
					if (CurrentTile->getTileLeft()->getIsWalkable())
					{
						std::cout << "Tile " << CurrentTile->getTileLeft()->getTileID() << " is walkable to the left!\n";
						pos = CurrentTile->getTileLeft()->getPos();
						CurrentTile = CurrentTile->getTileLeft();
						if (CurrentTile->getTileRight()->getHasDot())
						{
							CurrentTile->destroyDot();
						}
					}
					else
					{
						std::cout << "No walkable tile to the left!\n";
						isMovingLeft = false;
					}
				}
			}
			else
			{
				std::cout << "No walkable tile to the left!\n";
				isMovingLeft = false;
			}
		}
		sprite->setPosition(pos);
		colBox->setPosition(pos);
	}
}
