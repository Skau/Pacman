#include "Blinky.h"
#include <SFML/Graphics.hpp>
#include <iostream>
#include <memory>
#include <algorithm>
#include "Globals.h"
#include "Map.h"

Blinky::Blinky(sf::Image& image, std::shared_ptr<Tile>& SpawnTile, std::shared_ptr<Map>& MapIn) : Entity{ image, SpawnTile }, map{MapIn}
{
	std::cout << "Blinky pos: " << pos.x << ", " << pos.y << std::endl;
}

void Blinky::tick(float deltaTime)
{
	findPath();
}

void Blinky::findPath()
{
	std::cout << "Finding start tile\n";
	std::shared_ptr<Tile> startTile = CurrentTile;
	std::cout << "Finding end tile\n";
	std::shared_ptr<Tile> endTile = findEndTile();
	std::cout << "Found end tile (id: " << endTile->getTileID() << ")"<< std::endl;

	startTile->setHCost(manhattan(startTile));
	startTile->setFCost(0 + startTile->getHCost());
	std::cout << "Current tile F cost: " << startTile->getFCost() << std::endl;
	std::cout << "Adding CurrentTile to open tiles\n";
	openTiles.push_back(startTile);
	if (startTile.get())
	{
		startTile->colorTileGreen();
	}

	std::cout << "Starting while loop\n";
	while (openTiles.size() != 0)
	{
		std::cout << "Getting tile with lowest F cost\n";
		std::shared_ptr<Tile>& currentTile = openTiles[getMinfCost()];

		if (currentTile.get())
		{
			std::cout << "Tile with lowest is Tile: " << currentTile->getTileID() << std::endl;
		}
		else
		{
			std::cout << "NO CURRENT TILE\n";
		}

		std::cout << "Removing from openTiles\n";
		openTiles.erase(remove(openTiles.begin(), openTiles.end(), currentTile), openTiles.end());
		std::cout << "Adding to closedTiles\n";
		closedTiles.push_back(currentTile);

		if (currentTile)
		{
			startTile->colorTileRed();
		}
		std::cout << "Increment gCost\n";
		currentTile->setGCost(currentTile->getGCost() + 1);

		std::cout << "currentTile == endTile\n";
		if (currentTile->getPos() == endTile->getPos())
		{
			std::cout << "TRUE\n";
			break;
		}
		else
		{
			std::cout << "FALSE\n";
		}

		std::cout << "Finding neighboring tiles\n";
		findNeighbouringTiles(currentTile);

		std::cout << "Looping through neighboring tiles of size: " << neighboringTiles.size() << std::endl;;
		for (auto& tile : neighboringTiles)
		{
			std::cout << "Setting H cost of Tile" << tile->getTileID() << std::endl;
			tile->setHCost(manhattan(tile));
			std::cout << "Setting F cost of Tile\n";
			tile->setFCost(tile->getHCost() + tile->getGCost());
			std::cout << "F cost = " << tile->getFCost() << std::endl;
			std::cout << "Adding to openTiles\n";
			openTiles.push_back(tile);
			if (tile)
			{
				tile->colorTileGreen();
			}
		}
		std::cout << "Remvoing currentTile from openTiles\n";
		openTiles.erase(remove(openTiles.begin(), openTiles.end(), currentTile), openTiles.end());
		std::cout << "Adding currentTile to closedTiles\n";
		closedTiles.push_back(currentTile);
		if (currentTile)
		{
			currentTile->colorTileRed();
		}
	}
	std::cout << "Loop done, clearing closedTiles\n";
	for (auto& tile : closedTiles)
	{
		//tile->resetGCost();
		//tile->removeColor();
	}
	closedTiles.clear();
}

std::shared_ptr<Tile> Blinky::findEndTile()
{
	std::shared_ptr<Tile> endTile;
	for (auto& tile : map->getAllTiles())
	{
		if (tile->getPacmanIsHere())
		{
			endTile = tile;
		}
	}
	return endTile;
}

void Blinky::findNeighbouringTiles(std::shared_ptr<Tile> TileToCheck)
{
	if (neighboringTiles.size())
		neighboringTiles.clear();

	if (TileToCheck->GetTileUp())
	{
		if (TileToCheck->GetTileUp()->getIsWalkable() && !findElement(TileToCheck->GetTileUp()))
		{
			TileToCheck->GetTileUp()->setGCost(1 + TileToCheck->getGCost());
			neighboringTiles.push_back(TileToCheck->GetTileUp());
		}
	}
	if (TileToCheck->getTileDown())
	{
		if (TileToCheck->getTileDown()->getIsWalkable() && !findElement(TileToCheck->getTileDown()))
		{
			TileToCheck->getTileDown()->setGCost(1 + TileToCheck->getGCost());
			neighboringTiles.push_back(TileToCheck->getTileDown());
		}
	}
	if (TileToCheck->getTileRight())
	{
		if (TileToCheck->getTileRight()->getIsWalkable() && !findElement(TileToCheck->getTileRight()))
		{
			TileToCheck->getTileRight()->setGCost(1 + TileToCheck->getGCost());
			neighboringTiles.push_back(TileToCheck->getTileRight());
		}
	}
	if (TileToCheck->getTileLeft())
	{
		if (TileToCheck->getTileLeft()->getIsWalkable() && !findElement(TileToCheck->getTileLeft()))
		{
			TileToCheck->getTileLeft()->setGCost(1 + TileToCheck->getGCost());
			neighboringTiles.push_back(TileToCheck->getTileLeft());
		}
	}
}

int Blinky::getMinfCost()
{
	if (openTiles.size())
	{
		int minimumfCost = openTiles[0]->getFCost();
		int index = 0;
		for (unsigned int i = 0; i < openTiles.size() - 1; ++i)
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

int Blinky::manhattan(std::shared_ptr<Tile> TileToCheck)
{
	for (auto& tile : map->getAllTiles())
	{
		if (tile->getPacmanIsHere())
		{
			return (int)abs((int)(TileToCheck->getPos().x - tile->getPos().x) + abs(TileToCheck->getPos().y - tile->getPos().y));
		}
	}
	return 999;
}

void Blinky::generatePath()
{
	// TODO: ...
}

void Blinky::move(std::shared_ptr<Tile> TileToMoveTo)
{
	// TODO: ...
}

bool Blinky::findElement(std::shared_ptr<Tile> TileToCheck)
{
	std::vector<std::shared_ptr<Tile>>::iterator it;
	it = find(closedTiles.begin(), closedTiles.end(), TileToCheck);
	if (it != closedTiles.end())
	{
		std::cout << "Found element!\n";
		return true;
	}
	else
	{
		std::cout << "Did not find element!\n";
		return false;
	}

}
