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

	const std::shared_ptr<Pacman> getPacman() { return pacman; }
	const std::shared_ptr<Blinky> getBlinky() { return blinky; }
	const std::shared_ptr<Pinky> getPinky() { return pinky; }
	const std::shared_ptr<Inky> getInky() { return inky; }
	const std::shared_ptr<Clyde> getClyde() { return clyde; }

	void triggerFrightenedMode();

	const int getDotsLeft() { return dotsLeft; }
	void incrementDotsLeft() { dotsLeft++; }
	void decrementDotsLeft() { dotsLeft--; }

private:
	void createTexts();
	void beginPlay();
	void createEnemies();

	void gameLoop();
	void handleEvents();
	void mainTick();
	void render();

	void toggleAIShowPath();

	void destroyEntity(std::shared_ptr<Entity>& entity);

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

