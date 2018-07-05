#pragma once
#include "Entity.h"

class Map;

class Blinky : public Entity
{
public:
	Blinky() = default;
	Blinky(sf::Image& image, std::shared_ptr<Tile>& SpawnTile, std::shared_ptr<Map>& MapIn);
	~Blinky()=default;

	void tick(float deltaTime) override;

	void findPath();

	std::shared_ptr<Tile> endTile;

	std::shared_ptr<Tile> findEndTile();

	void findNeighbouringTiles(std::shared_ptr<Tile> TileToCheck);

	int getMinfCost();

	int manhattan(std::shared_ptr<Tile> TileToCheck);

	void generatePath();

	void move(std::shared_ptr<Tile> TileToMoveTo);

	bool findElement(std::shared_ptr<Tile> TileToCheck);

	std::shared_ptr<Map> map;
	
	std::vector<std::shared_ptr<Tile>> openTiles;
	std::vector<std::shared_ptr<Tile>> closedTiles;
	std::vector<std::shared_ptr<Tile>> neighboringTiles;
	std::vector<std::shared_ptr<Tile>> pathToMoveTiles;
};

