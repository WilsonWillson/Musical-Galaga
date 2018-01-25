#pragma once
#pragma once
#include "Game/Entity.hpp"
#include <vector>
class Enemy;


//const float BULLET_SPEED = 450.f;

class Pickups : public Entity
{
public:
	Pickups();
	Pickups(Entity bulletStats, Powerups powerup);
	~Pickups();


	void Update(float deltaSeconds);
	void Render();


public:
	Powerups m_typeOfPowerup;

};
