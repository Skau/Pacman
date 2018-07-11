#include "Enemy.h"
#include <iostream>
#include <time.h>
#include "Tile.h"
#include "Pacman.h"
#include "Game.h"
#include "Map.h"


Enemy::Enemy(sf::Image & image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Tile> scatterTileIn, std::weak_ptr<Map> mapIn, Game & game, bool isClydeIn)
	: Entity{ image, SpawnTile, mapIn, game }, isClyde{ isClydeIn }, canMove{ true }, isStarted{ false }, showPath{ false }
{
	srand((int)time(NULL));

	baseImage = image;

	if (!frightenedModeImage.loadFromFile("images/whiteImage.png"))
	{
		std::cout << "Failed to load pink image!" << std::endl;
		return;
	}

	std::shared_ptr<Tile> st = scatterTileIn.lock();
	if (st.get())
	{
		scatterTile = st;
	}
	pacman = game.getPacman();

	timeBetweenMovement = sf::Time(sf::milliseconds(100));
	maxFrightenedTime = sf::Time(sf::seconds(8));

	if (!isClyde)
		state = State::SCATTER;
	else
		state = State::CHASE;
	timeBetweenStates.push_back(sf::Time(sf::seconds(2000)));
	timeBetweenStates.push_back(sf::Time(sf::seconds(20)));
	timeBetweenStates.push_back(sf::Time(sf::seconds(7)));
	timeBetweenStates.push_back(sf::Time(sf::seconds(20)));
	timeBetweenStates.push_back(sf::Time(sf::seconds(7)));

	directions.push_back(Direction::UP);
	directions.push_back(Direction::DOWN);
	directions.push_back(Direction::LEFT);
	directions.push_back(Direction::RIGHT);
}

void Enemy::tick(float deltaTime)
{
	timeBetweenMovementElapsed += movementClock.restart();
	if (isStarted)
	{
		if (state == State::FRIGHTENED)
		{
			frightenedTimeElapsed += frightenedClock.restart();
			texture->loadFromImage(frightenedModeImage);
			if (frightenedTimeElapsed.asSeconds() >= 5)
			{
				if (isUsingFrightenedImage)
				{
					texture->loadFromImage(baseImage);
				}
				else
				{
					texture->loadFromImage(frightenedModeImage);
				}
				isUsingFrightenedImage = !isUsingFrightenedImage;
			}
			
			if (frightenedTimeElapsed >maxFrightenedTime)
			{
				std::cout << "Frightened mode done!\n";
				if (isUsingFrightenedImage)
					texture->loadFromImage(baseImage);
				timeBetweenMovement = sf::Time(sf::milliseconds(100));
				frightenedTimeElapsed = sf::Time::Zero;
				state = State::CHASE;
			}
		}
		else
		{
			if (pacman->getPos() == pos)
			{
				if (pathToMoveTiles.size())
					for (auto tile : pathToMoveTiles)
						tile->setImageOriginal();
				game->resetGame();
			}
			if (!isClyde)
			{
				if (timeBetweenStates.size())
				{
					if (stateClock.getElapsedTime() > timeBetweenStates.back())
					{
						if (state == State::CHASE)
							state = State::SCATTER;
						else
							state = State::CHASE;
						timeBetweenStates.pop_back();
						stateClock.restart();
					}
				}
			}
		}

		switch (state)
		{
		case State::CHASE:
			Chase();
			break;
		case State::SCATTER:
			Scatter();
			break;
		case State::FRIGHTENED:
			Frightened();
			break;
		default:
			break;
		}
	}
}

void Enemy::Scatter()
{
	if (pos == pacman->getPos()) { std::cout << "Game lost!\n"; game->resetGame(); }

	if (scatterTile.get())
	{
		if (!pathToMoveTiles.size())
		{
			findPath(pos, sf::Vector2f(208,240));
			foundPathToScatterTile = false;
		}

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
		std::cout << "No scattertile!\n";
	}
}

void Enemy::Frightened()
{
	if (pos == pacman->getPos()) setIsDead(true);

	if (!pathToMoveTiles.size())
	{
		findPath(pos, findRandomIntersection());
	}
	else
	{
		move();
	}
}

void Enemy::findPath(sf::Vector2f startLocation, sf::Vector2f endLocation)
{
	clearAndResetImageOnPathToMove();
	if (openTiles.size()) openTiles.clear();
	if (closedTiles.size()) closedTiles.clear();

	// Setup starting tile (current location)
	if (map->checkIfAdjacentTileIsInOfRange(startLocation, Direction::DEFAULT) && map->checkIfAdjacentTileIsInOfRange(endLocation, Direction::DEFAULT))
	{
		std::shared_ptr<Tile> startTile = map->getTileAtLocation(startLocation);
		std::shared_ptr<Tile> endTile = map->getTileAtLocation(endLocation);
		if (startTile.get() && endTile.get())
		{
			// calculate cost of starting tile
			startTile->setHCost(manhattan(startTile, endTile));
			startTile->setFCost(0 + startTile->getHCost());
			openTiles.push_back(startTile);

			// Loop while there are still tiles available to check
			while (openTiles.size() != 0)
			{
				// Get the tile with lowest F cost from open tiles
				std::shared_ptr<Tile> currentTile = openTiles[getMinfCost()];

				if (currentTile.get())
				{
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
					findAdjacentTiles(currentTile, endTile);
				}
			}
		}
	}
}

void Enemy::findAdjacentTiles(std::shared_ptr<Tile> currentTile, std::shared_ptr<Tile> endTile)
{
	std::vector<std::shared_ptr<Tile>> tilesToCheck;
	if (currentTile.get() && endTile.get())
	{
		if (adjacentTiles.size()) adjacentTiles.clear();

		for (Direction& direction : directions)
		{
			if (map->checkIfAdjacentTileIsInOfRange(currentTile->getPos(), direction))
			{
				std::shared_ptr<Tile> tileToCheck = map->getTileInDirectionFromLocation(currentTile->getPos(), direction);
				if (tileToCheck.get())
				{
					if (direction == Direction::LEFT || direction == Direction::RIGHT)
					{
						if (tileToCheck->getIsTeleporter())
						{
							for (std::shared_ptr<Tile> tile : map->getAllTiles())
							{
								if (tile.get())
								{
									if (tile->getIsTeleporter() && tile != tileToCheck)
									{
										tilesToCheck.push_back(tile);
									}
								}
							}
						}
						else
						{
							tilesToCheck.push_back(tileToCheck);
						}
					}
					else
					{
						tilesToCheck.push_back(tileToCheck);
					}
				}
			}
		}
	}
	
	for (std::shared_ptr<Tile> tile : tilesToCheck)
	{
		// Check to see if the neighbor exists
		if (tile.get())
		{
			if (tile->getPos() != bannedPos)
			{
				// Check to see if it's possible to move there
				if (tile->getIsWalkable())
				{
					if (tile->getPos() != pos)
					{
						// If it's not already on the closed list
						if (!findTileInVector(tile, closedTiles))
						{
							// If it's not already on the open list
							if (!findTileInVector(tile, openTiles))
							{
								// Set current tile to be parent and add it to the vector
								tile->setParentTile(currentTile);
								adjacentTiles.push_back(tile);
							}
							else
							{
								// Check to see if gCost is less if we use this square to get there.
								if (tile->getGCost() >
									(currentTile->getGCost() + (tile->getGCost() - tile->getParentTile()->getGCost())))
								{
									// Set current tile to be new parent
									tile->setParentTile(currentTile);
									// Re-calculate costs
									calculateCosts(tile, endTile);
								}
							}
						}
					}
				}
			}
		}
	}

	// Calculate costs and add tiles to openTiles
	for (std::shared_ptr<Tile> tile : adjacentTiles)
	{
		calculateCosts(tile, endTile);
		openTiles.push_back(tile);
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
	int v = 9999;
	if (startTile.get() && endTile.get())
	{
		float dx = abs(startTile->getPos().x - endTile->getPos().x);
		float dy = abs(startTile->getPos().y - endTile->getPos().y);
		v = (int)(10 * (dx + dy));
	}
	return v;
}

int Enemy::manhattan(sf::Vector2f startPos, sf::Vector2f endPos)
{
	float dx = abs(startPos.x - endPos.x);
	float dy = abs(startPos.y - endPos.y);
	return (int)(10 * (dx + dy));
}

// Generate the path to the end tile
void Enemy::generatePath(std::shared_ptr<Tile> finalTile)
{
	if (finalTile.get())
	{
		if (pathToMoveTiles.size()) pathToMoveTiles.clear();
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
						if(showPath)
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
	}

	move();
}

void Enemy::move()
{
	if (pathToMoveTiles.size() && timeBetweenMovementElapsed > timeBetweenMovement)
	{
		bannedPos = CurrentTile->getPos();
		if (pathToMoveTiles.back().get())
		{
			CurrentTile = pathToMoveTiles.back();
			pos = CurrentTile->getPos();
			sprite->setPosition(pos);
			colBox->setPosition(pos);
			pathToMoveTiles.back()->setImageOriginal();
			pathToMoveTiles.pop_back();
			timeBetweenMovementElapsed = sf::Time::Zero;
		}
	}
	else if (!pathToMoveTiles.size() && foundPathToScatterTile)
	{
		foundPathToScatterTile = false;
	}
}

sf::Vector2f Enemy::findRandomIntersection()
{
	std::vector<std::shared_ptr<Tile>> intersections;
	for (std::shared_ptr<Tile> tile : map->getAllTiles())
	{
		if (tile->getIsIntersection())
		{
			intersections.push_back(tile);
		}
	}

	int i = rand() % intersections.size();

	return intersections[i]->getPos();
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

void Enemy::clearAndResetImageOnPathToMove()
{
	for (std::shared_ptr<Tile> tile : pathToMoveTiles)
	{
		if(tile.get())
		tile->setImageOriginal();
	}
	pathToMoveTiles.clear();
}

void Enemy::toggleShowPath()
{
	if (showPath)
	{
		for (std::shared_ptr<Tile> tile : pathToMoveTiles)
		{
			tile->setImageOriginal();
		}
	}
	else
	{
		for (std::shared_ptr<Tile> tile : pathToMoveTiles)
		{
			tile->setImageGreen();
		}
	}
	showPath = !showPath;
}

void Enemy::triggerFrightenedMode()
{
	if (isStarted)
	{
		state = State::FRIGHTENED;
		timeBetweenMovement = sf::Time(sf::milliseconds(250));
		frightenedTimeElapsed = sf::Time::Zero;
		frightenedClock.restart();
	}
}

