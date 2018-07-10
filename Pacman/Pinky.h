#pragma once
#include "Enemy.h"
class Pinky :
	public Enemy
{
public:
	Pinky()=default;
	Pinky(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Tile> scatterTileIn, std::weak_ptr<Map> MapIn, Game& game, bool isClydeIn);
	~Pinky()=default;

private:
	void Chase() override;

	Direction pacmanLastDirection;
};

