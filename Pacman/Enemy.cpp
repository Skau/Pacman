#include "Enemy.h"
#include <iostream>
#include <time.h>
#include <stdlib.h>
#include "Tile.h"
#include "Pacman.h"
#include "Game.h"
#include "Map.h"


Enemy::Enemy(sf::Image & image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Tile> scatterTileIn, std::weak_ptr<Map> MapIn, Game & game) 
	: Entity{ image, SpawnTile, game }, map { MapIn }
{
	srand((int)time(NULL));

	auto st = scatterTileIn.lock();
	if (st)
	{
		scatterTile = st;
	}
	lastDirection = Direction::DOWN;
	state = State::CHASE;
	pacman = game.getPacman();
	EndTile = pacman->getCurrentTile();
	canMove = true;
}

void Enemy::tick(float deltaTime)
{
	switch (state)
	{
	case State::CHASE:
		Chase();
		break;
	case State::SCATTER:
		Scatter();
		break;
	case State::GHOST:
		Ghost();
		break;
	default:
		break;
	}
}

void Enemy::Chase()
{
	std::shared_ptr<Tile> eTile = pacman->getCurrentTile();

	if (EndTile.get() && CurrentTile->getPos() != pacman->getPos())
	{
		if (eTile->getPos() != EndTile->getPos())
		{
			if (pathToMoveTiles.size())
			{
				if (750 > manhattan(pathToMoveTiles[0], eTile))
				{
					move();
				}
				else
				{
					EndTile = pacman->getCurrentTile();
					findPath(CurrentTile->getPos(), EndTile->getPos());
				}
			}
			else
			{
				EndTile = pacman->getCurrentTile();
				findPath(CurrentTile->getPos(), EndTile->getPos());
			}
		}
		else
		{
			if (pathToMoveTiles.size())
			{
				move();
			}
			else
			{
				EndTile = pacman->getCurrentTile();
				findPath(CurrentTile->getPos(), EndTile->getPos());
			}
		}
	}
	else
	{
		game->resetGame();
	}
}

void Enemy::Scatter()
{
	if (scatterTile)
	{
		if (pos != scatterTile->getPos())
		{
			if (!foundPathToScatterTile)
			{
				findPath(pos, scatterTile->getPos());
				foundPathToScatterTile = true;
			}
			else
			{
				move();
			}
		}
		else
		{
			state = State::CHASE;
		}
	}
	else
	{
		std::cout << "No scattertile!\n";
	}
}

void Enemy::Ghost()
{
}

void Enemy::findPath(sf::Vector2f startLocation, sf::Vector2f endLocation)
{
	if (openTiles.size()) openTiles.clear();
	if (closedTiles.size()) closedTiles.clear();

	// Setup starting tile (current location)
	auto startTile = map->getTileAtLocation(startLocation);
	auto endTile = map->getTileAtLocation(endLocation);
	startTile->setHCost(manhattan(startTile, endTile));
	startTile->setFCost(0 + startTile->getHCost());
	openTiles.push_back(startTile);

	while (openTiles.size() != 0)
	{
		// Get the tile with lowest F cost from open tiles
		std::shared_ptr<Tile> currentTile = openTiles[getMinfCost()];

		// remove it from open tiles and add it to close tiles
		openTiles.erase(std::remove(openTiles.begin(), openTiles.end(), currentTile), openTiles.end());
		closedTiles.push_back(currentTile);

		// Increment G cost
		currentTile->setGCost(currentTile->getGCost() + 1);

		// If this tile is the end tile, generate the path to get there
		if (currentTile->getPos() == endTile->getPos())
		{
			generatePath(currentTile);
			break;
		}

		// Find adjacent tiles
		if (adjacentTIles.size()) adjacentTIles.clear();

		findAdjacentTiles(currentTile, map->getTileInDirectionFromLocation(currentTile->getPos(), Direction::UP), endTile);
		findAdjacentTiles(currentTile, map->getTileInDirectionFromLocation(currentTile->getPos(), Direction::DOWN), endTile);
		findAdjacentTiles(currentTile, map->getTileInDirectionFromLocation(currentTile->getPos(), Direction::LEFT), endTile);
		findAdjacentTiles(currentTile, map->getTileInDirectionFromLocation(currentTile->getPos(), Direction::RIGHT), endTile);
		

		// Calculate the costs of valid adjacent tiles and add them to the open list
		for (int i = 0; i < adjacentTIles.size(); ++i)
		{
			calculateCosts(adjacentTIles[i], endTile);
			openTiles.push_back(adjacentTIles[i]);
		}
	}
}

void Enemy::findAdjacentTiles(std::shared_ptr<Tile> currentTile, std::shared_ptr<Tile> TileToCheck, std::shared_ptr<Tile> endTile)
{
	// Check to see if the neighbor exists
	if (TileToCheck)
	{
		//if (TileToCheck->getPos() != map->getTileInDirectionFromLocation(currentTile->getPos(), lastDirection)->getPos())
		//{
			// Check to see if it's possible to move there
		if (TileToCheck->getIsWalkable())
		{
			// If it's not already on the closed list
			if (!findTileInVector(TileToCheck, closedTiles))
			{
				// If it's not already on the open list
				if (!findTileInVector(TileToCheck, openTiles))
				{
					// Set current tile to be parent and add it to the vector
					TileToCheck->setParentTile(currentTile);
					adjacentTIles.push_back(TileToCheck);
				}
				else
				{
					// Check to see if gCost is less if we use this square to get there.
					if (TileToCheck->getGCost() >
						(currentTile->getGCost() + (TileToCheck->getGCost() - TileToCheck->getParentTile()->getGCost())))
					{
						// Set current tile to be new parent
						TileToCheck->setParentTile(currentTile);
						// Re-calculate costs
						calculateCosts(TileToCheck, endTile);
					}
				}
			}
		}
		//}
	}
}

// Generate index to lowest F cost in open tiles
int Enemy::getMinfCost()
{
	if (openTiles.size())
	{
		int minimumfCost = openTiles.back()->getFCost();
		int index = 0;
		for (unsigned int i = 0; i < openTiles.size(); ++i)
		{
			if (openTiles[i]->getFCost() < minimumfCost)
			{
				minimumfCost = openTiles[i]->getFCost();
				index = i;
			}
		}
		return index;
	}

	return 0;
}

// Heuristic
int Enemy::manhattan(std::shared_ptr<Tile> startTile, std::shared_ptr<Tile> endTile)
{
	float dx = abs(startTile->getPos().x - endTile->getPos().x);
	float dy = abs(startTile->getPos().y - endTile->getPos().y);
	return (int)(10 * (dx + dy));
}

// Generate the path to the end tile
void Enemy::generatePath(std::shared_ptr<Tile> finalTile)
{
	clearPathToMoveTiles();
	bool generatingPath = true;

	pathToMoveTiles.push_back(finalTile);

	while (generatingPath)
	{
		if (pathToMoveTiles.back().get())
		{
			if (pathToMoveTiles.back()->getParentTile().get())
			{
				if (!findTileInVector(pathToMoveTiles.back()->getParentTile(), pathToMoveTiles) && pathToMoveTiles.back()->getParentTile()->getPos() != CurrentTile->getPos())
				{
					pathToMoveTiles.push_back(pathToMoveTiles.back()->getParentTile());
					pathToMoveTiles.back()->setImageGreen();
				}
				else
				{
					generatingPath = false;
				}
			}
			else
			{
				generatingPath = false;
			}
		}
		else
		{
			generatingPath = false;
		}
	}

	move();
}

void Enemy::move()
{
	// Bool is there to half the movement speed
	if (pathToMoveTiles.size() && canMove)
	{
		lastDirection = calculateDirection(pathToMoveTiles.back());
		CurrentTile = pathToMoveTiles.back();
		pos = CurrentTile->getPos();
		sprite->setPosition(pos);
		colBox->setPosition(pos);
		pathToMoveTiles.back()->setImageOriginal();
		pathToMoveTiles.pop_back();
	}
	canMove = !canMove;
}

void Enemy::findPathToNextIntersection(sf::Vector2f startLocation)
{
	switch (lastDirection)
	{
	case Direction::UP:
		break;
	case Direction::DOWN:
		break;
	case Direction::LEFT:
		break;
	case Direction::RIGHT:
		break;
	case Direction::DEFAULT:
		break;
	default:
		break;
	}
}

// Set the G, H and F costs of a tile
void Enemy::calculateCosts(std::shared_ptr<Tile> TileToCalculate, std::shared_ptr<Tile> endTile)
{
	TileToCalculate->setGCost(TileToCalculate->getGCost() + TileToCalculate->getParentTile()->getGCost());
	TileToCalculate->setHCost(manhattan(TileToCalculate, endTile));
	TileToCalculate->setFCost(TileToCalculate->getHCost() + TileToCalculate->getGCost());
}

bool Enemy::findTileInVector(std::shared_ptr<Tile> TileToCheck, std::vector<std::shared_ptr<Tile>>& VectorToCheck)
{
	std::vector<std::shared_ptr<Tile>>::iterator it;
	it = find(VectorToCheck.begin(), VectorToCheck.end(), TileToCheck);
	if (it != VectorToCheck.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Enemy::clearPathToMoveTiles()
{
	for (auto tile : pathToMoveTiles)
	{
		tile->setImageOriginal();
	}
	pathToMoveTiles.clear();
}

Direction Enemy::calculateDirection(std::shared_ptr<Tile> nextTile)
{
	if (nextTile->getPos().x < CurrentTile->getPos().x) return Direction::LEFT;
	if (nextTile->getPos().x > CurrentTile->getPos().x) return Direction::RIGHT;
	if (nextTile->getPos().y < CurrentTile->getPos().y) return Direction::UP;
	if (nextTile->getPos().y > CurrentTile->getPos().y) return Direction::DOWN;
	return Direction::DEFAULT;
}

std::vector<Direction> Enemy::possibleDirections()
{
	std::vector<Direction> directions;
	switch (lastDirection)
	{
	case Direction::UP:
	{
		directions.push_back(Direction::DOWN);
		directions.push_back(Direction::LEFT);
		directions.push_back(Direction::RIGHT);
		break;
	}
	case Direction::DOWN:
	{
		directions.push_back(Direction::UP);
		directions.push_back(Direction::LEFT);
		directions.push_back(Direction::RIGHT);

		break;
	}
	case Direction::LEFT:
	{
		directions.push_back(Direction::UP);
		directions.push_back(Direction::DOWN);
		directions.push_back(Direction::RIGHT);

		break;
	}
	case Direction::RIGHT:
	{
		directions.push_back(Direction::UP);
		directions.push_back(Direction::DOWN);
		directions.push_back(Direction::LEFT);

		break;
	}
	case Direction::DEFAULT:
	{
		directions.push_back(Direction::UP);
		directions.push_back(Direction::DOWN);
		directions.push_back(Direction::RIGHT);
		directions.push_back(Direction::LEFT);
		break;
	}
	default:
		break;
	}
	return directions;
}
