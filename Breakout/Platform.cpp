#include "Platform.h"
#include <iostream>

int PLATFORM_WIDTH = 80;
int PLATFORM_HEIGHT = 20;


// Constructors / Destructor
Platform::Platform(SDL_Renderer* renderer, int width, int height) : mRenderer(renderer), mDest{0, 0, 0, 0}, direction(0)
{
	mTexture = IMG_LoadTexture(mRenderer, "images/Platform.png");

	movePos.x = (width / 2) - PLATFORM_WIDTH; // Place it in the middle of the x position of the screen
	movePos.y = height - PLATFORM_HEIGHT * 2; // Place it a little above the end of the y position

	moveVel.x = 3.0f * PLATFORM_SPEED;
	moveVel.y = 0;
}

Platform::~Platform()
{
	SDL_DestroyRenderer(mRenderer); mRenderer = nullptr;
	SDL_DestroyTexture(mTexture); mTexture = nullptr;
}

//Functions
void Platform::SpawnPlatform() {
	SDL_Rect src{
		0, 0, PLATFORM_WIDTH, PLATFORM_HEIGHT
	};

	mDest.x = movePos.x;
	mDest.y = movePos.y;
	mDest.w = PLATFORM_WIDTH;
	mDest.h = PLATFORM_HEIGHT;

	SDL_RenderCopy(mRenderer, mTexture, &src, &mDest);
}

void Platform::Move(int width, float deltaTime)
{
	direction = 0;

	// Keyboard input
	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_D]) {
		direction += 1;
	}
	if (state[SDL_SCANCODE_A]) {
		direction -= 1;
	}

	if (direction != 0) {
		movePos.x += deltaTime * moveVel.x * direction;
	}

	CheckBoundaries(width);
}

SDL_Rect Platform::GetRectDest()
{
	return mDest;
}

// Private Functions

// Disable moving out of screen width
void Platform::CheckBoundaries(int width)
{
	if (movePos.x <= 0) movePos.x = 0;
	if (movePos.x >= width - PLATFORM_WIDTH) movePos.x = width - PLATFORM_WIDTH;
}
