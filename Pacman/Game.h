#pragma once
class Pacman;
class Entity;
class Tile;
class ImageManager;
class Map;
class Game
{
public:
	Game()=default;
	~Game()=default;

	void init();
private:
	void loadImages();
	
	void beginPlay();
	void gameLoop();
	void handleEvents();
	void mainTick();
	void render();

	std::unique_ptr<sf::RenderWindow> window;

	std::shared_ptr<ImageManager> imageManager;
	
	std::shared_ptr<Map> map;

	std::shared_ptr<Pacman> pacman;

	std::vector<std::shared_ptr<Entity>> allEntities;

	sf::Clock frameClock;
	const sf::Time timePerFrame = sf::seconds(1.0f / 60.f);
	sf::Time timeSinceLastUpdate;
};

