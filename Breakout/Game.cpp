#include "Game.h"

int SCREEN_WIDTH = 1280;
int SCREEN_HEIGHT = 720;

// Constructors / Destructor
Game::Game() : 
mWindow(mWindow = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0)),
mRenderer(SDL_CreateRenderer(mWindow, -1, 0)),
isRunning(true),
mPlatform(mRenderer, SCREEN_WIDTH, SCREEN_HEIGHT),
mBall(mRenderer),
mLevel(mRenderer)
{
	mLastTick = SDL_GetTicks();
	mFpsTicks = mLastTick;
	mFps = 0;
	mFrameCount = 0;

	if (!mRenderer) {
		std::cout << "Failed to make render";
	}
}

Game::~Game()
{
}

// Public Functions

bool Game::Initialize() 
{

	// Initialize video, images and font
	if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0) {
		std::cout << "Failed to initialize video" << SDL_GetError() << std::endl;
		return false;
	}

	if (!IMG_Init(IMG_INIT_PNG)) {
		std::cout << "Failed to initialize images" << SDL_GetError() << std::endl;
		return false;
	}	

	return true;
}

void Game::Shutdown()
{
	SDL_DestroyWindow(mWindow); mWindow = nullptr;
	
	SDL_DestroyRenderer(mRenderer); mRenderer = nullptr;

	SDL_Quit();
	TTF_Quit();
	IMG_Quit();
}

void Game::RunLoop()
{
	while (isRunning) {
		EventPolling();
		UpdateGame();
		Output();
	}
}

// Private Functions
void Game::EventPolling()
{
	SDL_Event event;
	while (SDL_PollEvent(&event)) {
		switch (event.type)
		{
		// Close the window
		case SDL_QUIT:
			isRunning = false;
			break;

		default:
			break;
		}
	}
}

void Game::UpdateGame()
{
	// get deltaTime
	unsigned int curtick = SDL_GetTicks();
	float deltaTime = (curtick - mLastTick) / 1000.0f;
	if (curtick - mFpsTicks >= FPS_DELAY) {
		mFps = mFrameCount * (1000.0f / (curtick - mFpsTicks));
		mFpsTicks = curtick;
		mFrameCount = 0;
	}
	else {
		mFrameCount++;
	}
	mLastTick = curtick;

	if (mLevel.GameStarted()) {
		if (mLevel.GetLevel() != mLevel.GetMaxLevel()) {
			if (mLevel.GetHp() != 0) {
				mPlatform.Move(SCREEN_WIDTH, deltaTime);
				mBall.MoveBall(deltaTime, SCREEN_WIDTH, SCREEN_HEIGHT, mPlatform.GetRectDest(), mLevel.GetBricks(), mLevel.GetMargin(), mLevel);
			}
		}
	}
}

// Render the game to the screen
void Game::Output()
{
	SDL_RenderClear(mRenderer);
	if (mLevel.GameStarted())
	{
		if (mLevel.GetLevel() != mLevel.GetMaxLevel()) {
			if (mLevel.GetHp() != 0) {
				mLevel.Render(SCREEN_WIDTH, SCREEN_HEIGHT, mPlatform.movePos.x, mBall.mScore);
				mPlatform.SpawnPlatform();
				mBall.RenderBall();
			}
			else {
				mLevel.RenderMenu(SCREEN_WIDTH, SCREEN_HEIGHT, "YOU LOST", mPlatform.movePos.x, mBall.mScore);
			}
		}
		else {
			mLevel.RenderMenu(SCREEN_WIDTH, SCREEN_HEIGHT, "VICTORY", mPlatform.movePos.x, mBall.mScore);
		}
	}
	else {
		mLevel.RenderMenu(SCREEN_WIDTH, SCREEN_HEIGHT, "BREAKOUT", mPlatform.movePos.x, mBall.mScore);
	}
	

	SDL_RenderPresent(mRenderer);
}

