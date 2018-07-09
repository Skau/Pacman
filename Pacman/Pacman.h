#pragma once
#include "Entity.h"
#include "Map.h"
class Map;
class Tile;

class Pacman : public Entity 
{
public:
	Pacman() = default;
	Pacman(sf::Image& image, std::weak_ptr<Tile> SpawnTile, Game& game);
	~Pacman() = default;

	void handleEvent(sf::Event& event);
	void tick(float deltaTime) override;
	void setMap(std::shared_ptr<Map> CurrentMap) { map = CurrentMap; }

	Direction getMoveDirection() { return moveDirection; }

private:
	void move(float deltaTime);
	
	Direction lastDirection;
	Direction moveDirection;

	std::shared_ptr<Map> map;
	std::shared_ptr<Tile> LeftTeleporter;
	std::shared_ptr<Tile> RightTeleporter;

	bool isMoving;
};

