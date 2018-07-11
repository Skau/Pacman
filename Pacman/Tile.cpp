 #include "Tile.h"
#include <iostream>
#include "Game.h"

Tile::Tile(sf::Vector2f Position, Game& g,bool Walkable, bool Spawnpoint, bool Teleporter, bool playerBlock, 
	bool Intersection, bool pelletIn, int TileIDin) :
	game{ &g }, isWalkable { Walkable}, isSpawnpoint{ Spawnpoint }, isTeleporter{ Teleporter }, isPlayerBlock{ playerBlock }, 
	isIntersection{ Intersection }, hasPellet{ pelletIn }, tileID{ TileIDin }, gCost{ 0 }, hasDot{ false }
{ 
	if (isPlayerBlock && !isSpawnpoint)
	{
		if (!originalImage.loadFromFile("images/blackImage.png"))
		{
			std::cout << "Failed to load black image!" << std::endl;
			return;
		}
	}
	else
	{
		if (!originalImage.loadFromFile("images/blueImage.png"))
		{
			std::cout << "Failed to load blue image!" << std::endl;
			return;
		}
	}
	baseTexture = std::make_unique<sf::Texture>(sf::Texture());
	baseTexture->loadFromImage(originalImage);

	baseSprite = std::make_unique<sf::Sprite>(sf::Sprite());
	baseSprite->setTexture(*baseTexture);
	baseSprite->setPosition(Position);
	baseSprite->setOrigin(8, 8);

	if (!greenImage.loadFromFile("images/greenImage.png"))
	{
		std::cout << "Failed to load green image!" << std::endl;
	}

	pos = Position;
	colBox = std::make_unique<sf::RectangleShape>(sf::RectangleShape());
	colBox->setSize(sf::Vector2f(16, 16));
	colBox->setOrigin(sf::Vector2f(8, 8));
	colBox->setPosition(pos);
	colBox->setOutlineThickness(1.5f);
	colBox->setOutlineColor(sf::Color::Red);

	if (isWalkable && !isSpawnpoint && !Teleporter && !hasPellet)
	{
		if (!dotImage.loadFromFile("images/dotImage.png"))
		{
			std::cout << "Failed to load dot image!" << std::endl;
			return;
		}

		dotTexture = std::make_unique<sf::Texture>(sf::Texture());
		dotTexture->loadFromImage(dotImage);

		dotSprite = std::make_unique<sf::Sprite>(sf::Sprite());
		dotSprite->setTexture(*dotTexture);

		dotSprite->setOrigin(4, 4);
		dotSprite->setPosition(Position);
		
		hasDot = true;
		game->incrementDotsLeft();
	}

	if (hasPellet)
	{
		if (!pelletImage.loadFromFile("images/pelletImage.png"))
		{
			std::cout << "Failed to load pelletImage image!" << std::endl;
			return;
		}

		if(!dotTexture.get())
		dotTexture = std::make_unique<sf::Texture>(sf::Texture());
		dotTexture->loadFromImage(pelletImage);
		if(!dotSprite.get())
		dotSprite = std::make_unique<sf::Sprite>(sf::Sprite());
		dotSprite->setTexture(*dotTexture);

		dotSprite->setOrigin(4.5, 6);
		dotSprite->setPosition(Position);
	}
}

Tile::~Tile()
{
}

void Tile::Draw(sf::RenderWindow & window)
{
	window.draw(*baseSprite);
	if (dotSprite)
	window.draw(*dotSprite);
}

void Tile::setImageOriginal()
{
	baseTexture->loadFromImage(originalImage);
}

void Tile::setImageGreen()
{
	baseTexture->loadFromImage(greenImage);
}

void Tile::destroyDot()
{
	if (dotSprite)
	{
		if (game->getDotsLeft() > 1)
		{
			game->decrementDotsLeft();
		}
		else
		{
			std::cout << "Game won!\n";
			game->resetGame();
		}
		dotSprite.release();
	}
}

void Tile::destroyPellet()
{
	if (dotSprite)
	{
		dotSprite.release();
	}
}
