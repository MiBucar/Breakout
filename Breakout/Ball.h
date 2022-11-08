#pragma once
#include "Platform.h"
#include <Brick.h>
#include <Bricks.h>
#include <Level.h>

#define BALL_SPEED 500

class Ball
{
public:
	// Constructors
	Ball(SDL_Renderer* renderer);
	~Ball();

	// Functions
	void RenderBall();
	void MoveBall(float deltaTime, int screenWidth, int screenHeight, SDL_Rect paddle, std::vector<Brick>* bricks, int topMargin, Level& level);
	void RespawnBall();
	// Objects
	int mScore = 0;

private:
	// Functions
	float GetReflection(float hitX, SDL_Rect paddleRect);
	float GetYSize(Brick brick);
	float GetXSize(Brick brick);
	void SpawnBall();
	void SetDirection(float dirX, float dirY);
	void CheckWallCollision(int screenWidth, int topMargin, int screenHeight, Level& level);
	void BrickCollision(std::vector<Brick>* bricks);
	void BrickInteraction(int dirI);
	void CheckPaddleCollision(SDL_Rect paddleRect);
	
	// Objects
	SDL_Texture* mTexture;
	SDL_Renderer* mRenderer;
	SDL_Rect ballRect;
	SDL_Rect intersectRect;
	Mix_Chunk* mHitSound;

	Vector2D movePos;
	Vector2D moveDir;

	int mDirection = 0;
};

