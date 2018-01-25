#include "Game/Bullet.hpp"
#include "Game/Enemy.hpp"
#include "Game/Player.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Audio/Audio.hpp"
#include "Game/GameCommon.hpp"

//---------------------------Constructor/Deconstructor-----------------------------------------------

Bullet::Bullet()
{

}

Bullet::Bullet(Entity bulletStats, bool isPlayerBullet)
	: Entity(bulletStats),
	m_deathTime(110),
	m_isPlayerBullet(isPlayerBullet),
	m_targetEnemy(-999)
{
	m_tracking = false;
	m_tracking = false;
	m_Lazar = false;
}


Bullet::Bullet(Entity bulletStats, bool isPlayerBullet, bool tracking, bool lazer)
	: Entity(bulletStats),
	m_isPlayerBullet(isPlayerBullet),
	m_tracking(tracking),
	m_Lazar(lazer),
	m_targetEnemy(-999)
{
	if (lazer == true)
		m_deathTime = 50;
	else
		m_deathTime = 110;
}

Bullet::~Bullet()
{

}

//-----------------------------Update------------------------------------------------------------------
void Bullet::Update(float deltaSeconds, std::vector<Enemy*> enemies, Player* thePlayer)
{
	--m_deathTime;
	if (m_tracking == false)
	{
		m_position.y += (m_velocity.y * deltaSeconds);

		if (m_Lazar == false)
			m_position.x += (m_velocity.x * deltaSeconds);
		else
			m_position.x = thePlayer->m_position.x;
	}
	else
	{
		if(m_targetEnemy == -999)
			FindNearestEnemy(enemies);
		if (m_targetEnemy == 10000)
			return;

		Vector2 goalPosition = enemies[m_targetEnemy]->m_position;
		m_EnemyPosition = goalPosition;
		enemies[m_targetEnemy]->m_isBeingTracked = true;

		Vector2 displacementToTarget = goalPosition - m_position;
		Vector2 directionToTarget = displacementToTarget;
		directionToTarget.Normalize();

		m_orientationDegrees = displacementToTarget.CalcAngleDegrees();

		m_velocity.x = BULLET_SPEED * CosDegrees(m_orientationDegrees);
		m_velocity.y = BULLET_SPEED * SinDegrees(m_orientationDegrees);

		m_position.x += (m_velocity.x * deltaSeconds);
		m_position.y += (m_velocity.y * deltaSeconds);
	}
}

void Bullet::FindNearestEnemy(std::vector<Enemy*> enemies)
{
	float nearestEnemy = 10000;
	m_targetEnemy = nearestEnemy;
	for (unsigned int i = 0; i < enemies.size(); i++)
	{
		if (enemies[i] != nullptr && enemies[i]->m_isBeingTracked == false)
		{
			float tempDistance = CalcDistance(m_position, enemies[i]->m_position);

			if (tempDistance < nearestEnemy)
			{
				nearestEnemy = tempDistance;
				m_targetEnemy = i;
			}
		}
	}
}


//---------------------------Render-------------------------------------------------------------------
void Bullet::Render()
{
	if (m_Lazar == true)
		RenderLazar();
	else
		RenderBullet();
}


void Bullet::RenderLazar()
{
	if(m_deathTime > 45)
		g_renderSystem->DrawAABB2(AABB2(Vector2(0,0), Vector2(1600, 900)), Rgba(255, 255, 255, 255));

	g_renderSystem->DrawAABB2(AABB2(Vector2(m_position.x - 25 - GetRandomFloatInRange(3, 7) , 120 ), Vector2(m_position.x + 25 +GetRandomFloatInRange(3, 7), 900)), Rgba(255, 0, 0, 5.6  * m_deathTime));

	if(m_deathTime > 20)
		g_renderSystem->DrawAABB2(AABB2(Vector2(m_position.x - 10 - GetRandomFloatInRange(4, 8), 120), Vector2(m_position.x + 10 + GetRandomFloatInRange(4, 8), 900)), Rgba(255, 255, 255, 3.7  * m_deathTime));
}

void Bullet::RenderBullet()
{
	/* DEBUG TESTINg
	if (m_targetEnemy != -999)
	{
		g_renderSystem->DrawLine2D(m_position, m_EnemyPosition, Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));
	}
	*/


	g_renderSystem->PushMatrix();
	g_renderSystem->Translatef(m_position.x, m_position.y, 0.f);
	g_renderSystem->Rotatef(m_orientationDegrees, 0.f, 0.f, 1.f);
	g_renderSystem->Scalef(1.f, 1.f, 1.f);
	g_renderSystem->BeginPolygon();
	{
		//The Body of the ship
		g_renderSystem->Color3f(.5f, .5f, .5f);
		g_renderSystem->Vertex2f(0.65f * m_radius, 0.35f * m_radius);
		g_renderSystem->Vertex2f(-0.65f * m_radius, 0.35f * m_radius);

		g_renderSystem->Vertex2f(-0.65f * m_radius, 0.35f * m_radius);
		g_renderSystem->Vertex2f(-0.65f * m_radius, -0.35f * m_radius);

		g_renderSystem->Vertex2f(-0.65f * m_radius, -0.35f * m_radius);
		g_renderSystem->Vertex2f(0.65f * m_radius, -0.35f * m_radius);

		g_renderSystem->Vertex2f(0.65f * m_radius, -0.35f * m_radius);
		g_renderSystem->Vertex2f(0.65f * m_radius, 0.35f * m_radius);

		g_renderSystem->Color3f(.4f, .4f, .7f);

		//The Tip of the Rocket
		g_renderSystem->Vertex2f(0.65f * m_radius, -0.35f * m_radius);
		g_renderSystem->Vertex2f(1.f * m_radius, 0 * m_radius);

		g_renderSystem->Vertex2f(1.f * m_radius, 0 * m_radius);
		g_renderSystem->Vertex2f(0.65f * m_radius, 0.35f * m_radius);

		g_renderSystem->Color3f(1.f, 0.f, 0.f);
	}
	g_renderSystem->GEnd();
	g_renderSystem->BeginLines();
	{
		//The Flame 
		g_renderSystem->Vertex2f(-0.65f*m_radius, -0.3f*m_radius);
		g_renderSystem->Vertex2f(GetRandomFloatInRange((float).70, (float)1.05)*-m_radius, 0 * m_radius);

		g_renderSystem->Vertex2f((float)GetRandomFloatInRange((float).70, (float)1.05)*-m_radius, 0 * m_radius);
		g_renderSystem->Vertex2f(-0.65f*m_radius, 0.3f*m_radius);
	}
	g_renderSystem->GEnd();

	g_renderSystem->PopMatrix();
}
