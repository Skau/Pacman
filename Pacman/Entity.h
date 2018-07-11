#pragma once
#include <SFML/Graphics.hpp>

class Tile;
class Game;
class Map;

class Entity
{
public:
	Entity(sf::Image& image, std::weak_ptr<Tile> SpawnTile, std::weak_ptr<Map> mapIn, Game& game);
	virtual ~Entity() = default;

	virtual void tick(float deltaTime)=0;
	void render(sf::RenderWindow& renderWindow);

	const std::shared_ptr<Tile> getCurrentTile() { return CurrentTile; }
	void SetIsControllable(bool Value) { isControllable = Value; }
	
	const sf::Sprite& GetSprite() { return *sprite; }
	sf::RectangleShape& getColBox() { return *colBox; }

	const sf::Vector2f getPos() { return pos; }
	void setPos(sf::Vector2f posIn);

	const bool getIsDead() { return isDead; }
	void setIsDead(bool value ) { isDead = value; }

	void destroyEntity();
protected:
	sf::Vector2f pos;
	sf::Vector2f vel;
	std::shared_ptr<Tile> CurrentTile;
	std::shared_ptr<Map> map;
	bool moving;
	bool isControllable;
	bool isDead;
	Game* game;

	std::unique_ptr<sf::Texture> texture;
	std::unique_ptr<sf::Sprite> sprite;
	std::unique_ptr<sf::RectangleShape> colBox;

	const int VELOCITY = 16;
};

