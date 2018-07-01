#pragma once
class Entity
{
public:
	Entity();
	~Entity() = default;

	void init();
	void handleEvent(sf::Event& event);
	void tick(float deltaTime);
	void render(sf::RenderWindow& renderWindow);
	void move(float deltaTime);

	sf::Sprite& GetSprite() const { return *sprite; }
	sf::Vector2f getPos() { return pos; }

	void setPosOfSprite(sf::Vector2f p) { pos = p; }
	void SetIsControllable(bool Value) { isControllable = Value; }

private:
	std::unique_ptr<sf::Texture> texture;
	std::unique_ptr<sf::Sprite> sprite;
	std::unique_ptr<sf::RectangleShape> colBox;

	const int VELOCITY = 500;

	sf::Vector2f pos;
	sf::Vector2f vel;
	bool isControllable;
};

