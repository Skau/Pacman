#include "Clyde.h"
#include <iostream>
#include "Pacman.h"
Clyde::Clyde(sf::Image & image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Tile> scatterTileIn, std::weak_ptr<Map> MapIn, Game & game, bool isClydeIn)
	: Enemy(image, SpawnTile, scatterTileIn, MapIn, game, isClydeIn)
{
	std::cout << "Clyde pos: " << pos.x << ", " << pos.y << std::endl;
}

void Clyde::Chase()
{
	if (pacman.get())
	{
		if (abs(pacman->getPos().x - pos.x) / 16 + abs(pacman->getPos().y - pos.y) / 16 < 8)
		{
			if (!pathToMoveTiles.size())
			{
				findPath(pos, scatterTile->getPos());
			}
			else
			{
				if (pathToMoveTiles[0]->getPos() != scatterTile->getPos())
				{
					findPath(pos, scatterTile->getPos());
				}
				else
				{
					move();
				}
			}
		}
		else
		{
			if (!pathToMoveTiles.size() || manhattan(pos, pacman->getPos()) > 750)
			{
				findPath(pos, pacman->getPos());
			}
			else
			{
				if (pathToMoveTiles[0]->getPos() == scatterTile->getPos())
				{
					findPath(pos, pacman->getPos());
				}
				else
				{
					move();
				}
			}

		}
	}
}
