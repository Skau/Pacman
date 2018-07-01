#pragma once
#include "Entity.h"
class Dot : public Entity
{
public:
	Dot();
	~Dot();

	void init() override;
	void tick(float deltaTime) override;
};

