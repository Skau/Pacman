#pragma once
#include "Enemy.h"
class Inky :
	public Enemy
{
public:
	Inky()=default;
	Inky(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Tile> scatterTileIn, std::weak_ptr<Map> MapIn, Game& game);
	~Inky()=default;

private:
	void childTick() override;

	void Chase() override;

	Direction pacmanLastDirection;
};

