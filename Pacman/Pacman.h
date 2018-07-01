#pragma once
#include "Entity.h"
class Pacman : public Entity 
{
public:
	Pacman();
	~Pacman();

	void init() override;
	void handleEvent(sf::Event& event);
	void tick(float deltaTime) override;

	//bool intersects();

	//void render(sf::RenderWindow& renderWindow) override;

	void move(float deltaTime);
};

