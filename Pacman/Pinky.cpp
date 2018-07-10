#include "Pinky.h"
#include <iostream>
#include "Pacman.h"
#include "Game.h"

Pinky::Pinky(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Tile> scatterTileIn, std::weak_ptr<Map> MapIn, Game& game)
	: Enemy{ image, SpawnTile, scatterTileIn, MapIn, game }
{
	std::cout << "Pinky pos: " << pos.x << ", " << pos.y << std::endl;
}

void Pinky::childTick()
{

}

void Pinky::Chase()
{
	if (pacman.get())
	{
		Direction dir = pacman->getMoveDirection();
		if (!pathToMoveTiles.size() || dir != pacmanLastDirection)
		{
			sf::Vector2f location = sf::Vector2f(-1, -1);

			for (int i = 16; i <= 96; i += 16)
			{
				switch (dir)
				{
				case Direction::UP:
				{
					if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacman->getPos().x, pacman->getPos().y - i), dir))
					{
						if (map->getTileAtLocation(sf::Vector2f(pacman->getPos().x, pacman->getPos().y - i))->getIsWalkable())
						{
							location = sf::Vector2f(pacman->getPos().x, pacman->getPos().y - i);
							break;
						}
					}
					break;
				}
				case Direction::DOWN:
				{
					if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacman->getPos().x, pacman->getPos().y + i), dir))
					{
						if (map->getTileAtLocation(sf::Vector2f(pacman->getPos().x, pacman->getPos().y + i))->getIsWalkable())
						{
							location = sf::Vector2f(pacman->getPos().x, pacman->getPos().y + i);
							break;
						}
					}
					break;
				}
				case Direction::LEFT:
				{
					if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacman->getPos().x - i, pacman->getPos().y), dir))
					{
						if (map->getTileAtLocation(sf::Vector2f(pacman->getPos().x - i, pacman->getPos().y))->getIsWalkable())
						{
							location = sf::Vector2f(pacman->getPos().x - i, pacman->getPos().y);
							break;
						}
					}
					break;
				}
				case Direction::RIGHT:
				{
					if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacman->getPos().x + i, pacman->getPos().y), dir))
					{
						if (map->getTileAtLocation(sf::Vector2f(pacman->getPos().x + i, pacman->getPos().y))->getIsWalkable())
						{
							location = sf::Vector2f(pacman->getPos().x + i, pacman->getPos().y);
							break;
						}
					}
					break;
				}
				default:
					break;
				}
			}
			pacmanLastDirection = dir;
			if (location.x > -1)
			{
				findPath(pos, location);
			}
			else
			{
				findPath(pos, pacman->getPos());
			}
		}
		else
		{
			move();
		}
	}
}
