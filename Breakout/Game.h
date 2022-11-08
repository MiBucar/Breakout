#pragma once
#include <SDL.h>
#include <SDL_mixer.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <Ball.h>
#include <Level.h>

#define FPS_DELAY 0

class Game
{
public:
	// Constructors / Destructor
	Game();
	~Game();
	// Functions
	bool Initialize();
	void Shutdown();
	void RunLoop();

private:
	// Functions
	void EventPolling();
	void UpdateGame();
	void Output();

	// Objects
	SDL_Window *mWindow;
	SDL_Renderer* mRenderer;

	bool isRunning;

	Platform mPlatform;
	Ball mBall;
	Level mLevel;

	unsigned int mLastTick, mFpsTicks, mFps, mFrameCount;
};

