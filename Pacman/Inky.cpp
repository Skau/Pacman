#include "Inky.h"
#include <iostream>
#include "Pacman.h"
#include "Blinky.h"
#include "Game.h"
Inky::Inky(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Tile> scatterTileIn, std::weak_ptr<Map> MapIn, Game& game, bool isClydeIn)
	: Enemy{ image, SpawnTile, scatterTileIn, MapIn, game, isClydeIn }
{
	std::cout << "Inky pos: " << pos.x << ", " << pos.y << std::endl;
}

void Inky::Chase()
{
	if (pacman.get() && game->getBlinky().get())
	{
		Direction dir = pacman->getMoveDirection();
		if (!pathToMoveTiles.size() || pacmanLastDirection != dir)
		{
			sf::Vector2f pacmanPos = pacman->getPos();
			sf::Vector2f twoTilesInFrontPos = sf::Vector2f(-1, -1);


			switch (dir)
			{
			case Direction::UP:
			{
				if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacmanPos.x, pacmanPos.y - 32), Direction::DEFAULT))
				{
					if (map->getTileAtLocation(sf::Vector2f(pacmanPos.x, pacmanPos.y - 32))->getIsWalkable())
					{
						twoTilesInFrontPos = sf::Vector2f(pacmanPos.x, pacmanPos.y - 32);
					}
					else
					{
						twoTilesInFrontPos = pacman->getPos();
					}
				}
			}
			case Direction::DOWN:
			{
				if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacmanPos.x, pacmanPos.y + 32), Direction::DEFAULT))
				{
					if (map->getTileAtLocation(sf::Vector2f(pacmanPos.x, pacmanPos.y + 32))->getIsWalkable())
					{
						twoTilesInFrontPos = sf::Vector2f(pacmanPos.x, pacmanPos.y + 32);
					}
					else
					{
						twoTilesInFrontPos = pacman->getPos();
					}
				}
			}
			case Direction::LEFT:
			{
				if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacmanPos.x - 32, pacmanPos.y), Direction::DEFAULT))
				{
					if (map->getTileAtLocation(sf::Vector2f(pacmanPos.x - 32, pacmanPos.y))->getIsWalkable())
					{
						twoTilesInFrontPos = sf::Vector2f(pacmanPos.x - 32, pacmanPos.y);
					}
					else
					{
						twoTilesInFrontPos = pacman->getPos();
					}
				}
			}
			case Direction::RIGHT:
			{
				if (map->checkIfAdjacentTileIsInOfRange(sf::Vector2f(pacmanPos.x + 32, pacmanPos.y), Direction::DEFAULT))
				{
					if (map->getTileAtLocation(sf::Vector2f(pacmanPos.x + 32, pacmanPos.y))->getIsWalkable())
					{
						twoTilesInFrontPos = sf::Vector2f(pacmanPos.x + 32, pacmanPos.y);
					}
					else
					{
						twoTilesInFrontPos = pacman->getPos();
					}
				}
			}
			default:
				break;
			}

			pacmanLastDirection = dir;

			sf::Vector2f movePos;

			if (game->getBlinky().get())
			{
				sf::Vector2f blinkyPos = game->getBlinky()->getPos();
				if (twoTilesInFrontPos.x > -1 && twoTilesInFrontPos != pacman->getPos())
				{
					if (twoTilesInFrontPos.x < blinkyPos.x)
					{
						movePos.x = twoTilesInFrontPos.x += -abs(twoTilesInFrontPos.x - blinkyPos.x);
					}
					else if (twoTilesInFrontPos.x > blinkyPos.x)
					{
						movePos.x = twoTilesInFrontPos.x += abs(twoTilesInFrontPos.x - blinkyPos.x);
					}
					else
					{
						movePos.x = twoTilesInFrontPos.x;
					}

					if (twoTilesInFrontPos.y < blinkyPos.y)
					{
						movePos.y = twoTilesInFrontPos.y -= abs(twoTilesInFrontPos.y - blinkyPos.y);
					}
					else if (twoTilesInFrontPos.y > blinkyPos.y)
					{
						movePos.y = twoTilesInFrontPos.y += abs(twoTilesInFrontPos.y - blinkyPos.y);
					}
					else
					{
						movePos.y = twoTilesInFrontPos.y;
					}
					findPath(pos, movePos);
				}
				else
				{
					findPath(pos, pacmanPos);
				}
			}
			else
			{
				findPath(pos, pacmanPos);
			}
		}
		else
		{
			move();
		}
	}
}
