#include "Brick.h"
#include <iostream>

Brick::Brick() : mIsHit(false) {
}

Brick::Brick(int rowSpacing, int columnSpacing, const char* Id, SDL_Renderer* renderer, std::string level)
{
	mDoc = new XMLDocument();
	mDoc->LoadFile(level.c_str());

	// Go through every "BrickType" element in the XML file
	dataElement = mDoc->FirstChildElement("Level")->FirstChildElement("BrickTypes");


	for (XMLElement* brickElement = dataElement->FirstChildElement("BrickType");
		brickElement != NULL; brickElement = brickElement->NextSiblingElement())
	{

		if (std::string(brickElement->FirstChildElement("Id")->GetText()) ==
			std::string(Id)) {

			if (std::string(Id) == "I") {
				mHitPoints = 9999;
			}
			else {
				std::stringstream hp(brickElement->FirstChildElement("HitPoints")->GetText());
				hp >> mHitPoints;

				mBreakSound = Mix_LoadWAV(brickElement->FirstChildElement("BreakSound")->GetText());
			}

			mId = Id;

			mIsHit = false;
			
			std::stringstream bs(brickElement->FirstChildElement("BreakScore")->GetText());
			bs >> mBreakScore;

			mTexture = IMG_LoadTexture(renderer, brickElement->FirstChildElement("Texture")->GetText());

			mCurrentHp = mHitPoints;

			mHitSound = Mix_LoadWAV(brickElement->FirstChildElement("HitSound")->GetText());

			mRowSpacing = rowSpacing;
			mColumnSpacing = columnSpacing;

			mRect = { mRowSpacing, mColumnSpacing, BRICK_WIDTH, BRICK_HEIGHT };
		}

	}

	if (std::string(Id) == std::string("E")) {
		mIsHit = true;
	}
}

void Brick::Hit(int& score)
{
	if (mCurrentHp > 0) {
		Mix_PlayChannel(-1, mHitSound, 0);
		mCurrentHp--;
	}
	if (mCurrentHp == 0) {
		Mix_PlayChannel(-1, mBreakSound, 0);
		mIsHit = true;
		score += mBreakScore;
	}

}

void Brick::Destroy()
{
	SDL_DestroyTexture(mTexture); mTexture = nullptr;
	mDoc = nullptr;
	Mix_FreeChunk(mHitSound); mHitSound = nullptr;
	Mix_FreeChunk(mBreakSound); mBreakSound = nullptr;
}


