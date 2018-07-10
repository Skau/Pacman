#include "Blinky.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <algorithm>
#include "Globals.h"
#include "Map.h"
#include "Pacman.h"
#include "Game.h"
Blinky::Blinky(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Tile> scatterTileIn, std::weak_ptr<Map> MapIn, Game& game, bool isClydeIn)
	: Enemy{ image, SpawnTile, scatterTileIn, MapIn, game, isClydeIn }
{
	std::cout << "Blinky pos: " << pos.x << ", " << pos.y << std::endl;
}

void Blinky::Chase()
{
	if (pacman.get())
	{
		if (!pathToMoveTiles.size() || manhattan(pos, pacman->getPos()) > 750)
		{
			findPath(pos, pacman->getPos());
		}
		else
		{
			move();
		}
	}
}
