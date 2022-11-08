#include "Ball.h"
#include <iostream>

int BALL_WIDTH = 20;
int BALL_HEIGHT = 20;

Ball::Ball(SDL_Renderer* renderer) : mRenderer(renderer), ballRect{ 0, 0, 0, 0}, intersectRect{ 0, 0, 0, 0}, movePos{300, 360}
{
	Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);
	mTexture = IMG_LoadTexture(mRenderer, "images//Ball.png");
	mHitSound = Mix_LoadWAV("sounds//BrickHit.wav");

	SetDirection(1, 1);

	SpawnBall();
}

Ball::~Ball()
{
	SDL_DestroyRenderer(mRenderer); mRenderer = nullptr;
	SDL_DestroyTexture(mTexture); mTexture = nullptr;
	Mix_FreeChunk(mHitSound); mHitSound = nullptr;
	Mix_Quit();
}

// Functions
void Ball::SpawnBall()
{
	ballRect.x = (int)(movePos.x + 0.5f);
	ballRect.y = (int)(movePos.y + 0.5f);
	ballRect.w = BALL_WIDTH;
	ballRect.h = BALL_HEIGHT;
}

void Ball::RespawnBall()
{
	movePos.x = 300;
	movePos.y = 360;
	SetDirection(1, 1);
}

void Ball::RenderBall()
{
	SpawnBall();

	SDL_Rect src{
	0, 0, BALL_WIDTH, BALL_HEIGHT
	};

	SDL_RenderCopy(mRenderer, mTexture, &src, &ballRect);
}

void Ball::MoveBall(float deltaTime, int screenWidth, int screenHeight, SDL_Rect paddleRect, std::vector<Brick>* bricks, int topMargin, Level& level)
{
	// Collisions
	CheckWallCollision(screenWidth, topMargin, screenHeight, level);
	CheckPaddleCollision(paddleRect);
	BrickCollision(bricks);

	// Respawn the ball after completing the level
	if (level.LevelDone()) {
		RespawnBall();
		level.SetLevelNotDone();
	}

	movePos.y += deltaTime * moveDir.y;
	movePos.x += deltaTime * moveDir.x;
}

void Ball::CheckWallCollision(int screenWidth, int topMargin, int screenHeight, Level& level)
{
	float ballCenter = ballRect.x + ballRect.w / 2.0f;

	if (movePos.y < topMargin) {
		Mix_PlayChannel(-1, mHitSound, 0);
		movePos.y = static_cast<float>(topMargin + 1); // +1 to make sure ball doesnt stay over the topMargin due to the speed
		moveDir.y *= -1;
	}
	else if (ballRect.y >= screenHeight) {
		RespawnBall();
		level.DecreaseHealth();
		if (level.GetHp() == 0) {
			mScore = 0;
		}
	}

	if (movePos.x <= 0) {
		Mix_PlayChannel(-1, mHitSound, 0);
		movePos.x = 1;
		moveDir.x *= -1; // -1 to make sure ball doesnt stay over the left wall
	}
	else if (movePos.x + ballRect.w >= screenWidth) {
		Mix_PlayChannel(-1, mHitSound, 0);
		movePos.x = movePos.x - 1; // -1 to make sure ball doesnt stay over the right wall
		moveDir.x *= -1;
	}
}

void Ball::CheckPaddleCollision(SDL_Rect paddleRect)
{
	float ballCenter = ballRect.x + ballRect.w / 2.0f;

	if (ballRect.x + ballRect.w > paddleRect.x && ballRect.x < paddleRect.x + paddleRect.w &&
		ballRect.y + ballRect.h > paddleRect.y && ballRect.y < paddleRect.y + paddleRect.h) {

		ballRect.y = paddleRect.y - ballRect.h;
		SetDirection(GetReflection(ballCenter - paddleRect.x, paddleRect), -1);
	}
}

void Ball::BrickCollision(std::vector<Brick>* bricks)
{
	std::for_each(bricks->begin(), bricks->end(), [this](Brick& brick) {
		if (SDL_IntersectRect(&ballRect, &brick.mRect, &intersectRect) && !brick.IsHit()) {

			float ballCenterX = ballRect.x + (ballRect.w / 2.0f);
			float ballCenterY = ballRect.y + (ballRect.h / 2.0f);

			float brickCenterX = brick.mRect.x + (brick.mRect.w / 2.0f);
			float brickCenterY = brick.mRect.y + (brick.mRect.h / 2.0f);

			if (brick.IsHit() == false) {
				brick.Hit(mScore);
			}
			
			if (ballRect.x <= brick.mRect.x + brick.mRect.w && ballRect.x + ballRect.w >= brick.mRect.x
				&& ballRect.y <= brick.mRect.y + brick.mRect.h && ballRect.y + ballRect.h >= brick.mRect.y) {

				float ySize = GetYSize(brick);
				float xSize = GetXSize(brick);

				// DO BRICK RESPONSE

				if (xSize > ySize) {
					if (ballCenterY > brickCenterY) {
						ballRect.y += ySize + 0.01f;
						BrickInteraction(3);
					}
					else {
						ballRect.y -= ySize + 0.01f;
						BrickInteraction(1);
					}
				}
				else {
					if (ballCenterX < brickCenterX) {
						ballRect.x -= xSize + 0.01f;
						BrickInteraction(0);
					}
					else {
						ballRect.x += xSize + 0.01f;
						BrickInteraction(2);
					}
				}

				return;
			}
		}
		});
}

float Ball::GetYSize(Brick brick)
{
	float yMin = 0;
	if (brick.mRect.y > ballRect.y) {
		yMin = brick.mRect.y;
	}
	else {
		yMin = ballRect.y;
	}

	float yMax = 0;
	if (brick.mRect.y + brick.mRect.h < ballRect.y + ballRect.h) {
		yMax = brick.mRect.y + brick.mRect.h;
	}
	else {
		yMax = ballRect.y + ballRect.h;
	}
	return yMax - yMin;
}

float Ball::GetXSize(Brick brick)
{
	float xMin = 0;
	if (brick.mRect.x > ballRect.x) {
		xMin = brick.mRect.x;
	}
	else {
		xMin = ballRect.x;
	}

	float xMax = 0;
	if (brick.mRect.x + brick.mRect.w < ballRect.x + ballRect.w) {
		xMax = brick.mRect.x + brick.mRect.w;
	}
	else {
		xMax = ballRect.x + ballRect.w;
	}

	return xMax - xMin;
}

void Ball::BrickInteraction(int dirI)
{
	int dirX = 1;
	int dirY = 1;

	if (moveDir.x > 0) {
		if (moveDir.y > 0) {
			if (dirI == 0 || dirI == 3) {
				dirX = -1;
			}
			else {
				dirY = -1;
			}
		}
		else if (moveDir.y < 0) {
			if (dirI == 0 || dirI == 1) {
				dirX = -1;
			}
			else {
				dirY = -1;
			}
		}
	}
	else if (moveDir.x < 0) {
		// Ball is moving in the negative x direction
		if (moveDir.y > 0) {
			// Ball is moving in the positive y direction
			// -1 +1
			if (dirI == 2 || dirI == 3) {
				dirX = -1;
			}
			else {
				dirY = -1;
			}
		}
		else if (moveDir.y < 0) {
			// Ball is moving in the negative y direction
			// -1 -1
			if (dirI == 1 || dirI == 2) {
				dirX = -1;
			}
			else {
				dirY = -1;
			}
		}
	}
	SetDirection(dirX * moveDir.x, dirY * moveDir.y);
}

float Ball::GetReflection(float hitX, SDL_Rect paddleRect)
{
	if (hitX < 0) {
		hitX = 0;
	}
	else if (hitX > paddleRect.w) {
		hitX = paddleRect.w;
	}

	hitX -= paddleRect.w / 2;

	return 2.0f * (hitX / (paddleRect.w / 2));
}

void Ball::SetDirection(float dirX, float dirY) {
	float length = sqrt(dirX * dirX + dirY * dirY);
	moveDir.x = BALL_SPEED * (dirX / length);
	moveDir.y = BALL_SPEED * (dirY / length);
}




