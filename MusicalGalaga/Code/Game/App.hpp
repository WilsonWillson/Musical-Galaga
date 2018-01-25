#pragma once
#include "Game/Game.hpp"

class Console;

class App
{

public:
	App();
	~App();

	void RunFrame(HWND hwnd);

	void OnExitRequested();
	bool isQuitting() const;

	void OnKeyDown(int keyCode, HWND hwnd);
	void OnKeyUp(int keyCode);

	void DeleteConsoleWindow();

	void RenderDebugHelper();

private:
	void Update(HWND hwnd);
	float GetDeltaSeconds();

	void Render() const;
	void RenderMainMenu() const;
	void RenderConsoleWindow() const;

	void NormalGameLogic(int asKey, HWND hwnd);


public:
	Console* m_console;
	bool m_isConsoleCommandOpen;

private:
	bool m_isQuitting;
	bool m_RenderDebug;
	float m_NoVolumeFrameCounter;
};

extern App* g_theApp;
