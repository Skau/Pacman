#include "Map.h"
#include <SFML/System/Time.hpp>
#include <fstream>
#include <string>
#include <sstream>
#include <iostream>
#include "picojson.h"
#include "ImageManager.h"

Map::Map(std::shared_ptr<ImageManager>& im) : w{28}, h{36}
{
	imageManager = im;
}


Map::~Map()
{
}

void Map::loadMap()
{
	std::ifstream levelData;
	picojson::value v;

	// Open and parse the JSON file
	levelData.open("Tileset/testMap4.json");
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

		// Loop through "objects"
		for (int i = 0; i < objectArray.size(); ++i)
		{
			// Get value of element
			auto val = objectArray[i];
			// Get object of value
			picojson::value::object& o = val.get<picojson::object>();
			// Get x and y coordinates of object
			int x = std::stoul(o["x"].to_str());
			int y = std::stoul(o["y"].to_str());

			// Get the ID
			int id = std::stoul(o["id"].to_str());

			//std::cout << "Creating Tile" << id << " at [" << x << "," << y << "]\n";

			// Get the type
			std::string type = o["type"].to_str();

			// Check type
			if (type != "Spawnpoint")
			{
				if (type == "teleporterTile")
				{
					allTiles.push_back(std::shared_ptr<Tile>(new Tile(
						imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f((float)x, (float)y), true, false, true, id)));
					std::cout << "Added teleporter tile with ID " << id << std::endl;
				}
				else if (type == "walkTile")
					allTiles.push_back(std::shared_ptr<Tile>(new Tile(
						imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f((float)x, (float)y), true, false, false, id)));
				else if (type == "intersectionTile")
					allTiles.push_back(std::shared_ptr<Tile>(new Tile(
						imageManager->getImage(4), imageManager->getImage(2), sf::Vector2f((float)x, (float)y), true, false, false, id)));
				else if (type == "collisionTile")
					allTiles.push_back(std::shared_ptr<Tile>(new Tile(
						imageManager->getImage(4), imageManager->getImage(0), sf::Vector2f((float)x, (float)y), false, false, false, id)));
			}
			else
			{
				std::shared_ptr<Tile> newTile = std::shared_ptr<Tile>(new Tile(
					imageManager->getImage(4), imageManager->getImage(1), sf::Vector2f((float)x, (float)y), true, true, false, id));
				allTiles.push_back(newTile);
				playerSpawnPoint = newTile;
			}
		}


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
}

void Map::drawMap(sf::RenderWindow& window)
{
	for (auto& tile : allTiles)
	{
		tile->Draw(window);
	}
}

sf::Vector2f Map::getPlayerSpawnpoint()
{
	sf::Vector2f pos = sf::Vector2f(0.f,0.f);
	for (auto& tile : allTiles)
	{
		if (tile->getIfSpawnpoint())
		{
			pos = tile->getPos();
		}
	}
	return pos;
}

