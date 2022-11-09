#include "Level.h"

Level::Level(SDL_Renderer* renderer) : mRenderer(renderer), mScore(0), mHp(3), mSrc{ 0, 0, 0, 0 }, mDest{ 0, 0, 0, 0 }, mWhite{ 255, 255, 255 } {
	TTF_Init();

	levelArr[LEVEL_ONE] = "levels//Level1.xml";
	levelArr[LEVEL_TWO] = "levels//Level2.xml";
	levelArr[LEVEL_THREE] = "levels//Level3.xml";
	levelArr[LEVEL_WIN] = "levels//Level3.xml";
	
	mDoc.LoadFile("levels//Level1.xml");

	mBackground = IMG_LoadTexture(mRenderer, mDoc.FirstChildElement("Level")->FirstChildElement("BackgroundTexture")->GetText());

	mTopWall = IMG_LoadTexture(mRenderer, "images//TopWall.png");

	mFont = TTF_OpenFont("font//Jersey.ttf", 24);

	mBricks.Init(mRenderer, mTopMargin, levelArr[mLevelN]);

	InitText();
}

Level::~Level()
{
	SDL_DestroyTexture(mBackground); mBackground = nullptr;
	SDL_DestroyTexture(mTopWall); mTopWall = nullptr;
	SDL_DestroyRenderer(mRenderer); mRenderer = nullptr;

	for (int i = 0; i < EMPTY; i++) {
		SDL_FreeSurface(mTextSurface[i]); mTextSurface[i] = nullptr;
		SDL_DestroyTexture(mTextTexture[i]); mTextTexture[i] = nullptr;
	}

	mFont = nullptr;
	TTF_CloseFont(mFont);
}

// Main rendering function
void Level::Render(int width, int height, float& paddleX, int score)
{
	RenderBackground(width, height);
	RenderTopWall();
	RenderText(score);
	RenderBricks(paddleX, width);
}

void Level::RenderBackground(int width, int height)
{
	SDL_Rect bck{
		0, 0, width, height
	};

	SDL_RenderCopy(mRenderer, mBackground, &bck, &bck);
}

void Level::RenderTopWall()
{
	mSrc.w = 1280;
	mSrc.h = 100;

	SDL_Rect wall{
		0, 0, mSrc.w, mSrc.h
	};

	SDL_RenderCopy(mRenderer, mTopWall, &mSrc, &wall);
}

void Level::RenderText(int score)
{
	mSrc.w = 100;
	mSrc.h = 30;
	mDest.w = 100;
	mDest.h = 30;

	// SCORE
	mDest.x = 25;
	mDest.y = 20;
	SDL_RenderCopy(mRenderer, mTextTexture[SCORE], &mSrc, &mDest);

	// LEVEL
	mDest.x = 1280 / 2 - 50;
	SDL_RenderCopy(mRenderer, mTextTexture[LEVEL], &mSrc, &mDest);

	// HP
	mDest.x = 1280 - 125;
	SDL_RenderCopy(mRenderer, mTextTexture[HP], &mSrc, &mDest);

	mSrc.w = 30;
	mSrc.h = 25;
	mDest.w = 30;
	mDest.h = 25;

	// Initialize score, hp and level value here because those have to update every frame
	// SCORE VALUE
	mDest.x = 25;
	mDest.y = 50;
	mText[SCORE_VALUE] = std::to_string(score);
	mTextSurface[SCORE_VALUE] = TTF_RenderText_Solid(mFont, mText[SCORE_VALUE].c_str(), mWhite);
	mTextTexture[SCORE_VALUE] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[SCORE_VALUE]);
	SDL_RenderCopy(mRenderer, mTextTexture[SCORE_VALUE], &mSrc, &mDest);

	// HP VALUE
	mDest.x = 1280 - 55;
	mDest.y = 50;
	mText[HP_VALUE] = std::to_string(mHp);
	mTextSurface[HP_VALUE] = TTF_RenderText_Solid(mFont, mText[HP_VALUE].c_str(), mWhite);
	mTextTexture[HP_VALUE] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[HP_VALUE]);
	SDL_RenderCopy(mRenderer, mTextTexture[HP_VALUE], &mSrc, &mDest);

	// LEVEL VALUE
	mDest.x = 1280 / 2 - mDest.w / 2;
	mDest.y = 50;
	mText[LEVEL_VALUE] = std::to_string(mLevelN + 1);
	mTextSurface[LEVEL_VALUE] = TTF_RenderText_Solid(mFont, mText[LEVEL_VALUE].c_str(), mWhite);
	mTextTexture[LEVEL_VALUE] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[LEVEL_VALUE]);
	SDL_RenderCopy(mRenderer, mTextTexture[LEVEL_VALUE], &mSrc, &mDest);
}

void Level::RenderBricks(float& paddleX, float screenWidth)
{
	// Render every brick and if its hp reaches 0, remove it
	auto brickArr = mBricks.GetBricksRef();

	std::for_each(brickArr->begin(), brickArr->end(), [this](Brick brick) {

		if (brick.IsHit()) {
			brick.mRect.w = 0;
			brick.mRect.h = 0;
		}

		mSrc.w = 80;
		mSrc.h = 20;

		SDL_RenderCopy(mRenderer, brick.GetTexture(), &mSrc, &brick.mRect);
		});

	// Change the level if every hittable brick has been destroyed
	auto brickArr2 = mBricks.GetBricks();
	if (CheckIsHit(brickArr2) == brickArr2.size()) {
		mLevelN++;
		mNextLevel = true;
		mBricks.Init(mRenderer, mTopMargin, levelArr[mLevelN]);
		paddleX = (screenWidth / 2) - 40;
	}
}

void Level::RenderMenu(int width, int height, std::string text, float& paddleX, int& score)
{
	RenderBackground(width, height);
	RenderMenuText(width, height, text, score);

	const Uint8* state = SDL_GetKeyboardState(NULL);
	if (state[SDL_SCANCODE_SPACE]) {
		if (mGameStarted == false) mGameStarted = true;

		mLevelN = 0;
		if (mLevelN == 0) {
			score = 0;
			mBricks.Init(mRenderer, mTopMargin, levelArr[mLevelN]);
		}

		if (mHp == 0) {
			RestartGame(paddleX, width);
		}
	}
}

void Level::RenderMenuText(int width, int height, std::string text, int score)
{
	mSrc.w = 400;
	mSrc.h = 100;
	mDest.w = 400;
	mDest.h = 100;
	mDest.x = width / 2 - mDest.w / 2;
	mDest.y = height / 2 - mDest.h / 2 - 75;
	mText[MENU] = text;
	mTextSurface[MENU] = TTF_RenderText_Solid(mFont, mText[MENU].c_str(), mWhite);
	mTextTexture[MENU] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[MENU]);
	SDL_RenderCopy(mRenderer, mTextTexture[MENU], &mSrc, &mDest);

	mSrc.w = 600;
	mSrc.h = 50;
	mDest.w = 600;
	mDest.h = 50;
	mDest.x = width / 2 - mDest.w / 2;
	mDest.y = height / 2 - mDest.h / 2 + 75;

	if (mGameStarted == false) {
		mText[PLAY_AGAIN] = "Press space to play";
	}
	else {
		mText[PLAY_AGAIN] = "Press space to play again";
	}

	mTextSurface[PLAY_AGAIN] = TTF_RenderText_Solid(mFont, mText[PLAY_AGAIN].c_str(), mWhite);
	mTextTexture[PLAY_AGAIN] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[PLAY_AGAIN]);
	SDL_RenderCopy(mRenderer, mTextTexture[PLAY_AGAIN], &mSrc, &mDest);
}

void Level::InitText()
{
	mText[SCORE] = "Score";
	mText[LEVEL] = "Level";
	mText[HP] = "Health";

	mTextSurface[SCORE] = TTF_RenderText_Solid(mFont, mText[SCORE].c_str(), mWhite);
	mTextSurface[LEVEL] = TTF_RenderText_Solid(mFont, mText[LEVEL].c_str(), mWhite);
	mTextSurface[HP] = TTF_RenderText_Solid(mFont, mText[HP].c_str(), mWhite);

	mTextTexture[SCORE] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[SCORE]);
	mTextTexture[LEVEL] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[LEVEL]);
	mTextTexture[HP] = SDL_CreateTextureFromSurface(mRenderer, mTextSurface[HP]);
}

void Level::RestartGame(float& paddleX, float screenWidth)
{
	mHp = 4;
	mBricks.Init(mRenderer, mTopMargin, levelArr[mLevelN]);
	paddleX = (screenWidth / 2) - 40;
}

// Check for bricks that have been hit
int Level::CheckIsHit(std::vector<Brick> brickArr)
{
	int isHitCounter = 0;
	int everyoneHit = 0;

	for (int i = 0; i < brickArr.size(); i++) {
		if (brickArr[i].IsHit() || std::string(brickArr[i].GetId()) == "I") {
			isHitCounter += 1;
		}
	}
	everyoneHit = isHitCounter;
	return everyoneHit;
}




