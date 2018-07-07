#pragma once
class Pacman;
class Entity;
class Tile;
class ImageManager;
class Map;
class Game
{
public:
	Game();
	~Game()=default;

	void init();

	void resetGame();

	std::shared_ptr<Pacman> getPacman() { return pacman; }

	int dotsLeft;

private:
	void loadImages();
	void createTexts();
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
	const sf::Time timePerFrame = sf::seconds(1.0f / 15.f);
	sf::Time timeSinceLastUpdate;

	bool gamePaused;

	std::unique_ptr<sf::Font> font;
	std::unique_ptr<sf::Text> pauseText;
	std::unique_ptr<sf::Text> resetText;
	std::unique_ptr<sf::Text> dotsText;
};

