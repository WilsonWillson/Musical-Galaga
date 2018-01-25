#include "Game/Pickups.hpp"
#include "Game/GameCommon.hpp"

Pickups::Pickups()
{

}

Pickups::Pickups(Entity bulletStats, Powerups powerup)
	:Entity(bulletStats)
{
	m_typeOfPowerup = powerup;
}

Pickups::~Pickups()
{

}

void Pickups::Update(float deltaSeconds)
{
	m_position.x -= (m_velocity.x * deltaSeconds);
	m_position.y -= (m_velocity.y * deltaSeconds);
}

void Pickups::Render()
{
	if (m_typeOfPowerup == GUIDED_ROCKETS)
	{
		g_renderSystem->DrawAABB2(AABB2(m_position, m_radius, m_radius), Rgba(0, 255, 0, 255));
		g_renderSystem->DrawCenteredText2D(m_position, "Rockets", 20, 30, Rgba(255, 255, 255, 255), g_textSprites);
	}

	if (m_typeOfPowerup == LASER)
	{
		g_renderSystem->drawCircle(m_radius, Rgba(255, 0, 0, 255), m_position);
		g_renderSystem->DrawCenteredText2D(m_position, "Lazar", 20, 30, Rgba(255, 255, 255, 255), g_textSprites);
	}
}

