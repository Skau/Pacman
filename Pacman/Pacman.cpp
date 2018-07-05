#include "Pacman.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"
#include "Tile.h"
#include "Map.h"

Pacman::Pacman(sf::Image & image, std::shared_ptr<Tile>& SpawnTile) : Entity{ image, SpawnTile }
{
	std::cout << "Player pos: " << pos.x << ", " << pos.y << std::endl;
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
				if (CurrentTile->GetTileUp()->getIsWalkable() && !CurrentTile->GetTileUp()->getIsPlayerBlock())
				{
					std::cout << "Tile " << CurrentTile->GetTileUp()->getTileID() << " is walkable above!\n";
					pos = CurrentTile->GetTileUp()->getPos();
					CurrentTile->setPacmanIsHere(false);
					CurrentTile = CurrentTile->GetTileUp();
					CurrentTile->setPacmanIsHere(true);
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
				if (CurrentTile->getTileDown()->getIsWalkable() && !CurrentTile->getTileDown()->getIsPlayerBlock())
				{
					std::cout << "Tile " << CurrentTile->getTileDown()->getTileID() << " is walkable below!\n";
					pos = CurrentTile->getTileDown()->getPos();
					CurrentTile->setPacmanIsHere(false);
					CurrentTile = CurrentTile->getTileDown();
					CurrentTile->setPacmanIsHere(true);
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
							CurrentTile->setPacmanIsHere(false);
							CurrentTile = tile;
							CurrentTile->setPacmanIsHere(true);
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
					if (CurrentTile->getTileRight()->getIsWalkable() && !CurrentTile->getTileRight()->getIsPlayerBlock())
					{
						std::cout << "Tile " << CurrentTile->getTileRight()->getTileID() << " is walkable to the right!\n";
						pos = CurrentTile->getTileRight()->getPos();
						CurrentTile->setPacmanIsHere(false);
						CurrentTile = CurrentTile->getTileRight();
						CurrentTile->setPacmanIsHere(true);
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
							CurrentTile->setPacmanIsHere(false);
							CurrentTile = tile;
							CurrentTile->setPacmanIsHere(true);
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
					if (CurrentTile->getTileLeft()->getIsWalkable() && !CurrentTile->getTileLeft()->getIsPlayerBlock())
					{
						std::cout << "Tile " << CurrentTile->getTileLeft()->getTileID() << " is walkable to the left!\n";
						pos = CurrentTile->getTileLeft()->getPos();
						CurrentTile->setPacmanIsHere(false);
						CurrentTile = CurrentTile->getTileLeft();
						CurrentTile->setPacmanIsHere(true);
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
