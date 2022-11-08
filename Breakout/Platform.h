#pragma once
#include <SDL.h>
#include <SDL_image.h>

#define PLATFORM_SPEED 200

struct Vector2D {
	float x;
	float y;
};

class Platform
{
public:
	// Constructors / Destructor
	Platform(SDL_Renderer* renderer, int width, int height);
	~Platform();

	// Functions
	void SpawnPlatform();
	void Move(int width, float deltaTime);
	SDL_Rect GetRectDest();

	// Objects
	Vector2D movePos;
private:
	// Functions
	void CheckBoundaries(int width);

	// Objects
	SDL_Renderer* mRenderer;
	SDL_Texture* mTexture;
	SDL_Rect mDest;

	Vector2D moveVel; // Movement velocity

	int direction;
};

