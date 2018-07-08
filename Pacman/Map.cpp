#include "Map.h"
#include <fstream>
#include <string>
#include <iostream>
#include "picojson.h"
#include "ImageManager.h"
#include "Game.h"

Map::Map(std::shared_ptr<ImageManager>& im, Game& g) : imageManager{ im }, game{ &g }
{
}

void Map::loadMap()
{
	std::ifstream levelData;
	picojson::value v;

	// Open and parse the JSON file
	levelData.open("map/testMap4.json");
	picojson::parse(v, levelData);

	// Total object of the file
	picojson::value::object& obj = v.get<picojson::object>();

	// Layers as value
	picojson::value& value = obj["layers"];
	if (value.is<picojson::value::array>())
	{
		//  Array object of layer
		picojson::value::array& object = value.get<picojson::value::array>();

		// Get value of "objects"
		picojson::value dataValue = object[1].get("objects");
		
		// Array object of "objects"
		picojson::value::array& objectArray = dataValue.get<picojson::value::array>();

		std::cout << "Started spawning tiles!\n";
		// Loop through "objects"
		for (int i = 0; i < objectArray.size(); ++i)
		{
			// Get value of element
			auto val = objectArray[i];
			// Get object of value
			picojson::value::object& o = val.get<picojson::object>();
			// Get x and y coordinates of object
			float x = (float)std::stoul(o["x"].to_str());
			float y = (float)std::stoul(o["y"].to_str());

			// Get the ID
			int id = std::stoul(o["id"].to_str());

			// Get the type
			std::string type = o["type"].to_str();

			// Check type and spawn tile with correct properties
			if (type == "teleporterTile")
				allTiles.push_back(std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f(x, y), *game, true, false, true, false, false, id)));
			else if (type == "intersectionTile")
				allTiles.push_back(std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(2), sf::Vector2f(x, y), *game, true, false, false, false, true, id)));
			else if (type == "collisionTile")
				allTiles.push_back(std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(0), sf::Vector2f(x, y), *game, false, false, false, true, false, id)));
			else if (type == "playerSpawnpoint")
			{
				std::shared_ptr<Tile> newTile = std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f(x, y), *game, true, true, false, false, false, id));
				allTiles.push_back(newTile);
				newTile->setPacmanIsHere(true);
				playerSpawnPoint = newTile;
			}
			else if (type == "enemySpawnTile1")
			{
				std::shared_ptr<Tile> newTile = std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f(x, y), *game, true, true, false, true, false, id));
				enemy1Spawnpoint = newTile;
				allTiles.push_back(newTile);
			}
			else if (type == "enemySpawnTile2")
			{
				std::shared_ptr<Tile> newTile = std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f(x, y), *game, true, true, false, true, false, id));
				enemy2Spawnpoint = newTile;
				allTiles.push_back(newTile);
			}
			else if (type == "enemySpawnTile3")
			{
				std::shared_ptr<Tile> newTile = std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f(x, y), *game, true, true, false, true, false, id));
				enemy3Spawnpoint = newTile;
				allTiles.push_back(newTile);
			}
			else if (type == "enemySpawnTile4")
			{
				std::shared_ptr<Tile> newTile = std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f(x, y), *game, true, true, false, true, false, id));
				enemy4Spawnpoint = newTile;
				allTiles.push_back(newTile);
			}
			else if (type == "enemy1ScatterTile")
			{
				std::shared_ptr<Tile> newTile = std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f(x, y), *game, true, false, false, false, false, id));
				enemy1ScatterTile = newTile;
				allTiles.push_back(newTile);
			}
			else if (type == "enemy2ScatterTile")
			{
				std::shared_ptr<Tile> newTile = std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f(x, y), *game, true, false, false, false, false, id));
				enemy2ScatterTile = newTile;
				allTiles.push_back(newTile);
			}
			else if (type == "enemy3ScatterTile")
			{
				std::shared_ptr<Tile> newTile = std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f(x, y), *game, true, false, false, false, false, id));
				enemy3ScatterTile = newTile;
				allTiles.push_back(newTile);
			}
			else if (type == "enemy4ScatterTile")
			{
				std::shared_ptr<Tile> newTile = std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f(x, y), *game, true, false, false, false, false, id));
				enemy4ScatterTile = newTile;
				allTiles.push_back(newTile);
			}
			else
				allTiles.push_back(std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f(x, y), *game, true, false, false, false, false, id)));
		}
		std::cout << "Spawning tiles DONE!\n";

		std::cout << "Setting tile pointers!\n";
		for (int i = 0; i < allTiles.size(); ++i)
		{
			sf::Vector2f pos = allTiles[i]->getPos();
			for (int j = 0; j < allTiles.size(); ++j)
			{
				if (allTiles[j] != allTiles[i])
				{
					if (allTiles[i]->getPos() == sf::Vector2f(allTiles[j]->getPos().x, allTiles[j]->getPos().y + 16))
					{
						allTiles[i]->setTileUp(allTiles[j]);
					}
					else if (allTiles[i]->getPos() == sf::Vector2f(allTiles[j]->getPos().x, allTiles[j]->getPos().y - 16))
					{
						allTiles[i]->setTileDown(allTiles[j]);
					}
					else if (allTiles[i]->getPos() == sf::Vector2f(allTiles[j]->getPos().x - 16, allTiles[j]->getPos().y))
					{
						allTiles[i]->setTileRight(allTiles[j]);
					}
					else if (allTiles[i]->getPos() == sf::Vector2f(allTiles[j]->getPos().x + 16, allTiles[j]->getPos().y))
					{
						allTiles[i]->setTileLeft(allTiles[j]);
					}
				}
			}
		}
		std::cout << "Setting tile pointers DONE!\n";
	}

	sortTiles();
}

void Map::drawMap(sf::RenderWindow& window)
{
	for (auto& tile : allTiles)
	{
		tile->Draw(window);
	}
}

std::shared_ptr<Tile> Map::getTileAtLocation(sf::Vector2f location)
{
	return allTiles[(int)((location.x / 16) * 36 + (location.y / 16))];
}

std::shared_ptr<Tile> Map::getTileInDirectionFromLocation(sf::Vector2f location, Direction dir)
{
	switch (dir)
	{
	case Direction::UP:
	{
		return allTiles[(int)((location.x/ 16) * 36 + ((location.y - 16) / 16))];
		break;
	}
	case Direction::DOWN:
	{
		return allTiles[(int)((location.x / 16) * 36 + ((location.y + 16) / 16))];
		break;
	}
	case Direction::LEFT:
	{
		return allTiles[(int)(((location.x - 16) / 16) * 36 + (location.y / 16))];
		break;
	}
	case Direction::RIGHT:
	{
		return allTiles[(int)(((location.x + 16) / 16) * 36 + (location.y / 16))];
		break;
	}
	default:
		std::cout << "getTileInDirectionFromLocation default ERROR!\n";
		return allTiles[0];
		break;
	}

	std::cout << "getTileInDirectionFromLocation ERROR!\n";
	return allTiles[0];
}

void Map::sortTiles()
{
	std::vector<std::shared_ptr<Tile>> tiles;
	for (int i = 0; i < 448; i+=16)
	{
		for (int j = 0; j < 576; j+=16)
		{
			for (auto& tile : allTiles)
			{
				if (tile->getPos().x == i && tile->getPos().y == j)
				{
					tiles.push_back(tile);
				}
			}
		}
	}
	allTiles = tiles;
}

