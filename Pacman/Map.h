#pragma once
#include "Tile.h"

class ImageManager;

class Map
{
public:
	Map(std::shared_ptr<ImageManager>& im, Game& g);
	~Map()=default;

	void loadMap();

	void drawMap(sf::RenderWindow& window);

	std::shared_ptr<Tile> getPlayerSpawnTile() { return playerSpawnPoint; }
	std::shared_ptr<Tile> getEnemy1Spawnpoint() { return enemy1Spawnpoint; }
	std::shared_ptr<Tile> getEnemy2Spawnpoint() { return enemy2Spawnpoint; }
	std::shared_ptr<Tile> getEnemy3Spawnpoint() { return enemy3Spawnpoint; }
	std::shared_ptr<Tile> getEnemy4Spawnpoint() { return enemy4Spawnpoint; }

	std::vector<std::shared_ptr<Tile>>& getAllTiles() { return allTiles; }

	std::shared_ptr<Tile> getTileAtLocation(int x, int y);

	void sortTiles();

private:
	std::vector<std::shared_ptr<Tile>> allTiles;

	std::shared_ptr<Tile> playerSpawnPoint;
	std::shared_ptr<Tile> enemy1Spawnpoint;
	std::shared_ptr<Tile> enemy2Spawnpoint;
	std::shared_ptr<Tile> enemy3Spawnpoint;
	std::shared_ptr<Tile> enemy4Spawnpoint;

	std::shared_ptr<ImageManager> imageManager;

	Game* game;
};

