#pragma once
class Pacman;
class Blinky;
class Pinky;
class Inky;
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
	std::shared_ptr<Blinky> getBlinky() { return blinky; }
	std::shared_ptr<Pinky> getPinky() { return pinky; }
	std::shared_ptr<Inky> getInky() { return inky; }

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
	std::shared_ptr<Blinky> blinky;
	std::shared_ptr<Pinky> pinky;
	std::shared_ptr<Inky> inky;

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

