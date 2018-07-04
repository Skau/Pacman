#pragma once
#include <SFML/Graphics.hpp>

class Tile;

class Entity
{
public:
	Entity(sf::Image& image);
	virtual ~Entity() = default;

	virtual void init(std::shared_ptr<Tile>& CurrentTileIn)=0;
	virtual void tick(float deltaTime)=0;

	void render(sf::RenderWindow& renderWindow);

	void setPos(sf::Vector2f posIn);
	
	sf::Sprite& GetSprite() const { return *sprite; }
	sf::RectangleShape& getColBox() { return *colBox; }

	sf::Vector2f getPos() { return pos; }
	void SetIsControllable(bool Value) { isControllable = Value; }

	void setSpawnpoint(sf::Vector2f sp) { spawnpoint = sp; }
	sf::Vector2f getSpawnpooint() { return spawnpoint; }

	bool getIsDestroyed() { return isDestroyed; }
	void setIsDestroyed(bool value) { isDestroyed = value; }

protected:
	sf::Vector2f pos;
	sf::Vector2f vel;
	sf::Vector2f spawnpoint;

	bool moving;

	bool isControllable;

	std::string imagePath;

	std::unique_ptr<sf::Texture> texture;
	std::unique_ptr<sf::Sprite> sprite;
	std::unique_ptr<sf::RectangleShape> colBox;

	const int VELOCITY = 16;

	bool isDestroyed = false;

};

