#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <SDL_mixer.h>
#include "tinyxml2-master/tinyxml2.h"
#include <sstream>
#include <thread>
using namespace tinyxml2;

#define BRICK_WIDTH 80
#define BRICK_HEIGHT 20

class Brick
{
public:
	Brick();
	Brick(int rowSpacing, int columnSpacing, const char* Id, SDL_Renderer* renderer, std::string level);

	// Functions
	void Hit(int& score);
	void Destroy();
	bool IsHit() { return mIsHit; };
	int GetHealth() { return mCurrentHp; };
	int GetBreakScore() { return mBreakScore; };
	SDL_Texture* GetTexture() { return mTexture; };
	std::string GetId() { return mId; };
	// Objects
	SDL_Rect mRect{};
private:
	Mix_Chunk* mHitSound;
	Mix_Chunk* mBreakSound;
	SDL_Texture* mTexture;

	XMLElement* dataElement;
	XMLDocument* mDoc;

	bool mIsHit;
	int mHitPoints;
	int mBreakScore;
	int mCurrentHp;
	int mRowSpacing, mColumnSpacing;

	std::string mId;
};

