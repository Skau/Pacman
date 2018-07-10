#pragma once
#include "Enemy.h"
class Clyde :
	public Enemy
{
public:
	Clyde()=default;
	Clyde(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Tile> scatterTileIn, std::weak_ptr<Map> MapIn, Game& game, bool isClydeIn);
	~Clyde()=default;

private:
	void Chase() override;
};

