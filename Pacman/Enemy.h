#pragma once
#include "Entity.h"
#include "Map.h"
class Pacman;
class Map;
class Tile;

enum class State
{
	CHASE,
	SCATTER,
	GHOST
};

class Enemy : public Entity
{
public:
	Enemy()=default;
	Enemy(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Tile> scatterTileIn, std::weak_ptr<Map> MapIn, Game& game);
	~Enemy()=default;

protected:
	void tick(float deltaTime);

	virtual void Chase()=0;
	virtual void childTick() = 0;

	void Scatter();

	void Ghost();

	void findPath(sf::Vector2f startLocation, sf::Vector2f endLocation);

	std::shared_ptr<Tile> EndTile;

	void findAdjacentTiles(std::shared_ptr<Tile> currentTile, std::shared_ptr<Tile> endTile);

	int getMinfCost();

	int manhattan(std::shared_ptr<Tile> startTile, std::shared_ptr<Tile> endTile);

	int manhattan(sf::Vector2f startPos, sf::Vector2f endPos);

	void generatePath(std::shared_ptr<Tile> finalTile);

	void move();

	sf::Vector2f findRandomIntersection();

	void calculateCosts(std::shared_ptr<Tile> TileToCalculate, std::shared_ptr<Tile> endTile);

	bool findTileInVector(std::shared_ptr<Tile> TileToCheck, std::vector<std::shared_ptr<Tile>>& VectorToCheck);

	void clearAndResetImageOnPathToMove();

	bool canMove;

	bool foundPathToScatterTile;

	State state;

	std::vector<sf::Time> timeBetweenStates;

	sf::Clock clock;

	sf::Vector2f bannedPos;

	std::shared_ptr<Pacman> pacman;

	std::shared_ptr<Map> map;

	std::shared_ptr<Tile> scatterTile;

	std::vector<std::shared_ptr<Tile>> openTiles;
	std::vector<std::shared_ptr<Tile>> closedTiles;
	std::vector<std::shared_ptr<Tile>> adjacentTiles;
	std::vector<std::shared_ptr<Tile>> pathToMoveTiles;
	std::vector<Direction> directions;
};

