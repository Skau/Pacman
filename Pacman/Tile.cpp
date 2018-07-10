 #include "Tile.h"
#include <iostream>
#include "Game.h"

Tile::Tile(sf::Image& dotImage, sf::Image& baseImage, sf::Vector2f Position, Game& g,
	bool Walkable, bool Spawnpoint, bool Teleporter, bool playerBlock, bool Intersection, bool pelletIn, int TileIDin) :
	game{ &g }, isWalkable { Walkable}, isSpawnpoint{ Spawnpoint }, isTeleporter{ Teleporter }, isPlayerBlock{ playerBlock }, 
	isIntersection{ Intersection }, hasPellet{ pelletIn }, tileID{ TileIDin }, gCost{ 0 }, hasDot{ false }
{ 
	baseTexture = std::make_unique<sf::Texture>(sf::Texture());

	baseTexture->loadFromImage(baseImage);

	baseSprite = std::make_unique<sf::Sprite>(sf::Sprite());
	baseSprite->setTexture(*baseTexture);

	baseSprite->setPosition(Position);
	baseSprite->setOrigin(8, 8);

	OriginalImage = baseImage;

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
		dotTexture = std::make_unique<sf::Texture>(sf::Texture());
		dotTexture->loadFromImage(dotImage);

		dotSprite = std::make_unique<sf::Sprite>(sf::Sprite());
		dotSprite->setTexture(*dotTexture);

		dotSprite->setOrigin(7, 6);
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
	baseTexture->loadFromImage(OriginalImage);
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
