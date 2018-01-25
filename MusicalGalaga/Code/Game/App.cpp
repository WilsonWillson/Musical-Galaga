#include "Engine/Core/Console.hpp"
#include "Engine/Input/InputSystem.hpp"
#include "Engine/Core/Time.hpp"
#include "Engine/Audio/Audio.hpp"
#include "Engine/Core/ErrorWarningAssert.hpp"
#include "Game/App.hpp"
#include "Game/Game.hpp"


//--------------------------Variables------------------------------------------------------------------
const float MIN_FRAMES_PER_SECOND = 10.f; // Game time will slow down beneath this framerate
const float MAX_FRAMES_PER_SECOND = 60.f;
const float MIN_SECONDS_PER_FRAME = (1.f / MAX_FRAMES_PER_SECOND);
const float MAX_SECONDS_PER_FRAME = (1.f / MIN_FRAMES_PER_SECOND);
App* g_theApp = nullptr;


//-----------------------Creation/Deletion--------------------------------------------------------------
App::App()
{
	g_theGame = new Game();
	g_inputSystem = new InputSystem();
	g_AudioSystem = new AudioSystem();
	g_renderSystem = new Renderer();
	m_console = new Console();

	Texture* texture2 = g_renderSystem->CreateOrGetTexture("Fonts/SquirrelFixedFont.png");
	SpriteSheet* spirteSheet = new SpriteSheet(texture2, 16, 16);
	g_textSprites = new BitMapFont();
	g_textSprites->m_spriteSheet = spirteSheet;

	m_isConsoleCommandOpen = false;
	m_RenderDebug = false;

	m_NoVolumeFrameCounter = 1.0f; 
}

App::~App()
{

}

//----------------------------Run Frame---------------------------------------------------------------

void App::RunFrame(HWND hwnd)
{
	Update(hwnd);
	Render();
}

//------------------------Update------------------------------------------------------------------------

void App::Update(HWND hwnd)
{
	float deltaSeconds = GetDeltaSeconds();
	g_inputSystem->Update(hwnd);
	g_AudioSystem->Update();

	if (g_AudioSystem->m_WasThereNoVolumeLastFrame == true)
	{
		m_NoVolumeFrameCounter -= .01f;

		if (m_NoVolumeFrameCounter < 0)
			m_NoVolumeFrameCounter = 0;

		deltaSeconds *= m_NoVolumeFrameCounter;
	}
	else
	{
		m_NoVolumeFrameCounter = 1.0f;
	}


	if (g_theGame != nullptr)
		g_theGame->Update((float)deltaSeconds, hwnd);
}

float App::GetDeltaSeconds()
{
	double timeNow = GetCurrentTimeSeconds();
	static double lastFrameTime = timeNow;
	double deltaSeconds = timeNow - lastFrameTime;

	// Wait until [nearly] the minimum frame time has elapsed (limit framerate to within the max)
	while (deltaSeconds < MIN_SECONDS_PER_FRAME * .999f)
	{
		timeNow = GetCurrentTimeSeconds();
		deltaSeconds = timeNow - lastFrameTime;
	}
	lastFrameTime = timeNow;

	// Clamp deltaSeconds to be within the max time allowed (e.g. sitting at a debug break point)
	if (deltaSeconds > MAX_SECONDS_PER_FRAME)
	{
		deltaSeconds = MAX_SECONDS_PER_FRAME;
	}
	return (float)deltaSeconds;
}

bool App::isQuitting() const
{
	return m_isQuitting;
}

void App::OnExitRequested()
{
	m_isQuitting = true;
}




//--------------------User Input------------------------------------------------------------------------

void App::OnKeyDown(int asKey, HWND hwnd)
{
	g_inputSystem->OnKeyDown(asKey);

	//Run Normal Commands
	if (g_theGame == nullptr || m_isConsoleCommandOpen == false)
		NormalGameLogic(asKey, hwnd);

	//Run Console Commands
	else
	{
		if (asKey == Key_TILDE)
		{
			m_isConsoleCommandOpen = false;
			return;
		}
		m_console->ConsoleCommand(asKey);
	}

}

void App::NormalGameLogic(int asKey, HWND hwnd)
{
	if (asKey == Key_TILDE && g_theGame != nullptr)
		m_isConsoleCommandOpen = true;
	if (asKey == KEY_ESCAPE)
		OnExitRequested();
	if (asKey == KEY_F1)
		g_theGame->StartNewGame(hwnd); 
}


void App::OnKeyUp(int asKey)
{
	g_inputSystem->OnKeyUp(asKey);
}


//--------------------------Rendering-------------------------------------------------------------------------

void App::Render() const
{
	g_theGame->BasicRenderSetUp();
	if (g_theGame != nullptr && g_theGame->m_GameBegin == true)
		g_theGame->Render();
	if (g_theGame != nullptr && g_theGame->m_GameBegin == false)
		RenderMainMenu();
	if (m_isConsoleCommandOpen == true)
		RenderConsoleWindow();
	if (m_RenderDebug == true)
		g_theGame->RenderDebug();
}

void App::RenderMainMenu() const
{
	g_renderSystem->LoadIdentity();
	g_renderSystem->ClearColor(0.0f, 0.f, 0.f, 1.f);
	g_renderSystem->ClearColorBufferBit();
	g_renderSystem->Ortho(0.f, 1600.f, 0.f, 900.f, 0.f, 1.f);
	g_renderSystem->DrawText2D(Vector2(500.0f, 550.0f), "MUSICAL GALAGA", 30.0f, 600, Rgba(255, 255, 255, 255), g_textSprites);
	g_renderSystem->DrawText2D(Vector2(650.0f, 450.0f), "Press F1 to Begin", 30.0f, 300, Rgba(255, 255, 255, 255), g_textSprites);
}

void App::RenderConsoleWindow() const
{
	g_renderSystem->DrawAABB2(AABB2(0, 100, 1600, 900), Rgba(255, 255, 255, 150));
	g_renderSystem->DrawText2D(Vector2(200.0f, 150.0f), m_console->m_currentConsoleCommand, 30.0f, 25.0f * m_console->m_currentConsoleCommand.length(), Rgba(100, 200, 255, 255), g_textSprites);

	int currentPrintCounter = 0;
	for (int consoleCommandHistoryStep = m_console->m_CommandHistory.size() - 1; consoleCommandHistoryStep > -1; consoleCommandHistoryStep--)
	{
		g_renderSystem->DrawText2D(Vector2(200.0f, 200.0f + currentPrintCounter * 50.0f), m_console->m_CommandHistory[consoleCommandHistoryStep], 30.0f, 25.0f * m_console->m_CommandHistory[consoleCommandHistoryStep].length(), Rgba(0, 0, 0, 255), g_textSprites);
		currentPrintCounter++;
	}
}

void App::RenderDebugHelper()
{
	m_RenderDebug = !m_RenderDebug;
}

//---------------------------Console Window------------------------------------------------------------------------

void App::DeleteConsoleWindow()
{
	m_isConsoleCommandOpen = false;
	ShowCursor(true);
	//g_theGame->DeleteConsoleWindow();
	return;
}