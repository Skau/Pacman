#include "Tile.h"
#include <iostream>

Tile::Tile(sf::Image& dotImage, sf::Image& baseImage, sf::Vector2f Position, 
	bool Walkable, bool Spawnpoint, bool Teleporter, bool playerBlock, bool Intersection, int TileIDin) :
	isWalkable{ Walkable }, isSpawnpoint{ Spawnpoint }, isTeleporter{ Teleporter }, isPlayerBlock{ playerBlock }, isIntersection{ Intersection }, tileID {
	TileIDin
}
{ 
	baseTexture = std::unique_ptr<sf::Texture>(new sf::Texture);

	baseTexture->loadFromImage(baseImage);

	baseSprite = std::unique_ptr<sf::Sprite>(new sf::Sprite);
	baseSprite->setTexture(*baseTexture);

	baseSprite->setOrigin(8, 8);
	baseSprite->setPosition(Position);

	pos = Position;

	colBox = std::unique_ptr<sf::RectangleShape>(new sf::RectangleShape());
	colBox->setSize(sf::Vector2f(16, 16));
	colBox->setOrigin(sf::Vector2f(8, 8));
	colBox->setPosition(pos);
	colBox->setOutlineThickness(1.5f);
	colBox->setOutlineColor(sf::Color::Red);

	if (isWalkable && !isSpawnpoint && !Teleporter)
	{
		dotTexture = std::unique_ptr<sf::Texture>(new sf::Texture);
		dotTexture->loadFromImage(dotImage);

		dotSprite = std::unique_ptr<sf::Sprite>(new sf::Sprite);
		dotSprite->setTexture(*dotTexture);

		dotSprite->setOrigin(4, 4);
		dotSprite->setPosition(Position);
		
		hasDot = true;
	}
	else
	{
		hasDot = false;
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

void Tile::ColorSprite()
{
	baseSprite->setColor(sf::Color::Red);
}

void Tile::destroyDot()
{
	if (dotSprite)
	{
		std::cout << "Ate a dot!\n";
		dotSprite.release();
	}
}
