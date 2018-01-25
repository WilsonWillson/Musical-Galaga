#pragma once
#include "Game/Entity.hpp"
#include <vector>
class Enemy;
class Player;


const float BULLET_SPEED = 450.f;

class Bullet : public Entity
{
public:
	Bullet();
	Bullet(Entity bulletStats, bool m_isPlayerBullet);
	Bullet(Entity bulletStats, bool isPlayerBullet, bool tracking, bool lazer);
	~Bullet();

	void Update(float deltaSeconds, std::vector<Enemy*> enemies, Player* thePlayer);
	void Render();

private:
	void FindNearestEnemy(std::vector<Enemy*> enemies);

public:
	Vector2		m_EnemyPosition;
	int			m_deathTime;
	bool		m_isPlayerBullet;
	bool		m_tracking;
	bool		m_Lazar;
	int			m_targetEnemy;

private:
	void RenderLazar();
	void RenderBullet();
};
