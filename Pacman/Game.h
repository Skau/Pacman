#pragma once

class Entity;

class Game
{
public:
	Game() = default;
	~Game() = default;

	void init();
	void beginPlay();
	void gameLoop();
	void handleEvents();
	void mainTick();
	void render();

private:
	std::unique_ptr<sf::RenderWindow> window;
	sf::Clock frameClock;
	const sf::Time timePerFrame = sf::seconds(1.0f / 60.0f);
	sf::Time timeSinceLastUpdate;

	std::shared_ptr<Entity> entity;
};

