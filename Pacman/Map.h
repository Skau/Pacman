#pragma once
#include "Tile.h"
#include <tuple>
class ImageManager;

class Map
{
public:
	Map(std::shared_ptr<ImageManager>& im);
	~Map();

	void loadMap();

	void drawMap(sf::RenderWindow& window);

	std::shared_ptr<Tile>& GetSpawnTile() { return playerSpawnPoint; }

	sf::Vector2f getPlayerSpawnpoint();

	std::vector<std::shared_ptr<Tile>> getAllTiles() { return allTiles; }

private:
	std::vector<std::shared_ptr<Tile>> allTiles;

	std::shared_ptr<Tile> playerSpawnPoint;

	int w;
	int h;

	std::shared_ptr<ImageManager> imageManager;
};

