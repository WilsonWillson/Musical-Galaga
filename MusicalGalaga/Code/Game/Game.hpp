#pragma once
#include "Engine/Input/XboxControl.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Game/GameCommon.hpp"
#include "Game/Player.hpp"
#include "Game/Enemy.hpp"
#include "Game/Bullet.hpp"
#include "Game/Explosion.hpp"




//-----------------------------------------------------------------------------------------------

typedef void* AudioChannelHandle;

const float EXPLOSION_LIFETIME = .25f;
const int MAX_ENEMIES = 40;
const int ENEMY_X_START = 250;
const int ENEMY_Y_START = 600;
const int START_BASS_CAP = 2;
const int START_MID_CAP = 8;
const int MAX_STARS = 15;
const float PICKUP_BASE_CHANCE = .20;
const float PICKUP_INCREASE_PER_KILL = .05;


class Pickups;

class Game
{
public:
	Game();
	~Game();

	void Update(float deltaSeconds, HWND hwnd);

	void StartNewGame(HWND hwnd);

	void Render();
	void BasicRenderSetUp();
	void RenderDebug();


private:
	void PickASong(HWND hwnd);
	void StartNewWave();	
	void CreateNewVariables();
	void DeleteEnemiesAndBullets();
	void ResetVariables();
	void MakeEnemies();
	void ResetCrucialVariables();

	void UpdateGameElements(float deltaSeconds);
	
	void UpdateBullets(float deltaSeconds);
	bool DidBulletHitPlayer(Bullet*  m_Bullets);
	bool DidBulletHitAnyAsteroids(Bullet* bullet);


	void CreateNewBullet(Vector2 position, bool didPlayerShootIt, bool guidedRocket, bool lazar);
	void UpdateEnemies(float deltaSeconds);
	bool AmIFrontEnemy(unsigned int enemyStep);
	bool DidCollsionHappenWithPlayerAndEnemy();


	void UpdateExplosions(float deltaSeconds);
	void UpdateStars(float deltaSeconds);

	void UpdatePickups(float deltaseconds);
	bool DidPickupHitPlayer(Pickups* pickup);
	void UpdateGameElementsDependingOnSong(float deltaSeconds);


	void RenderGameElements();
	void RenderUI();
	void RenderScreenShake();
	void RenderStars();

	void RenderPickups();
	void RenderGameOverScreen();
	void SpawnPickup(Vector2 position);


public:
	AudioChannelHandle*		m_currentPlayedSong;
	XboxControl*			m_controller;

	Player*					m_ThePlayer;
	std::vector<Enemy*>		m_Enemies;
	std::vector<Explosion*>	m_Explosions;
	std::vector<Bullet*>	m_Bullets;
	std::vector<Pickups*>	m_Pickups;
	std::vector<Vector2>	m_linePositions;
	std::vector<Vector2>	m_starPositions;


	float					m_Score;
	float					m_cooldown;
	float					m_screenshake;
	float					m_vibrateLeft;
	float					 m_vibrateRight;
	int						m_currentWave;
	int						m_EnemyCounter;
	float					m_pickupLiklihood;

	bool					m_GameBegin;
	bool					m_FiringMyLazar;
};


//-----------------------------------------------------------------------------------------------
extern Game* g_theGame;

