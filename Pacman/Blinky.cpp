#include "Blinky.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <algorithm>
#include "Globals.h"
#include "Map.h"
#include "Pacman.h"
#include "Game.h"
Blinky::Blinky(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Map> MapIn, Game& game)
	: Enemy{ image, SpawnTile, MapIn, game }
{
	std::cout << "Blinky pos: " << pos.x << ", " << pos.y << std::endl;
}
