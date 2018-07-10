#pragma once
class Entity;
class Tile;
class ImageManager;
class Map;
class Pacman;
class Blinky;
class Pinky;
class Inky;
class Clyde;

class Game
{
public:
	Game();
	~Game()=default;

	void init();

	void resetGame();

	 std::shared_ptr<Pacman> getPacman() const { return pacman; }
	 std::shared_ptr<Blinky> getBlinky() const { return blinky; }
	 std::shared_ptr<Pinky> getPinky() const { return pinky; }
	 std::shared_ptr<Inky> getInky() const { return inky; }
	 std::shared_ptr<Clyde> getClyde() const { return clyde; }

	void triggerGhostMode();

	void killEnemy();

	int getDotsLeft() const { return dotsLeft; }
	void incrementDotsLeft() { dotsLeft++; }
	void decrementDotsLeft() { dotsLeft--; }

private:
	void loadImages();
	void createTexts();
	void beginPlay();
	void createEnemies();
	void gameLoop();
	void handleEvents();
	void mainTick();
	void render();

	void toggleAIShowPath();

	std::unique_ptr<sf::RenderWindow> window;

	std::shared_ptr<ImageManager> imageManager;
	
	std::shared_ptr<Map> map;

	std::shared_ptr<Pacman> pacman;
	std::shared_ptr<Blinky> blinky;
	std::shared_ptr<Pinky> pinky;
	std::shared_ptr<Inky> inky;
	std::shared_ptr<Clyde> clyde;

	bool inkyHasStartedMoving;
	bool clydeHasStartedMoving;

	int dotsLeft;

	std::vector<std::shared_ptr<Entity>> allEntities;

	sf::Clock frameClock;
	const sf::Time timePerFrame = sf::seconds(1.0f / 15.f);
	sf::Time timeSinceLastUpdate;

	bool gamePaused;

	std::unique_ptr<sf::Font> font;
	std::unique_ptr<sf::Text> pauseText;
	std::unique_ptr<sf::Text> showPathText;
	std::unique_ptr<sf::Text> resetText;
	std::unique_ptr<sf::Text> dotsText;
};

