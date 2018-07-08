#include "Pacman.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"
#include "Tile.h"

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
					moveDirection = Direction::UP;
					isMoving = true;
					break;
				}
				case sf::Keyboard::S: 
				{
					moveDirection = Direction::DOWN;
					isMoving = true;
					break;
				}
				case sf::Keyboard::D:
				{ 
					moveDirection = Direction::RIGHT;
					isMoving = true;
					break;
				}
				case sf::Keyboard::A:
				{
					moveDirection = Direction::LEFT;
					isMoving = true;
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
	if(isMoving)
	move(deltaTime);
}

void Pacman::move(float deltaTime)
{
	if (map)
	{
		std::shared_ptr<Tile> tile;

		tile = map->getTileInDirectionFromLocation(pos, moveDirection);
		if (tile)
		{
			if (tile->getIsWalkable() && !tile->getIsPlayerBlock())
			{
				if (tile->getIsTeleporter())
				{
					for (auto& t : map->getAllTiles())
					{
						if (t->getIsTeleporter() && t != tile)
						{

							CurrentTile = t;
							break;
						}
					}
				}
				else
				{
					CurrentTile = tile;
				}
				pos = CurrentTile->getPos();
				CurrentTile->setPacmanIsHere(false);
				CurrentTile->setPacmanIsHere(true);
				if (CurrentTile->getHasDot())
				{
					CurrentTile->destroyDot();
				}
			}
			else
			{
				isMoving = false;
			}
		}
		else
		{
			isMoving = false;
		}

		sprite->setPosition(pos);
		colBox->setPosition(pos);
	}
	
}
