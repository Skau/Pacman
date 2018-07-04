#pragma once
#include "Entity.h"
class Dot : public Entity
{
public:
	Dot(sf::Image& image);
	Dot(sf::Image& image, std::shared_ptr<Tile> SpawnTile);
	~Dot();

	std::shared_ptr<Tile> CurrentTile;

	void init(std::shared_ptr<Tile>& CurrentTileIn) override;
	void tick(float deltaTime) override;
};

