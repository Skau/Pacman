#include "Pacman.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include "Globals.h"
#include "Game.h"
#include "Tile.h"
Pacman::Pacman(sf::Image & image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Map> mapIn, Game& game) : Entity{ image, SpawnTile, mapIn, game }
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
				case sf::Keyboard::Up:
				{
					moveDirection = Direction::UP;
					isMoving = true;
					break;
				}
				case sf::Keyboard::Down: 
				{
					moveDirection = Direction::DOWN;
					isMoving = true;
					break;
				}
				case sf::Keyboard::Right:
				{ 
					moveDirection = Direction::RIGHT;
					isMoving = true;
					break;
				}
				case sf::Keyboard::Left:
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
				lastDirection = moveDirection;
				pos = CurrentTile->getPos();
				if (CurrentTile->getHasDot())
				{
					CurrentTile->destroyDot();
				}
				else if (CurrentTile->getHasPellet())
				{
					CurrentTile->destroyPellet();
					game->triggerFrightenedMode();
				}
			}
			else
			{
				moveDirection = lastDirection;
			}
		}
		else
		{
			moveDirection = lastDirection;
		}

		sprite->setPosition(pos);
		colBox->setPosition(pos);
	}
	
}
