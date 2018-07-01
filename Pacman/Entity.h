#pragma once
#include <SFML/Graphics.hpp>
class Entity
{
public:
	Entity();
	~Entity();

	virtual void init()=0;
	virtual void tick(float deltaTime)=0;

	void render(sf::RenderWindow& renderWindow);

	void setPos(sf::Vector2f posIn);
	
	sf::Sprite& GetSprite() const { return *sprite; }
	sf::RectangleShape& getColBox() { return *colBox; }

	sf::Vector2f getPos() { return pos; }
	void SetIsControllable(bool Value) { isControllable = Value; }

	sf::Vector2f spawnpoint;

	bool getIsDestroyed() { return isDestroyed; }
	void setIsDestroyed(bool value) { isDestroyed = value; }

protected:
	sf::Vector2f pos;
	sf::Vector2f vel;
	bool isControllable;

	std::string imagePath;

	std::unique_ptr<sf::Texture> texture;
	std::unique_ptr<sf::Sprite> sprite;
	std::unique_ptr<sf::RectangleShape> colBox;

	const int VELOCITY = 250;

	bool isDestroyed = false;

};

