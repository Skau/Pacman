#pragma once
#include <SFML/Graphics.hpp>

class Game;

class Tile
{
public:
	Tile(sf::Vector2f Position, Game& g, bool Walkable, bool Spawnpoint, bool Teleporter, bool playerBlock,
		bool Intersection, bool pelletIn, int tileIDIn);
	~Tile();

	void Draw(sf::RenderWindow& window);

	sf::Vector2f getPos() { return pos; }

	int getTileID() { return tileID; }

	std::unique_ptr<sf::RectangleShape>& getColBox() { return colBox; }

	bool getIfSpawnpoint() { return isSpawnpoint; }

	bool getIsWalkable() { return isWalkable; }

	void setImageOriginal();
	void setImageGreen();

	void setIsTeleporter(bool Value) { isTeleporter = Value; }
	bool getIsTeleporter() { return isTeleporter; }

	void setTeleporterTile(std::shared_ptr<Tile> TileIn) { TeleporterTile = TileIn; }

	std::shared_ptr<Tile> getTeleporterTile() { return TeleporterTile; }

	void setParentTile(std::shared_ptr<Tile> pTile) { ParentTile = pTile; }
	void resetParentTile() { ParentTile.reset(); }
	std::shared_ptr<Tile> getParentTile() { return ParentTile; }

	bool getHasDot() { return hasDot; }

	bool getHasPellet() { return hasPellet; }

	bool getIsPlayerBlock() { return isPlayerBlock; }

	bool getIsIntersection() { return isIntersection; }

	void destroyDot();

	void destroyPellet();

	void resetGCost() { gCost = 0; }
	void setGCost(int value) { gCost = value; }
	int getGCost() { return gCost; }

	void resetHCost() { hCost = 0; }
	void setHCost(int value) { hCost = value; }
	int getHCost() { return hCost; }

	void resetFCost() { fCost = 0; }
	void setFCost(int value) { fCost = value; }
	int getFCost() { return fCost; }

private:
	std::unique_ptr<sf::Sprite> baseSprite;
	std::unique_ptr<sf::Sprite> dotSprite;

	std::unique_ptr<sf::Texture> baseTexture;
	std::unique_ptr<sf::Texture> dotTexture;

	std::unique_ptr<sf::RectangleShape> colBox;

	sf::Vector2f pos;

	Game* game;

	std::shared_ptr<Tile> TeleporterTile;
	std::shared_ptr<Tile> ParentTile;

	sf::Image originalImage;
	sf::Image greenImage;
	sf::Image dotImage;
	sf::Image pelletImage;

	int tileID;
	int gCost;
	int hCost;
	int fCost;

	bool isSpawnpoint;
	bool isWalkable;
	bool isTeleporter;
	bool hasDot;
	bool isPlayerBlock;
	bool isIntersection;
	bool hasPellet;
};

