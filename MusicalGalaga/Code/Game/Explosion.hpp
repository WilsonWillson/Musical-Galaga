#pragma once
#include "Engine/Math/Vector2.hpp"

const float DEBRIES_GRAVITY = 9800;
class Explosion
{
public:
	Explosion(Vector2 position, float radius, float lifetime);
	Explosion(Vector2 position, float radius, float lifetime, Vector2 debriesPosition);
	~Explosion();


	void Update(float deltaseconds);
	void Render();


public:
	Vector2 m_position;
	float m_radius;
	float m_lifetime;
	float m_debriesDrift;
	Vector2 m_DebriesPosition;
	bool explosionDone;
};