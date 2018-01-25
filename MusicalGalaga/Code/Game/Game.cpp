//Based on code given by Squirrel Eiserloh
#include "Game/Game.hpp"
#include "Engine/Renderer/Renderer.hpp"
#include "Engine/Math/MathUtils.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Audio/Audio.hpp"
#include "Game/Enemy.hpp"
#include "Game/Pickups.hpp"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Commdlg.h>

//---------------------------------Variables--------------------------------------------------------------
Game* g_theGame = nullptr;

//----------------------------------------Creation/Deletion-------------------------------------------------------
Game::Game()
{
	m_currentPlayedSong = nullptr;
	m_ThePlayer = nullptr;
	m_Score = 0;

	XboxControl* newControl = new XboxControl();
	m_controller = newControl;
	m_cooldown = 0;

	m_vibrateLeft = m_vibrateRight = m_screenshake = 0;
	m_currentWave = 1;
	m_GameBegin = false;
	m_pickupLiklihood = PICKUP_BASE_CHANCE;
	m_FiringMyLazar = false;
}

Game::~Game()
{
}

//-----------------------------Start Game----------------------------------------------
void Game::StartNewGame(HWND hwnd)
{
	m_GameBegin = true;
	PickASong(hwnd);
	ResetVariables();
}

void Game::PickASong(HWND hwnd)
{
	if (m_currentPlayedSong != nullptr)
		g_AudioSystem->StopChannel(m_currentPlayedSong);
	
	OPENFILENAMEA ofn;       // common dialog box structure
	char szFile[260];       // buffer for file name

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = szFile;
	// Set lpstrFile[0] to '\0' so that GetOpenFileName does not 
	// use the contents of szFile to initialize itself.
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(szFile);
	//ofn.lpstrFilter = "All\0*.*\0wav\0*.mp3\0";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	// Display the Open dialog box. 

	TCHAR NPath[1000];
	GetCurrentDirectory(1000, NPath);

	if (GetOpenFileNameA(&ofn) == TRUE)
	{
		SoundID playerShoot = g_AudioSystem->CreateOrGetSound(ofn.lpstrFile);
		if(m_currentPlayedSong == nullptr)
			m_currentPlayedSong = new AudioChannelHandle();
		*m_currentPlayedSong = g_AudioSystem->PlaySound(playerShoot);

		SetCurrentDirectory(NPath);


		return;
	}
	else
	{
		//Medium
		//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/seperateways.mp3");

		//Hard
		//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/TakeOnMe2.mp3");
		//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/WelcometotheClubNow.mp3");
		//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/ReadyToLove.mp3");

		//Expert
		//SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/Demonheart.mp3");
		SoundID playerShoot = g_AudioSystem->CreateOrGetSound("Data/Audio/TheLastStand.mp3");
		if (m_currentPlayedSong == nullptr)
			m_currentPlayedSong = new AudioChannelHandle();
		*m_currentPlayedSong = g_AudioSystem->PlaySound(playerShoot);
		SetCurrentDirectory(NPath);
		return;
	}
}

void Game::StartNewWave()
{
	m_currentWave++;
	DeleteEnemiesAndBullets();
	CreateNewVariables();
}

void Game::ResetVariables()
{
	ResetCrucialVariables();
	DeleteEnemiesAndBullets();
	CreateNewVariables();
}

void Game::ResetCrucialVariables()
{
	if (m_ThePlayer != nullptr)
		delete m_ThePlayer;
	m_FiringMyLazar = false;
	m_Score = 0;
	m_currentWave = 1;
	m_pickupLiklihood = PICKUP_BASE_CHANCE;
	Entity playerEntity(Vector2(800, 100), Vector2(0, 0), 0.0f, 90.0f, 13.f, 1.0f, 3);
	m_ThePlayer = new Player(playerEntity);

	m_linePositions.clear();
}

void Game::DeleteEnemiesAndBullets()
{

	m_EnemyCounter = 0;
	m_Enemies.clear();
	m_Bullets.clear();
}



void Game::CreateNewVariables()
{
	MakeEnemies();
	

	int amountOfStars = GetRandomIntInRange(15, 30);
	int amountOflines= GetRandomIntInRange(45, 60);
	for (int addStars = 0; addStars < amountOflines; addStars++)
	{
		Vector2 position = Vector2((float)GetRandomIntInRange(0, 1600), (float)GetRandomIntInRange(0, 900));
		m_linePositions.push_back(position);
	}

	for (int addStars = 0; addStars < amountOfStars; addStars++)
	{
		Vector2 position = Vector2((float)GetRandomIntInRange(0, 1550), (float)GetRandomIntInRange(0,875));
		m_starPositions.push_back(position);
	}
}

void Game::MakeEnemies()
{
	float xCounter = 0;
	float yCounter = 0;
	for (int addEnemies = 0; addEnemies < MAX_ENEMIES; addEnemies++)
	{
		Entity newEntity(Vector2(ENEMY_X_START + xCounter, ENEMY_Y_START + yCounter), Vector2(0, 0), 0.0f, 90.0f, 13.f, 1.0f, 3);
		Enemy* newEnemy = new Enemy(newEntity);
		newEnemy->m_startingXColumn= xCounter/100;
		newEnemy->m_startingYRow = yCounter/50;

		if (newEnemy->m_startingXColumn <= START_BASS_CAP)
		{
			newEnemy->m_MatchWithSong = BASSSTART;
		}
		else if (newEnemy->m_startingXColumn >= START_BASS_CAP && newEnemy->m_startingXColumn <= START_MID_CAP)
		{
			newEnemy->m_MatchWithSong = MIDSTART;
		}
		else
		{
			newEnemy->m_MatchWithSong = HIGHSTART;
		}

		m_Enemies.push_back(newEnemy);

		xCounter += 100;

		if (xCounter == 1200 && yCounter == 0)
		{
			xCounter = 0;
			yCounter += 50;
		}

		if (xCounter == 1200 && yCounter == 50 || xCounter == 1000 && yCounter == 100 || xCounter == 1000 && yCounter == 150)
		{
			xCounter = 200;
			yCounter += 50;
		}
	}
}

//--------------------------------------Update---------------------------------------------------------
void Game::Update(float deltaSeconds, HWND hwnd)
{
	if(m_currentPlayedSong != nullptr)
		g_AudioSystem->FrequencyAnalysis(m_currentPlayedSong, deltaSeconds);

	m_controller->UpdateController(0);

	if (m_controller->m_isButtonDown[START_BUTTON] == true)
		StartNewGame(hwnd);

	UpdateGameElements(deltaSeconds);
}

void Game::UpdateGameElements(float deltaSeconds)
{
	if (m_ThePlayer != nullptr && m_EnemyCounter >= 40)
		StartNewWave();

	if (m_ThePlayer != nullptr && m_FiringMyLazar == false)
		m_ThePlayer->Update(deltaSeconds, m_controller);

	UpdateEnemies(deltaSeconds);
	UpdateBullets(deltaSeconds);
	UpdateExplosions(deltaSeconds);
	UpdateGameElementsDependingOnSong(deltaSeconds);
	UpdateStars(deltaSeconds);
	UpdatePickups(deltaSeconds);
}

//---------------------------Update Enemies----------------------------------------------------

void Game::UpdateEnemies(float deltaSeconds)
{
	for (unsigned int cleanStep = 0; cleanStep < m_Enemies.size(); cleanStep++)
	{
		if (m_Enemies[cleanStep] != nullptr)
		{
			if (DidCollsionHappenWithPlayerAndEnemy() == false)
			{
				m_Enemies[cleanStep]->Update(deltaSeconds);

				if (AmIFrontEnemy(cleanStep) && m_Enemies[cleanStep]->ShouldIFire() == true)
				{
					CreateNewBullet(m_Enemies[cleanStep]->m_position, false, false, false);
					m_Enemies[cleanStep]->m_TimeSinceLastShot = ENEMY_WAIT_TO_SHOOT_TIME;
				}
				else if (m_Enemies[cleanStep]->m_deathTime > 0 && m_Enemies[cleanStep]->m_wrappingHasHappened == true)
				{
					m_Enemies[cleanStep]->m_wrappingHasHappened = false;
				}

			}
		}
	}
}

bool Game::AmIFrontEnemy(unsigned int enemyStep)
{
	for (unsigned int cleanStep = 0; cleanStep < m_Enemies.size(); cleanStep++)
	{
		if (m_Enemies[cleanStep] != nullptr)
		{
			if (m_Enemies[enemyStep]->m_startingXColumn == m_Enemies[cleanStep]->m_startingXColumn)
				if (m_Enemies[enemyStep]->m_startingYRow > m_Enemies[cleanStep]->m_startingYRow)
					return false;
		}
	}

	return true;
}

bool Game::DidCollsionHappenWithPlayerAndEnemy()
{
	for (unsigned int enemyStep = 0; enemyStep < m_Enemies.size(); enemyStep++)
	{
		if (m_Enemies[enemyStep] != nullptr)
		{
			float distFromAsteroid = CalcDistance(m_ThePlayer->m_position, m_Enemies[enemyStep]->m_position);
			if (distFromAsteroid < (m_Enemies[enemyStep]->m_PhysicsRadius + m_ThePlayer->m_PhysicsRadius))
			{

				m_Enemies[enemyStep] = nullptr;
				m_ThePlayer->m_health -= 1;

				m_screenshake += 15;
				m_vibrateLeft = 55000;
				m_vibrateRight = 95000;
				return true;
			}
		}
	}
	return false;
}


//---------------------Update Bullets-------------------------------------------------------
void Game::UpdateBullets(float deltaSeconds)
{
	m_cooldown += deltaSeconds;

	if (g_inputSystem->IsKeyDown(KEY_SPACE) == true && m_ThePlayer != nullptr && m_ThePlayer->m_WaitToShootPenalty <= 0 || m_controller->m_isButtonDown[A_BUTTON] == true && m_ThePlayer != nullptr && m_ThePlayer->m_WaitToShootPenalty <= 0)
	{
		if (m_ThePlayer->m_currentWeaponAbility == NORMAL)
			CreateNewBullet(m_ThePlayer->m_position, true, false, false);


		if (m_ThePlayer->m_currentWeaponAbility == GUIDED_ROCKETS)
		{
			CreateNewBullet(m_ThePlayer->m_position, true, true, false);
			CreateNewBullet(m_ThePlayer->m_position, true, true, false);
			CreateNewBullet(m_ThePlayer->m_position, true, true, false);
			CreateNewBullet(m_ThePlayer->m_position, true, true, false);
			CreateNewBullet(m_ThePlayer->m_position, true, true, false);
			CreateNewBullet(m_ThePlayer->m_position, true, true, false);
			m_ThePlayer->m_currentWeaponAbility = NORMAL;
		}

		if (m_ThePlayer->m_currentWeaponAbility == LASER)
		{
			CreateNewBullet(m_ThePlayer->m_position, true, false, true);
			m_ThePlayer->m_currentWeaponAbility = NORMAL;
			m_FiringMyLazar = true;

			m_screenshake = 100;
		}
		m_cooldown = 0;
	}


	
	for (unsigned int cleanStep = 0; cleanStep < m_Bullets.size(); cleanStep++)
	{
		if (m_Bullets[cleanStep] != nullptr)
		{
			if (m_Bullets[cleanStep]->m_isPlayerBullet == true && DidBulletHitAnyAsteroids(m_Bullets[cleanStep]) && m_Bullets[cleanStep]->m_Lazar == false|| m_Bullets[cleanStep]->m_isPlayerBullet == false && DidBulletHitPlayer(m_Bullets[cleanStep]))
			{
				delete m_Bullets[cleanStep];
				m_Bullets[cleanStep] = nullptr;
			}
			else 
			{
 				if (m_Bullets[cleanStep]->m_deathTime > 0)
					m_Bullets[cleanStep]->Update(deltaSeconds, m_Enemies, m_ThePlayer);
				else
				 m_Bullets[cleanStep] = nullptr;

				if (m_Bullets[cleanStep] != nullptr && m_Bullets[cleanStep]->m_targetEnemy == 10000 || m_Bullets[cleanStep] != nullptr && m_Bullets[cleanStep]->m_deathTime <= 0)
				{
					if (m_Bullets[cleanStep]->m_Lazar == true)
						m_FiringMyLazar = false;

					delete m_Bullets[cleanStep];
					m_Bullets[cleanStep] = nullptr;
				}
			}
		}
	}
}

bool Game::DidBulletHitAnyAsteroids(Bullet* bullet)
{
	for (unsigned int asteroidStep = 0; asteroidStep < m_Enemies.size(); asteroidStep++)
	{
		if (m_Enemies[asteroidStep] != nullptr)
		{
			float distFromAsteroid = CalcDistance(bullet->m_position, m_Enemies[asteroidStep]->m_position);
			if (distFromAsteroid < (m_Enemies[asteroidStep]->m_PhysicsRadius + bullet->m_PhysicsRadius))
			{
				m_Score += 100 * m_currentWave;

				if (m_pickupLiklihood > GetRandomFloatInRange(0, 1))
				{
					SpawnPickup(m_Enemies[asteroidStep]->m_position);
					m_pickupLiklihood = PICKUP_BASE_CHANCE;
				}
				else
					m_pickupLiklihood += PICKUP_INCREASE_PER_KILL;


				Explosion* newExplosion = new Explosion(m_Enemies[asteroidStep]->m_position, m_Enemies[asteroidStep]->m_radius, EXPLOSION_LIFETIME, m_Enemies[asteroidStep]->m_position);
				m_Explosions.push_back(newExplosion);
				delete m_Enemies[asteroidStep];
				m_EnemyCounter++;
				m_screenshake = 3;
				m_vibrateLeft = 40000;
				m_vibrateRight = 60000;
				return true;
			}
		}
	}
	return false;
}

bool Game::DidBulletHitPlayer(Bullet* bullet)
{
	if (m_ThePlayer != nullptr)
	{
		float distFromAsteroid = CalcDistance(bullet->m_position, m_ThePlayer->m_position);
		if (distFromAsteroid < (m_ThePlayer->m_PhysicsRadius + bullet->m_PhysicsRadius))
		{
			Explosion* newExplosion = new Explosion(m_ThePlayer->m_position, m_ThePlayer->m_radius, EXPLOSION_LIFETIME, Vector2(-999, -999));
			m_Explosions.push_back(newExplosion);
			m_ThePlayer->m_health -= 1;
			m_screenshake = 3;
			m_vibrateLeft = 40000;
			m_vibrateRight = 60000;
			return true;
		}
	}
	return false;
}


void Game::CreateNewBullet(Vector2 position, bool didPlayerShootIt, bool guidedRocket, bool lazar)
{
	Entity newEntity;
	if (didPlayerShootIt)
	{

		if (lazar == true)
		{
			newEntity = Entity(position, Vector2(BULLET_SPEED * CosDegrees(90), BULLET_SPEED * 4 * SinDegrees(90)), 0, 90, 50, 50, 1);
			m_ThePlayer->m_WaitToShootPenalty = 1;
			Bullet* newBullet = new Bullet(newEntity, didPlayerShootIt, guidedRocket, lazar);
			m_Bullets.push_back(newBullet);
		}
		else
		{
			newEntity = Entity(position, Vector2(BULLET_SPEED * CosDegrees(90), BULLET_SPEED * SinDegrees(90)), 0, 90, 18, 18, 1);
			m_ThePlayer->m_WaitToShootPenalty = 1;
			Bullet* newBullet = new Bullet(newEntity, didPlayerShootIt, guidedRocket, lazar);
			m_Bullets.push_back(newBullet);
		}
	}
	else
	{
		newEntity = Entity(position, Vector2(BULLET_SPEED * CosDegrees(90), -BULLET_SPEED * SinDegrees(90)), 0, 270, 18, 18, 1);
		Bullet* newBullet = new Bullet(newEntity, didPlayerShootIt);	
		m_Bullets.push_back(newBullet);
	}
}


//-----------------------------------Update Explosions----------------------------------------------

void Game::UpdateExplosions(float deltaSeconds)
{
	for (unsigned int cleanStep = 0; cleanStep < m_Explosions.size(); cleanStep++)
	{
		if (m_Explosions[cleanStep] != nullptr)
		{
			m_Explosions[cleanStep]->Update(deltaSeconds);
			if (m_Explosions[cleanStep]->explosionDone == true)
				m_Explosions[cleanStep] = nullptr;
		}
	}
}

void Game::UpdateStars(float deltaSeconds)
{
	for (unsigned int i = 0; i < m_linePositions.size(); i++)
	{
		m_linePositions[i].y += deltaSeconds * DEBRIES_GRAVITY * -g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame;

		if (m_linePositions[i].y < -50)
			m_linePositions[i] = Vector2(GetRandomIntInRange(0, 1550), GetRandomIntInRange(900, 950));
	}


	for (unsigned int cleanStep = 0; cleanStep < m_starPositions.size(); cleanStep++)
	{
		m_starPositions[cleanStep].y += deltaSeconds * DEBRIES_GRAVITY * -g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame;

		if (m_starPositions[cleanStep].y < -50)
			m_starPositions[cleanStep] = Vector2(GetRandomIntInRange(0, 1550), GetRandomIntInRange(900, 950));
	}
}


//-------------------------------------------Update Pickups------------------------------------------------
void Game::UpdatePickups(float deltaseconds)
{
	for (unsigned int i = 0; i < m_Pickups.size(); i++)
	{
		if (m_Pickups[i] != nullptr)
		{
			m_Pickups[i]->Update(deltaseconds);

			if (DidPickupHitPlayer(m_Pickups[i]) == true)
			{
				delete m_Pickups[i];
				m_Pickups[i] = nullptr;
			}
		}
	}
}

bool Game::DidPickupHitPlayer(Pickups* pickup)
{
	if (m_ThePlayer != nullptr)
	{
		float distFromAsteroid = CalcDistance(pickup->m_position, m_ThePlayer->m_position);
		if (distFromAsteroid < (m_ThePlayer->m_PhysicsRadius + pickup->m_PhysicsRadius))
		{
			m_ThePlayer->m_currentWeaponAbility = pickup->m_typeOfPowerup;
			return true;
		}
	}
	return false;
}


void Game::SpawnPickup(Vector2 position)
{
	float percent = GetRandomFloatInRange(0, 1);

	if (percent < .5)
	{
		Entity newEntity(position, Vector2(BULLET_SPEED * CosDegrees(90), BULLET_SPEED * SinDegrees(90)), 0, 0, 18, 18, 1);
		Pickups* newpickup = new Pickups(newEntity, LASER);
		m_Pickups.push_back(newpickup);
	}
	if (percent > .5)
	{
		Entity newEntity(position, Vector2(BULLET_SPEED * CosDegrees(90), BULLET_SPEED * SinDegrees(90)), 0, 0, 18, 18, 1);
		Pickups* newpickup = new Pickups(newEntity, GUIDED_ROCKETS);
		m_Pickups.push_back(newpickup);
	}
}


//---------------------------Update Game Based on Audio----------------------------------------------------------
void Game::UpdateGameElementsDependingOnSong(float deltaSeconds)
{
	if (m_vibrateLeft < 6300)
	{
		m_vibrateLeft = g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * 63000;
		m_vibrateRight = g_AudioSystem->m_averageMediumAllVolumeOfWholeSongLastFrame * 63000;
	}
}



//-----------------------------------Rendering------------------------------------------------------------
void Game::Render()
{
	BasicRenderSetUp();
	RenderGameElements();
	RenderUI();
}

void Game::BasicRenderSetUp()
{
	g_renderSystem->LoadIdentity();
	RenderScreenShake();
	g_renderSystem->ClearColor(0.0f, 0.f, 0.f, 1.f);
	g_renderSystem->ClearColorBufferBit();
}

void Game::RenderScreenShake()
{
	float m_ScreenShakeX = GetRandomFloatInRange(-m_screenshake, m_screenshake);
	float m_ScreenShakeY = GetRandomFloatInRange(-m_screenshake, m_screenshake);
	m_screenshake = m_screenshake * .98f;
	g_renderSystem->Ortho(0.f + m_ScreenShakeX, 1600.f + m_ScreenShakeX, 0.f + m_ScreenShakeY, 900.f + m_ScreenShakeY, 0.f, 1.f);


	m_controller->UpdateVibrationValue(m_vibrateLeft, m_vibrateRight);
	m_vibrateLeft = m_vibrateLeft * .95f;
	m_vibrateRight = m_vibrateRight * .95f;

	
}



void Game::RenderGameElements()
{
	g_renderSystem->CheckError();
	RenderStars();
	if (m_ThePlayer != nullptr && m_ThePlayer->m_health > 0)
		m_ThePlayer->Render(m_controller);
	else
		RenderGameOverScreen();

	for (unsigned int cleanStep = 0; cleanStep < m_Enemies.size(); cleanStep++)
	{
		if (m_Enemies[cleanStep] != nullptr)
		{
			m_Enemies[cleanStep]->Render();
			//g_renderSystem->DrawLine2D(m_Enemies[cleanStep]->m_position, m_ThePlayer->m_position, Rgba(255, 255, 255, 255), Rgba(255, 255, 255, 255));
		}
	}


	for (unsigned int cleanStep = 0; cleanStep < m_Bullets.size(); cleanStep++)
	{
		if(m_Bullets[cleanStep] != nullptr)
			m_Bullets[cleanStep]->Render();
	}

	for (unsigned int cleanStep = 0; cleanStep < m_Explosions.size(); cleanStep++)
	{
		if (m_Explosions[cleanStep] != nullptr)
			m_Explosions[cleanStep]->Render();
	}


	RenderPickups();
}


void Game::RenderStars()
{
	g_renderSystem->CheckError();
	for (unsigned int i = 0; i < m_linePositions.size(); i++)
	{
		float a = RangeMapFloat(g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame, 0, .5, 191.25f, 0);
		g_renderSystem->DrawLine2D(Vector2(m_linePositions[i].x, m_linePositions[i].y - 100 * g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame), Vector2(m_linePositions[i].x, m_linePositions[i].y + 100 * g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame), Rgba(255, 255, 255, 255), Rgba(255, 255, 255, a));
	}

	for (unsigned int i = 0; i < m_starPositions.size(); i++)
	{
		const int NUM_VERTEXES = 48;
		const float DEGREES_PER_VERTEX = 360.f / (float)NUM_VERTEXES;
		g_renderSystem->PushMatrix();
		g_renderSystem->Scalef(1.f, 1.f, 1.f);

		float a = RangeMapFloat(g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame, 0, .5, .75f, 0);
		g_renderSystem->Color4f(1.f, 1.f, 1.f, a);
		g_renderSystem->BeginPolygon();


		for (int vertexNum = 0; vertexNum < NUM_VERTEXES; ++vertexNum)
		{
		float degrees = DEGREES_PER_VERTEX * (float)vertexNum;
		float x = GetRandomIntInRange(22, 30) * CosDegrees(degrees);
		float y = GetRandomIntInRange(22, 30) * SinDegrees(degrees);

		Vector2 position = Vector2(m_starPositions[i].x + g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * x, m_starPositions[i].y + g_AudioSystem->m_averageBassVolumeOfWholeSongLastFrame * y);

		g_renderSystem->Vertex2f(position.x, position.y);
		}

		g_renderSystem->GEnd();
		g_renderSystem->PopMatrix();

	}
}


void Game::RenderPickups()
{
	for (unsigned int i = 0; i < m_Pickups.size(); i++)
	{
		if (m_Pickups[i] != nullptr)
		{
			m_Pickups[i]->Render();
		}
	}
}



void Game::RenderGameOverScreen()
{
	g_renderSystem->DrawText2D(Vector2(500, 400),"GAME OVER", 100, 600, Rgba(255, 0, 0, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(500, 300), "Press F1 to pick a new song and restart", 100, 600, Rgba(255, 0, 0, 255), g_textSprites);
}

void Game::RenderUI()
{

	g_renderSystem->DrawCenteredText2D(Vector2(50, 850), "Score:", 25, 50, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(75, 835), std::to_string(m_Score), 25, 100, Rgba(255, 255, 255, 255), g_textSprites);

	if(m_ThePlayer != nullptr)
		m_ThePlayer->RenderUIShips();
}





void Game::RenderDebug()
{
	g_AudioSystem->RenderGraphicsEqualizer(g_renderSystem, g_textSprites);
}
