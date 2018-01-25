#include "Game/Enemy.hpp"
#include "Engine/Audio/Audio.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/GameCommon.hpp"

//-----------------------Constructor/Deconstructor---------------------------------
Enemy::Enemy()
	:Entity()
{

}

Enemy::Enemy(Entity enemyStats, Rgba asteroidColor)
	: Entity(enemyStats),
	m_AsteroidColor(asteroidColor)
{
	Vector2 asteroidCoordinate;
	const float DEGREES_PER_VERTEX = 360.f / (float)ASTEROID_NUM_SIZE;
	{
		for (int vertexNum = 0; vertexNum < ASTEROID_NUM_SIZE; ++vertexNum)
		{
			float varyingRadius = GetRandomFloatInRange(0.75f, 1.1f);
			float degrees = DEGREES_PER_VERTEX * (float)vertexNum;
			degrees += m_orientationDegrees;
			asteroidCoordinate.x = varyingRadius * CosDegrees(degrees);
			asteroidCoordinate.y = varyingRadius * SinDegrees(degrees);
			m_vertexes[vertexNum] = asteroidCoordinate;
		}
	}
	m_TimeSinceLastShot = 0;
	m_StartingLocation = m_position;
	m_RenderVarianceNegative = true;
	m_isBeingTracked = false;
}

Enemy::Enemy(Entity enemyStats)
	: Entity(enemyStats)
{
	Vector2 asteroidCoordinate;
	const float DEGREES_PER_VERTEX = 360.f / (float)ASTEROID_NUM_SIZE;
	{
		for (int vertexNum = 0; vertexNum < ASTEROID_NUM_SIZE; ++vertexNum)
		{
			float varyingRadius = GetRandomFloatInRange(0.75f, 1.1f);
			float degrees = DEGREES_PER_VERTEX * (float)vertexNum;
			degrees += m_orientationDegrees;
			asteroidCoordinate.x = varyingRadius * CosDegrees(degrees);
			asteroidCoordinate.y = varyingRadius * SinDegrees(degrees);
			m_vertexes[vertexNum] = asteroidCoordinate;
		}
	}

	m_StartingLocation = m_position;

	m_TimeSinceLastShot = 0;
	m_AsteroidColor = Rgba(1,1, 1, 1);
	m_RenderVarianceNegative = true;
	m_isBeingTracked = false;
}

Enemy::~Enemy()
{

}

//--------------------------Update-----------------------------------------------------
void Enemy::Update(float deltaSeconds)
{
	m_TimeSinceLastShot -= deltaSeconds;

	if (m_velocity.x == 0)
	{
		m_velocity.x += 50;
	}
	
	if (m_velocity.x < 10 && m_velocity.x > 0)
	{
		m_velocity.x -= 100;
	}

	if (m_velocity.x > -10 && m_velocity.x < 0)
	{
		m_velocity.x += 100;
	}


	float averageOfPosition = DetermineHeightFromSong();

	m_position.x += (m_velocity.x * deltaSeconds);
	m_position.y += (m_velocity.y * deltaSeconds);

	m_velocity *= .995f;
	DetermineWrapping();
}

float Enemy::DetermineHeightFromSong()
{
	float averageOfPosition = 0;

	if (m_startingXColumn == 0)
	{
		averageOfPosition = g_AudioSystem->m_averageBassVolumeLastFrame[0];
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_0_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 50 * m_startingYRow);
	}
	if (m_startingXColumn == 1)
	{
		averageOfPosition = g_AudioSystem->m_averageBassVolumeLastFrame[1];
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_1_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 50 * m_startingYRow);
	}
	if (m_startingXColumn == 2)
	{
		averageOfPosition = g_AudioSystem->m_averageBassVolumeLastFrame[2];
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_2_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 50 * m_startingYRow);
	}

	if (m_startingXColumn == 3)
	{
		averageOfPosition = g_AudioSystem->m_averageMediumHighVolumeLastFrame[0] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[1] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[2] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[3];
		averageOfPosition = averageOfPosition / 4;
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_3_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 50 * m_startingYRow);
	}

	if (m_startingXColumn == 4)
	{
		averageOfPosition = g_AudioSystem->m_averageMediumHighVolumeLastFrame[5] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[6] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[7] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[4];
		averageOfPosition = averageOfPosition / 4;
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_4_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 50 * m_startingYRow);
	}
	if (m_startingXColumn == 5)
	{
		averageOfPosition = g_AudioSystem->m_averageMediumHighVolumeLastFrame[10] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[11] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[12] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[8] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[9];
		averageOfPosition = averageOfPosition / 5;
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_5_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 50 * m_startingYRow);
	}
	if (m_startingXColumn == 6)
	{
		averageOfPosition = g_AudioSystem->m_averageMediumHighVolumeLastFrame[15] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[16] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[17] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[13] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[14];
		averageOfPosition = averageOfPosition / 5;
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_6_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 50 * m_startingYRow);
	}
	if (m_startingXColumn == 7)
	{
		averageOfPosition = g_AudioSystem->m_averageMediumHighVolumeLastFrame[20] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[21] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[22] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[19] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[18];
		averageOfPosition = averageOfPosition / 5;
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_7_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 50 * m_startingYRow);
	}
	if (m_startingXColumn == 8)
	{
		averageOfPosition = g_AudioSystem->m_averageMediumHighVolumeLastFrame[25] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[26] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[23] + g_AudioSystem->m_averageMediumHighVolumeLastFrame[24];
		averageOfPosition = averageOfPosition / 5;
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_8_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 70 * m_startingYRow);
	}
	if (m_startingXColumn == 9)
	{
		averageOfPosition = g_AudioSystem->m_AverageHighVolumeLastFrame[0] + g_AudioSystem->m_AverageHighVolumeLastFrame[1] + g_AudioSystem->m_AverageHighVolumeLastFrame[2] + g_AudioSystem->m_AverageHighVolumeLastFrame[3] + g_AudioSystem->m_AverageHighVolumeLastFrame[4];
		averageOfPosition = averageOfPosition / 5;
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_9_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 50 * m_startingYRow);
	}
	if (m_startingXColumn == 10)
	{
		averageOfPosition = g_AudioSystem->m_AverageHighVolumeLastFrame[5] + g_AudioSystem->m_AverageHighVolumeLastFrame[6] + g_AudioSystem->m_AverageHighVolumeLastFrame[7] + g_AudioSystem->m_AverageHighVolumeLastFrame[8] + g_AudioSystem->m_AverageHighVolumeLastFrame[9];
		averageOfPosition = averageOfPosition / 5;
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_10_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 50 * m_startingYRow);
	}
	if (m_startingXColumn == 11)
	{
		averageOfPosition = g_AudioSystem->m_AverageHighVolumeLastFrame[10] + g_AudioSystem->m_AverageHighVolumeLastFrame[11] + g_AudioSystem->m_AverageHighVolumeLastFrame[12] + g_AudioSystem->m_AverageHighVolumeLastFrame[13] + g_AudioSystem->m_AverageHighVolumeLastFrame[14];
		averageOfPosition = averageOfPosition / 4;
		m_position.y = RangeMapFloat(averageOfPosition, 0, COLUMN_11_MAX_HEIGHT, 525 + 50 * m_startingYRow, 575 + 50 * m_startingYRow);
	}

	return averageOfPosition;
}

bool Enemy::ShouldIFire()
{
	if (m_TimeSinceLastShot >= 0 && m_TimeSinceLastShot < ENEMY_WAIT_TO_OVERRIDE_BASS_SHOOT_TIME && m_startingXColumn < 3)
	{
		if (m_startingXColumn == 0 || m_startingXColumn == 1 || m_startingXColumn == 2)
			return CheckBassNotesForOverRideFire();
	}

	if (m_TimeSinceLastShot >= 0 && m_TimeSinceLastShot < ENEMY_WAIT_TO_OVERRIDE_MID_SHOOT_TIME && m_startingXColumn > 2 && m_startingXColumn < 9)
	{
		if (m_startingXColumn == 3 || m_startingXColumn == 4 || m_startingXColumn == 5 || m_startingXColumn == 6 || m_startingXColumn == 7 || m_startingXColumn == 8)
			return CheckMidNotesForOverRideFire();
	}

	if (m_TimeSinceLastShot >= 0 && m_TimeSinceLastShot < ENEMY_WAIT_TO_OVERRIDE_HIGH_SHOOT_TIME && m_startingXColumn > 9)
	{
		if (m_startingXColumn == 9 || m_startingXColumn == 10 || m_startingXColumn == 11)
			return CheckHighNotesForOverRideFire();
	}

	if(m_TimeSinceLastShot <= 0)
	{
		if (m_startingXColumn == 0 || m_startingXColumn == 1 || m_startingXColumn == 2)
			return CheckBassNotesForFire();
		if (m_startingXColumn == 3 || m_startingXColumn == 4 || m_startingXColumn == 5 || m_startingXColumn == 6 || m_startingXColumn == 7 || m_startingXColumn == 8)
			return CheckMidNotesForFire();
		if (m_startingXColumn == 9 || m_startingXColumn == 10 || m_startingXColumn == 11)
			return CheckHighNotesForFire();
	}

	return false;
}

bool Enemy::CheckBassNotesForFire()
{
	if (m_startingXColumn == 0)
	{
		if (g_AudioSystem->m_averageBassVolumeLastFrame[0] > COLUMN_0_MIN_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 1)
	{
		if (g_AudioSystem->m_averageBassVolumeLastFrame[1] > COLUMN_1_MIN_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 2)
	{
		if (g_AudioSystem->m_averageBassVolumeLastFrame[2] >  COLUMN_2_MIN_VALUE_TO_SHOOT)
			return true;
	}

	return false;
}


bool Enemy::CheckMidNotesForFire()
{
	if (m_startingXColumn == 3)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[0] > COLUMN_3_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[1] > COLUMN_3_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[2] > COLUMN_3_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[2] > COLUMN_3_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[3] > COLUMN_3_MIN_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 4)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[5] > COLUMN_4_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[6] > COLUMN_4_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[7] > COLUMN_4_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[4] > COLUMN_4_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[4] > COLUMN_4_MIN_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 5)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[10] > COLUMN_5_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[11] > COLUMN_5_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[12] > COLUMN_5_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[8] > COLUMN_5_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[9] > COLUMN_5_MIN_VALUE_TO_SHOOT  )
			return true;
	}

	if (m_startingXColumn == 6)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[15] > COLUMN_6_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[16] > COLUMN_6_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[17] > COLUMN_6_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[13] > COLUMN_6_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[14] > COLUMN_6_MIN_VALUE_TO_SHOOT  )
			return true;
	}

	if (m_startingXColumn == 7)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[20] > COLUMN_7_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[21] > COLUMN_7_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[22] > COLUMN_7_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[18] > COLUMN_7_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[19] > COLUMN_7_MIN_VALUE_TO_SHOOT  )
			return true;
	}

	if (m_startingXColumn == 8)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[25] > COLUMN_8_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[26] > COLUMN_8_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[23] > COLUMN_8_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[24] > COLUMN_8_MIN_VALUE_TO_SHOOT )
			return true;
	}

	return false;
}

bool Enemy::CheckHighNotesForFire()
{
	if (m_startingXColumn == 9)
	{
		if (g_AudioSystem->m_AverageHighVolumeLastFrame[0] > COLUMN_9_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[1] > COLUMN_9_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_AverageHighVolumeLastFrame[2] > COLUMN_9_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_AverageHighVolumeLastFrame[3] > COLUMN_9_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_AverageHighVolumeLastFrame[4] > COLUMN_9_MIN_VALUE_TO_SHOOT )
			return true;
	}

	if (m_startingXColumn == 10)
	{
		if (g_AudioSystem->m_AverageHighVolumeLastFrame[5] > COLUMN_10_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[6] > COLUMN_10_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_AverageHighVolumeLastFrame[7] > COLUMN_10_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_AverageHighVolumeLastFrame[8] > COLUMN_10_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_AverageHighVolumeLastFrame[9] > COLUMN_10_MIN_VALUE_TO_SHOOT  )
			return true;
	}

	if (m_startingXColumn == 11)
	{
		if (g_AudioSystem->m_AverageHighVolumeLastFrame[10] > COLUMN_11_MIN_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[11] > COLUMN_11_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_AverageHighVolumeLastFrame[12] > COLUMN_11_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_AverageHighVolumeLastFrame[13] > COLUMN_11_MIN_VALUE_TO_SHOOT || 
			g_AudioSystem->m_AverageHighVolumeLastFrame[14] > COLUMN_11_MIN_VALUE_TO_SHOOT )
			return true;
	}

	return false;
}

bool Enemy::CheckBassNotesForOverRideFire()
{
	if (m_startingXColumn == 0)
	{
		if (g_AudioSystem->m_averageBassVolumeLastFrame[0] > COLUMN_0_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 1)
	{
		if (g_AudioSystem->m_averageBassVolumeLastFrame[1] > COLUMN_1_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 2)
	{
		if (g_AudioSystem->m_averageBassVolumeLastFrame[2] > COLUMN_2_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	return false;
}

bool Enemy::CheckMidNotesForOverRideFire()
{
	if (m_startingXColumn == 3)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[0] > COLUMN_3_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[1] > COLUMN_3_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[2] > COLUMN_3_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[2] > COLUMN_3_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[3] > COLUMN_3_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 4)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[5] > COLUMN_4_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[6] > COLUMN_4_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[7] > COLUMN_4_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[4] > COLUMN_4_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[4] > COLUMN_4_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 5)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[10] > COLUMN_5_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[11] > COLUMN_5_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[12] > COLUMN_5_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[8] > COLUMN_5_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[9] > COLUMN_5_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 6)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[15] > COLUMN_6_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[16] > COLUMN_6_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[17] > COLUMN_6_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[13] > COLUMN_6_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[14] > COLUMN_6_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 7)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[20] > COLUMN_7_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[21] > COLUMN_7_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[22] > COLUMN_7_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[18] > COLUMN_7_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[19] > COLUMN_7_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 8)
	{
		if (g_AudioSystem->m_averageMediumHighVolumeLastFrame[25] > COLUMN_8_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[26] > COLUMN_8_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[23] > COLUMN_8_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_averageMediumHighVolumeLastFrame[24] > COLUMN_8_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	return false;
}

bool Enemy::CheckHighNotesForOverRideFire()
{
	if (m_startingXColumn == 9)
	{
		if (g_AudioSystem->m_AverageHighVolumeLastFrame[0] > COLUMN_9_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[1] > COLUMN_9_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[2] > COLUMN_9_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[3] > COLUMN_9_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[4] > COLUMN_9_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 10)
	{
		if (g_AudioSystem->m_AverageHighVolumeLastFrame[5] > COLUMN_10_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[6] > COLUMN_10_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[7] > COLUMN_10_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[8] > COLUMN_10_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[9] > COLUMN_10_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	if (m_startingXColumn == 11)
	{
		if (g_AudioSystem->m_AverageHighVolumeLastFrame[10] > COLUMN_11_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[11] > COLUMN_11_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[12] > COLUMN_11_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[13] > COLUMN_11_MAXOVERRIDE_VALUE_TO_SHOOT ||
			g_AudioSystem->m_AverageHighVolumeLastFrame[14] > COLUMN_11_MAXOVERRIDE_VALUE_TO_SHOOT)
			return true;
	}

	return false;
}


//-----------------------------Rendering------------------------------------------------
void Enemy::Render()
{
	g_renderSystem->CheckError();
	g_renderSystem->PushMatrix();
	g_renderSystem->Translatef(m_position.x, m_position.y, 0.f);

	if (m_MatchWithSong == BASSSTART)
	{
		m_AsteroidColor = Rgba(255, 0, 0, 255);
	}
	if (m_MatchWithSong == MIDSTART)
	{
		m_AsteroidColor = Rgba(0, 255, 0, 255);
	}
	if (m_MatchWithSong == HIGHSTART)
	{
		m_AsteroidColor = Rgba(0, 0, 255, 255);
	}
	//g_renderSystem->Color3f(m_AsteroidColor.r, m_AsteroidColor.g, m_AsteroidColor.b);


	float volumeControlDrawBass = g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * 10;
	float volumeControlDrawMid = g_AudioSystem->m_averageMediumHighVolumeOfWholeSongLastFrame * 10;


	volumeControlDrawBass = RangeMapFloat(volumeControlDrawBass, 0, 6, .75, 2);

	if (m_RenderVarianceNegative == true)
	{
		volumeControlDrawBass = -volumeControlDrawBass;
		volumeControlDrawMid = -volumeControlDrawMid;
	}

	m_RenderVarianceNegative = !m_RenderVarianceNegative;

	g_renderSystem->DrawLine2D(Vector2(0 + volumeControlDrawMid, -15), Vector2(-10 + volumeControlDrawBass, 15), m_AsteroidColor, Rgba(255, 255, 255, 255));
	g_renderSystem->DrawLine2D(Vector2(-10 + volumeControlDrawMid, 15), Vector2(-7 + volumeControlDrawBass, 10), m_AsteroidColor, Rgba(255, 255, 255, 255));

	g_renderSystem->DrawLine2D(Vector2(-7 + volumeControlDrawMid, 10), Vector2(7 + volumeControlDrawMid, 10), m_AsteroidColor, Rgba(255, 255, 255, 255));

	g_renderSystem->DrawLine2D(Vector2(10 + volumeControlDrawMid, 15), Vector2(7 + volumeControlDrawBass, 10), m_AsteroidColor, Rgba(255, 255, 255, 255));
	g_renderSystem->DrawLine2D(Vector2(0 + volumeControlDrawMid, -15), Vector2(10 + volumeControlDrawBass, 15), m_AsteroidColor, Rgba(255, 255, 255, 255));

	//g_renderSystem->drawCircle(m_radius, Rgba(255,0,0,255), Vector2(0,0) );

	float flameDistanceX = .75f;
	float flameDistanceY = .10f;
	//if (abs(volumeControlDrawBass) > .5)
	//{
		g_renderSystem->DrawLine2D(Vector2(volumeControlDrawMid + -7 * flameDistanceX, 10), Vector2(0, abs(volumeControlDrawBass) * 15 + 10 * flameDistanceY), Rgba(255, 0, 0, 255), Rgba(255, 0, 0, 255));
		g_renderSystem->DrawLine2D(Vector2(volumeControlDrawMid + 7 * flameDistanceX, 10), Vector2(0, abs(volumeControlDrawBass) * 15 + 10 * flameDistanceY), Rgba(255, 0, 0, 255), Rgba(255, 0, 0, 255));

		g_renderSystem->DrawLine2D(Vector2(volumeControlDrawMid + -7 * flameDistanceX, 10), Vector2(0, abs(volumeControlDrawBass) * 15 + 10 * (flameDistanceY + .05) - 3), Rgba(255, 0, 0, 255), Rgba(255, 0, 0, 255));
		g_renderSystem->DrawLine2D(Vector2(volumeControlDrawMid + 7 * flameDistanceX, 10), Vector2(0, abs(volumeControlDrawBass) * 15 + 10 * (flameDistanceY +.05) - 3), Rgba(255, 0, 0, 255), Rgba(255, 0, 0, 255));
	/*}
	else
	{
		g_renderSystem->DrawLine2D(Vector2(volumeControlDrawMid + -7 * flameDistanceX, 10), Vector2(0, 12 + 60 * abs(volumeControlDrawBass)), Rgba(255, 0, 0, 255), Rgba(255, 0, 0, 255));
		g_renderSystem->DrawLine2D(Vector2(volumeControlDrawMid + 7 * flameDistanceX, 10), Vector2(0, 12 + 60 * abs(volumeControlDrawBass)), Rgba(255, 0, 0, 255), Rgba(255, 0, 0, 255));
	}*/
		g_renderSystem->CheckError();
	//g_renderSystem->GEnd();
	g_renderSystem->CheckError();
	g_renderSystem->PopMatrix();

	g_renderSystem->CheckError();

	if(m_StartLocationRenderRemaining > 0)
		RenderStartingLocation();

	g_renderSystem->CheckError();
}

void Enemy::RenderStartingLocation()
{
	Rgba StartParticleColor = Rgba(m_AsteroidColor.r, m_AsteroidColor.g, m_AsteroidColor.b, 255 * m_StartLocationRenderRemaining);
	
	Vector2 endLocation = m_StartingLocation;

	if (endLocation.y > 900)
		endLocation.y -= 200;
	else
		endLocation.y += 200;


	Vector2 endLocation2 = endLocation;
	
	if (endLocation.x > 1600)
	{
		endLocation.x -= 200;
		endLocation2.x -= 200;
		endLocation2.y -= 400;
	}
	else if (endLocation.x < 0)
	{
		endLocation.x += 200;
		endLocation2.x += 200;
		endLocation2.y -= 400;
	}
	else
	{
		endLocation.x += 200;
		endLocation2.x -= 200;
	}


	g_renderSystem->DrawLine2D(m_StartingLocation, endLocation, StartParticleColor, StartParticleColor);
	g_renderSystem->DrawLine2D(m_StartingLocation, endLocation2, StartParticleColor, StartParticleColor);
}
