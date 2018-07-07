#pragma once
#include "Entity.h"

class Map;
class Pacman;

class Blinky : public Entity
{
public:
	Blinky() = default;
	Blinky(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Map> MapIn, Game& game);
	~Blinky()=default;

	void tick(float deltaTime) override;

	void findPath(std::shared_ptr<Tile> startTile, std::shared_ptr<Tile> endTile);

	std::shared_ptr<Tile> EndTile;

	void findNeighbouringTiles(std::shared_ptr<Tile> currentTile, std::shared_ptr<Tile> TileToCheck, std::shared_ptr<Tile> endTile);

	int getMinfCost();

	int manhattan(std::shared_ptr<Tile> startTile, std::shared_ptr<Tile> endTile);

	void generatePath(std::shared_ptr<Tile> finalTile);

	void move();

	void calculateCosts(std::shared_ptr<Tile> TileToCalculate, std::shared_ptr<Tile> endTile);

	bool findElementClosedTiles(std::shared_ptr<Tile> TileToCheck);
	bool findElementPathToMoveTiles(std::shared_ptr<Tile> TileToCheck);
	bool findElementOpenTiles(std::shared_ptr<Tile> TileToCheck);

	void clearPathToMoveTiles();

	bool canMove;

	std::shared_ptr<Pacman> pacman;

	std::shared_ptr<Map> map;
	
	std::vector<std::shared_ptr<Tile>> openTiles;
	std::vector<std::shared_ptr<Tile>> closedTiles;
	std::vector<std::shared_ptr<Tile>> neighboringTiles;
	std::vector<std::shared_ptr<Tile>> pathToMoveTiles;

};

