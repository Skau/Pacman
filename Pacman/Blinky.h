#pragma once
#include "Enemy.h"

class Map;
class Pacman;

class Blinky : public Enemy
{
public:
	Blinky() = default;
	Blinky(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Map> MapIn, Game& game);
	~Blinky()=default;
};

