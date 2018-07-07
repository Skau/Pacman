#include "Blinky.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <algorithm>
#include "Globals.h"
#include "Map.h"
#include "Pacman.h"
#include "Game.h"
Blinky::Blinky(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Map> MapIn, Game& game)
	: Entity{ image, SpawnTile, game }, map{MapIn}
{
	std::cout << "Blinky pos: " << pos.x << ", " << pos.y << std::endl;
	pacman = game.getPacman();
	EndTile = pacman->getCurrentTile();
	canMove = true;
}

void Blinky::tick(float deltaTime)
{
	if (openTiles.size()) openTiles.clear();
	if (closedTiles.size()) closedTiles.clear();

	std::shared_ptr<Tile> eTile = pacman->getCurrentTile();

	if (CurrentTile == eTile)
	{
		game->resetGame();
	}

	if (EndTile.get() && CurrentTile->getPos() != pacman->getPos())
	{
		if (eTile->getPos() != EndTile->getPos())
		{
			if (pathToMoveTiles.size())
			{
				if (manhattan(pathToMoveTiles[0], eTile) < 750)
				{
					move();
				}
				else
				{
					EndTile = pacman->getCurrentTile();
					findPath(CurrentTile, EndTile);
				}
			}
			else
			{
				EndTile = pacman->getCurrentTile();
				findPath(CurrentTile, EndTile);
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
				findPath(CurrentTile, EndTile);
			}
		}
	}
}

void Blinky::findPath(std::shared_ptr<Tile> startTile, std::shared_ptr<Tile> endTile)
{
	startTile->setHCost(manhattan(startTile, endTile));
	startTile->setFCost(0 + startTile->getHCost());
	openTiles.push_back(startTile);
	int i = 0;
	while (openTiles.size() != 0)
	{
		std::shared_ptr<Tile> currentTile = openTiles[getMinfCost()];

		openTiles.erase(std::remove(openTiles.begin(), openTiles.end(), currentTile), openTiles.end());

		closedTiles.push_back(currentTile);

		currentTile->setGCost(currentTile->getGCost() + 1);

		if (currentTile->getPos() == endTile->getPos())
		{
			generatePath(currentTile);
			break;
		}

		if (neighboringTiles.size()) neighboringTiles.clear();

		findNeighbouringTiles(currentTile, currentTile->GetTileUp(), endTile);
		findNeighbouringTiles(currentTile, currentTile->getTileDown(), endTile);
		findNeighbouringTiles(currentTile, currentTile->getTileLeft(), endTile);
		findNeighbouringTiles(currentTile, currentTile->getTileRight(), endTile);

		for (int i = 0; i < neighboringTiles.size(); ++i)
		{
			calculateCosts(neighboringTiles[i], endTile);
			openTiles.push_back(neighboringTiles[i]);
		}
	}
}

void Blinky::findNeighbouringTiles(std::shared_ptr<Tile> currentTile, std::shared_ptr<Tile> TileToCheck, std::shared_ptr<Tile> endTile)
{
	// Check to see if the neighbor exists
	if (TileToCheck)
	{
		// Check to see if it's possible to move there
		if (TileToCheck->getIsWalkable())
		{
			// If it's not already on the closed list
			if (!findElementClosedTiles(TileToCheck))
			{
				// If it's not already on the open list
				if (!findElementOpenTiles(TileToCheck))
				{
					// Set current tile to be parent and add it to the vector
					TileToCheck->setParentTile(currentTile);
					neighboringTiles.push_back(TileToCheck);
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
	}
}

int Blinky::getMinfCost()
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

int Blinky::manhattan(std::shared_ptr<Tile> startTile, std::shared_ptr<Tile> endTile)
{
	float dx = abs(startTile->getPos().x - endTile->getPos().x);
	float dy = abs(startTile->getPos().y - endTile->getPos().y);
	return (int)(10 * (dx + dy));
}

void Blinky::generatePath(std::shared_ptr<Tile> finalTile)
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
				if (!findElementPathToMoveTiles(pathToMoveTiles.back()->getParentTile()) && pathToMoveTiles.back()->getParentTile()->getPos() != CurrentTile->getPos())
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

void Blinky::move()
{
	if (pathToMoveTiles.size() && canMove)
	{
		CurrentTile = pathToMoveTiles.back();
		pos = CurrentTile->getPos();
		sprite->setPosition(pos);
		colBox->setPosition(pos);
		pathToMoveTiles.back()->setImageOriginal();
		pathToMoveTiles.pop_back();
	}
	canMove = !canMove;
}

void Blinky::calculateCosts(std::shared_ptr<Tile> TileToCalculate, std::shared_ptr<Tile> endTile)
{
	TileToCalculate->setGCost(TileToCalculate->getGCost() + TileToCalculate->getParentTile()->getGCost());
	TileToCalculate->setHCost(manhattan(TileToCalculate, endTile));
	TileToCalculate->setFCost(TileToCalculate->getHCost() + TileToCalculate->getGCost());
}

bool Blinky::findElementClosedTiles(std::shared_ptr<Tile> TileToCheck)
{
	std::vector<std::shared_ptr<Tile>>::iterator it;
	it = find(closedTiles.begin(), closedTiles.end(), TileToCheck);
	if (it != closedTiles.end())
	{
		return true;
	}
	else
	{
		return false;
	}

}

bool Blinky::findElementPathToMoveTiles(std::shared_ptr<Tile> TileToCheck)
{
	std::vector<std::shared_ptr<Tile>>::iterator it;
	it = find(pathToMoveTiles.begin(), pathToMoveTiles.end(), TileToCheck);
	if (it != pathToMoveTiles.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool Blinky::findElementOpenTiles(std::shared_ptr<Tile> TileToCheck)
{
	std::vector<std::shared_ptr<Tile>>::iterator it;
	it = find(openTiles.begin(), openTiles.end(), TileToCheck);
	if (it != openTiles.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Blinky::clearPathToMoveTiles()
{
	for (auto tile : pathToMoveTiles)
	{
		tile->setImageOriginal();
	}
	pathToMoveTiles.clear();
}
