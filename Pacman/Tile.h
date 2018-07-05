#pragma once
#include <SFML/Graphics.hpp>
class Tile
{
public:
	Tile(sf::Image& dotImage, sf::Image& baseImage, sf::Vector2f Position, 
		bool Walkable, bool Spawnpoint, bool Teleporter, bool playerBlock, bool Intersection, int tileIDIn);
	~Tile();

	void Draw(sf::RenderWindow& window);

	sf::Vector2f getPos() { return pos; }

	int getTileID() { return tileID; }

	std::unique_ptr<sf::RectangleShape>& getColBox() { return colBox; }

	bool getIfSpawnpoint() { return isSpawnpoint; }

	bool getIsWalkable() { return isWalkable; }

	void ColorSprite();

	void setIsTeleporter(bool Value) { isTeleporter = Value; }
	bool getIsTeleporter() { return isTeleporter; }

	void setTileUp(std::shared_ptr<Tile> TileIn) { TileUp = TileIn; }
	void setTileLeft(std::shared_ptr<Tile> TileIn) { TileLeft = TileIn; }
	void setTileRight(std::shared_ptr<Tile> TileIn) { TileRight = TileIn; }
	void setTileDown(std::shared_ptr<Tile> TileIn) { TileDown = TileIn; }

	void setTeleporterTile(std::shared_ptr<Tile> TileIn) { TeleporterTile = TileIn; }

	std::shared_ptr<Tile> GetTileUp() { return TileUp; }
	std::shared_ptr<Tile> getTileLeft() { return TileLeft; }
	std::shared_ptr<Tile> getTileRight() { return TileRight; }
	std::shared_ptr<Tile> getTileDown() { return TileDown; }

	std::shared_ptr<Tile> getTeleporterTile() { return TeleporterTile; }

	void colorTileRed() { baseSprite->setColor(sf::Color::Red); }
	void colorTileGreen() { baseSprite->setColor(sf::Color::Green); }
	void removeColor() { baseSprite->setColor(sf::Color::Transparent); }

	bool getHasDot() { return hasDot; }

	bool getIsPlayerBlock() { return isPlayerBlock; }

	bool getIsIntersection() { return isIntersection; }

	void destroyDot();

	void setPacmanIsHere(bool value) { pacmanIsHere = value; }
	bool getPacmanIsHere() { return pacmanIsHere; }

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

	std::shared_ptr<Tile> TileUp;
	std::shared_ptr<Tile> TileLeft;
	std::shared_ptr<Tile> TileRight;
	std::shared_ptr<Tile> TileDown;
	std::shared_ptr<Tile> TeleporterTile;
	std::shared_ptr<Tile> ParentTile;

	int tileID;
	int gCost;
	int hCost;
	int fCost;

	bool pacmanIsHere;
	bool isSpawnpoint;
	bool isWalkable;
	bool isTeleporter;
	bool hasDot;
	bool isPlayerBlock;
	bool isIntersection;
};

