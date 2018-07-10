#pragma once
#include "Entity.h"
#include "Map.h"

class Map;
class Tile;

class Pacman : public Entity 
{
public:
	Pacman() = default;
	Pacman(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Map> mapIn, Game& game);
	~Pacman() = default;

	void handleEvent(sf::Event& event);
	void tick(float deltaTime) override;

	Direction getMoveDirection() const { return moveDirection; }

private:
	void move(float deltaTime);
	
	Direction lastDirection;
	Direction moveDirection;

	std::shared_ptr<Tile> LeftTeleporter;
	std::shared_ptr<Tile> RightTeleporter;

	bool isMoving;
};

