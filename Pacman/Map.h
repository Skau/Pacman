#pragma once
#include "Tile.h"

class ImageManager;

enum class Direction
{
	UP,
	DOWN,
	LEFT,
	RIGHT,
	DEFAULT
};

class Map
{
public:
	Map(std::shared_ptr<ImageManager>& im, Game& g);
	~Map()=default;

	void loadMap();

	void drawMap(sf::RenderWindow& window);

	bool checkIfAdjacentTileIsInOfRange(sf::Vector2f currentLoc, Direction dir);

	std::shared_ptr<Tile> getPlayerSpawnTile() { return playerSpawnPoint; }
	std::shared_ptr<Tile> getEnemy1Spawnpoint() { return enemy1Spawnpoint; }
	std::shared_ptr<Tile> getEnemy2Spawnpoint() { return enemy2Spawnpoint; }
	std::shared_ptr<Tile> getEnemy3Spawnpoint() { return enemy3Spawnpoint; }
	std::shared_ptr<Tile> getEnemy4Spawnpoint() { return enemy4Spawnpoint; }

	std::shared_ptr<Tile> getEnemy1ScatterTile() { return enemy1ScatterTile; }
	std::shared_ptr<Tile> getEnemy2ScatterTile() { return enemy2ScatterTile; }
	std::shared_ptr<Tile> getEnemy3ScatterTile() { return enemy3ScatterTile; }
	std::shared_ptr<Tile> getEnemy4ScatterTile() { return enemy4ScatterTile; }

	std::vector<std::shared_ptr<Tile>> getAllTiles() { return allTiles; }

	std::shared_ptr<Tile> getTileAtLocation(sf::Vector2f location);
	std::shared_ptr<Tile> getTileInDirectionFromLocation(sf::Vector2f location, Direction dir);

	void sortTiles();

private:
	std::vector<std::shared_ptr<Tile>> allTiles;

	std::shared_ptr<Tile> playerSpawnPoint;
	std::shared_ptr<Tile> enemy1Spawnpoint;
	std::shared_ptr<Tile> enemy2Spawnpoint;
	std::shared_ptr<Tile> enemy3Spawnpoint;
	std::shared_ptr<Tile> enemy4Spawnpoint;

	std::shared_ptr<Tile> enemy1ScatterTile;
	std::shared_ptr<Tile> enemy2ScatterTile;
	std::shared_ptr<Tile> enemy3ScatterTile;
	std::shared_ptr<Tile> enemy4ScatterTile;

	std::shared_ptr<ImageManager> imageManager;

	Game* game;
};

