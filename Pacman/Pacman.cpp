#include "Pacman.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"
#include "Tile.h"
#include "Map.h"

Pacman::Pacman(sf::Image & image, std::weak_ptr<Tile> SpawnTile, Game& game) : Entity{ image, SpawnTile, game }
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
				case sf::Keyboard::S: 
				{
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
			if (CurrentTile->GetTileUp())
			{
				if (CurrentTile->GetTileUp()->getIsWalkable() && !CurrentTile->GetTileUp()->getIsPlayerBlock())
				{
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
					isMovingUp = false;
				}
			}
			else
			{
				isMovingUp = false;
			}
		}
		else if (isMovingDown)
		{
			if (CurrentTile->getTileDown())
			{
				if (CurrentTile->getTileDown()->getIsWalkable() && !CurrentTile->getTileDown()->getIsPlayerBlock())
				{
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
					isMovingDown = false;
				}
			}
			else
			{
				isMovingDown = false;
			}
		}
		else if (isMovingRight)
		{
			if (CurrentTile->getTileRight())
			{
				if (CurrentTile->getTileRight()->getIsTeleporter())
				{
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
						isMovingRight = false;
					}
				}
			}
			else
			{
				isMovingRight = false;
			}
		}
		else if (isMovingLeft)
		{
			if (CurrentTile->getTileLeft())
			{
				if (CurrentTile->getTileLeft()->getIsTeleporter())
				{
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
						isMovingLeft = false;
					}
				}
			}
			else
			{
				isMovingLeft = false;
			}
		}
		sprite->setPosition(pos);
		colBox->setPosition(pos);
	}
}
