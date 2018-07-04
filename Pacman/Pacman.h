#pragma once
#include "Entity.h"

class Map;
class Tile;

class Pacman : public Entity 
{
public:
	Pacman(sf::Image& image);
	Pacman(sf::Image& image, std::shared_ptr<Tile> SpawnTile);
	~Pacman()=default;

	void init(std::shared_ptr<Tile>& CurrentTileIn) override;
	void handleEvent(sf::Event& event);
	void tick(float deltaTime) override;

	void setMap(std::shared_ptr<Map>& CurrentMap) { map = CurrentMap; }

	void move(float deltaTime);

	std::shared_ptr<Map> map;
	std::shared_ptr<Tile> CurrentTile;
	std::shared_ptr<Tile> LeftTeleporter;
	std::shared_ptr<Tile> RightTeleporter;

	bool isMovingUp;
	bool isMovingDown;
	bool isMovingLeft;
	bool isMovingRight;
};

