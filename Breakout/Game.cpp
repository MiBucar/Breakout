#include "Game.h"

int SCREEN_WIDTH = 240;
int SCREEN_HEIGHT = 320;

// Constructors
Game::Game() : mWindow(nullptr), mRenderer(nullptr)
{
}

Game::~Game()
{
}

// Functions

bool Game::Initialize()
{
	if (SDL_Init(SDL_INIT_VIDEO) != 0) {
		// Create the window
		mWindow = SDL_CreateWindow("Breakout", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_WIDTH, SCREEN_HEIGHT, 0);
		if (!mWindow) {
			std::cout << "Failed to create the window" << SDL_GetError() << std::endl;
			return false;
		}

		// Create the renderer

		return true;
	}
}
