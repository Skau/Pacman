#pragma once
#include <SFML/Graphics.hpp>

class Tile;
class Game;

class Entity
{
public:
	Entity(sf::Image& image, std::weak_ptr<Tile> SpawnTile, Game& game);
	virtual ~Entity() = default;

	virtual void tick(float deltaTime)=0;
	void render(sf::RenderWindow& renderWindow);

	std::shared_ptr<Tile> getCurrentTile() { return CurrentTile; }
	void SetIsControllable(bool Value) { isControllable = Value; }
	
	sf::Sprite& GetSprite() const { return *sprite; }
	sf::RectangleShape& getColBox() { return *colBox; }

	void setPos(sf::Vector2f posIn);
	sf::Vector2f getPos() { return pos; }

protected:
	sf::Vector2f pos;
	sf::Vector2f vel;
	std::shared_ptr<Tile> CurrentTile;

	bool moving;
	bool isControllable;

	Game* game;

	std::unique_ptr<sf::Texture> texture;
	std::unique_ptr<sf::Sprite> sprite;
	std::unique_ptr<sf::RectangleShape> colBox;

	const int VELOCITY = 16;
};

