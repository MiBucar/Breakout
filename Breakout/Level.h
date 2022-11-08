#pragma once
#include "Bricks.h"
#include <algorithm>
#include <SDL_ttf.h>

class Level
{
public:
	// Constructors
	Level(SDL_Renderer* renderer);
	~Level();

	// Functions
	void Render(int width, int height, float& paddleX, int score);
	void RenderMenu(int width, int height, std::string text, float& paddleX, int& score);
	void DecreaseHealth() { mHp--; };
	void SetLevelNotDone() { mNextLevel = false; };
	// Getters
	int GetMargin() { return mTopMargin; };
	int GetLevel() { return mLevelN; };
	int GetHp() { return mHp; };
	int GetMaxLevel() { return LEVEL_WIN; };
	std::vector<Brick>* GetBricks() { return mBricks.GetBricksRef(); };
	bool GameStarted() { return mGameStarted; };
	bool LevelDone() { return mNextLevel; };

private:
	// Rendering Functions
	void RenderBricks(float &paddleX, float screenWidth);
	void RenderBackground(int width, int height);
	void RenderTopWall();
	void RenderText(int score);
	void RenderMenuText(int width, int height, std::string text, int score);

	// Init functions
	void InitText();

	// Game Functions
	void RestartGame(float& paddleX, float screenWidth);
	int CheckIsHit(std::vector<Brick> brickArr);

	// Objects
	enum text {
		SCORE,
		SCORE_VALUE,
		HIGHSCORE,
		HIGHSCORE_VALUE,
		LEVEL,
		LEVEL_VALUE,
		HP,
		HP_VALUE,
		MENU,
		PLAY_AGAIN,
		EMPTY
	};

	enum level {
		LEVEL_ONE,
		LEVEL_TWO,
		LEVEL_THREE,
		LEVEL_WIN,
		LEVEL_EMPTY
	};

	SDL_Renderer* mRenderer;
	SDL_Surface* mTextSurface[EMPTY];
	SDL_Texture* mBackground;
	SDL_Texture* mTopWall;
	SDL_Texture* mTextTexture[EMPTY];
	SDL_Rect mSrc;
	SDL_Rect mDest;
	SDL_Color mWhite;
	TTF_Font* mFont;

	XMLDocument mDoc;

	Bricks mBricks;

	int mTopMargin = 100;
	int mLevelN = 0;
	int mHp;
	int mScore;
	int mHighScore = 0;
	bool mGameStarted = false;
	bool mNextLevel = false;

	std::string mText[EMPTY];
	std::string levelArr[LEVEL_EMPTY];
};

