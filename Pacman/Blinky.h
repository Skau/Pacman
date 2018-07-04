#pragma once
#include "Entity.h"
class Blinky : public Entity
{
public:
	Blinky(sf::Image& image);
	~Blinky();

	void init(std::shared_ptr<Tile>& CurrentTileIn) override;
	void tick(float deltaTime) override;

	void move(float deltaTime);
};

