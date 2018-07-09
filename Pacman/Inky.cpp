#include "Inky.h"
#include <iostream>
#include "Pacman.h"
#include "Blinky.h"
#include "Game.h"
Inky::Inky(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Tile> scatterTileIn, std::weak_ptr<Map> MapIn, Game& game)
	: Enemy{ image, SpawnTile, scatterTileIn, MapIn, game }
{
	std::cout << "Inky pos: " << pos.x << ", " << pos.y << std::endl;
}


void Inky::childTick()
{
}

void Inky::Chase()
{
	if (pacman.get() && game->getBlinky().get())
	{
		Direction dir = pacman->getMoveDirection();
		if (!pathToMoveTiles.size() || pacmanLastDirection != dir)
		{
			sf::Vector2f pacmanLoc = pacman->getPos();
			sf::Vector2f blinkyLoc = game->getBlinky()->getPos();
			sf::Vector2f location = sf::Vector2f(-1, -1);

			float valueToAddToX = (pos.x - blinkyLoc.x) / 16;
			float valueToAddToY = (pos.y - blinkyLoc.y) / 16;
			
			sf::Vector2f location = sf::Vector2f(valueToAddToX, valueToAddToY);

			switch (dir)
			{
			case Direction::UP:
			{
				if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacmanLoc.x, pacmanLoc.y - 32), Direction::DEFAULT))
				{
					if (map->getTileAtLocation(sf::Vector2f(pacmanLoc.x, pacmanLoc.y - 32))->getIsWalkable())
					{
						location = sf::Vector2f(pacmanLoc.x, pacmanLoc.y - 32);
					}
					else
					{
						location = pacman->getPos();
					}
				}
			}
			case Direction::DOWN:
			{
				if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacmanLoc.x, pacmanLoc.y + 32), Direction::DEFAULT))
				{
					if (map->getTileAtLocation(sf::Vector2f(pacmanLoc.x, pacmanLoc.y + 32))->getIsWalkable())
					{
						location = sf::Vector2f(pacmanLoc.x, pacmanLoc.y + 32);
					}
					else
					{
						location = pacman->getPos();
					}
				}
			}
			case Direction::LEFT:
			{
				if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacmanLoc.x - 32, pacmanLoc.y), Direction::DEFAULT))
				{
					if (map->getTileAtLocation(sf::Vector2f(pacmanLoc.x - 32, pacmanLoc.y))->getIsWalkable())
					{
						location = sf::Vector2f(pacmanLoc.x - 32, pacmanLoc.y);
					}
					else
					{
						location = pacman->getPos();
					}
				}
			}
			case Direction::RIGHT:
			{
				if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacmanLoc.x + 32, pacmanLoc.y), Direction::DEFAULT))
				{
					if (map->getTileAtLocation(sf::Vector2f(pacmanLoc.x + 32, pacmanLoc.y))->getIsWalkable())
					{
						location = sf::Vector2f(pacmanLoc.x + 32, pacmanLoc.y);
					}
					else
					{
						location = pacman->getPos();
					}
				}
			}
			default:
				break;
			}

			pacmanLastDirection = dir;

			// Found possible location
			if (location.x > -1)
			{
				sf::Vector2f moveLoc = sf::Vector2f((abs(location.x - blinkyLoc.x)), (abs(location.y - blinkyLoc.y))) * 2.f;
				findPath(pos, moveLoc);
				std::cout << moveLoc.x << ", " << moveLoc.y << std::endl;
			}
		}
		else
		{
			move();
		}
	}
}
