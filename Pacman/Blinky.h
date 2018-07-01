#pragma once
#include "Entity.h"
class Blinky : public Entity
{
public:
	Blinky();
	~Blinky();

	void init() override;
	void tick(float deltaTime) override;

	void move(float deltaTime);
};

