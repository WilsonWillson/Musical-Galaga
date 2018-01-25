#pragma once
#include "Game/Entity.hpp"
#include "Engine/Core/Rgba.hpp"

enum EnemyPosition
{
	BASSSTART,
	MIDSTART,
	HIGHSTART,
};

const int ASTEROID_NUM_SIZE = 17;
const float DEATH_TIME = 8;
const float START_RENDER_TIME = 1;
const float ENEMY_WAIT_TO_OVERRIDE_BASS_SHOOT_TIME = .5f;
const float ENEMY_WAIT_TO_OVERRIDE_MID_SHOOT_TIME = .65f;
const float ENEMY_WAIT_TO_OVERRIDE_HIGH_SHOOT_TIME = .5f;
const float ENEMY_WAIT_TO_SHOOT_TIME = 1.f;


const float COLUMN_0_MAX_HEIGHT = .25f;
const float COLUMN_1_MAX_HEIGHT = .25f;
const float COLUMN_2_MAX_HEIGHT = .25f;
const float COLUMN_3_MAX_HEIGHT = .12f;
const float COLUMN_4_MAX_HEIGHT = .12f;
const float COLUMN_5_MAX_HEIGHT = .12f;
const float COLUMN_6_MAX_HEIGHT = .12f;
const float COLUMN_7_MAX_HEIGHT = .10f;
const float COLUMN_8_MAX_HEIGHT = .10f;
const float COLUMN_9_MAX_HEIGHT = .04f;
const float COLUMN_10_MAX_HEIGHT = .04f;
const float COLUMN_11_MAX_HEIGHT = .04f;

const float COLUMN_0_MIN_VALUE_TO_SHOOT = .2f;
const float COLUMN_1_MIN_VALUE_TO_SHOOT = .17f;
const float COLUMN_2_MIN_VALUE_TO_SHOOT = .15f;
const float COLUMN_3_MIN_VALUE_TO_SHOOT = .13f;
const float COLUMN_4_MIN_VALUE_TO_SHOOT = .12f;
const float COLUMN_5_MIN_VALUE_TO_SHOOT = .11f;
const float COLUMN_6_MIN_VALUE_TO_SHOOT = .09f;
const float COLUMN_7_MIN_VALUE_TO_SHOOT = .07f;
const float COLUMN_8_MIN_VALUE_TO_SHOOT = .05f;
const float COLUMN_9_MIN_VALUE_TO_SHOOT = .01f;
const float COLUMN_10_MIN_VALUE_TO_SHOOT = .01f;
const float COLUMN_11_MIN_VALUE_TO_SHOOT = .01f;

const float COLUMN_0_MAXOVERRIDE_VALUE_TO_SHOOT = .28f;
const float COLUMN_1_MAXOVERRIDE_VALUE_TO_SHOOT = .24f;
const float COLUMN_2_MAXOVERRIDE_VALUE_TO_SHOOT = .22f;
const float COLUMN_3_MAXOVERRIDE_VALUE_TO_SHOOT = .18f;
const float COLUMN_4_MAXOVERRIDE_VALUE_TO_SHOOT = .17f;
const float COLUMN_5_MAXOVERRIDE_VALUE_TO_SHOOT = .10f;
const float COLUMN_6_MAXOVERRIDE_VALUE_TO_SHOOT = .14f;
const float COLUMN_7_MAXOVERRIDE_VALUE_TO_SHOOT = .12f;
const float COLUMN_8_MAXOVERRIDE_VALUE_TO_SHOOT = .10f;
const float COLUMN_9_MAXOVERRIDE_VALUE_TO_SHOOT = .02f;
const float COLUMN_10_MAXOVERRIDE_VALUE_TO_SHOOT = .02f;
const float COLUMN_11_MAXOVERRIDE_VALUE_TO_SHOOT = .02f;


class Enemy : public Entity
{
public:
	Enemy();
	Enemy(Entity enemyStats);
	Enemy(Entity enemyStats, Rgba asteroidColor);
	~Enemy();


	void Update(float deltaSeconds);
	bool ShouldIFire();

	void Render();

private:
	void RenderStartingLocation();
	bool CheckBassNotesForFire();
	bool CheckMidNotesForFire();
	bool CheckHighNotesForFire();

	bool CheckBassNotesForOverRideFire();
	bool CheckMidNotesForOverRideFire();
	bool CheckHighNotesForOverRideFire();
	float DetermineHeightFromSong();
public:
	Vector2 m_vertexes[ASTEROID_NUM_SIZE];
	Rgba m_AsteroidColor;
	int m_size;
	float m_deathTime;
	Vector2 m_StartingLocation;
	float     m_StartLocationRenderRemaining;
	float m_TimeSinceLastShot;
	EnemyPosition m_MatchWithSong;
	float m_startingYRow;
	float m_startingXColumn;
	bool m_isBeingTracked;

private:
	bool m_RenderVarianceNegative;
};
