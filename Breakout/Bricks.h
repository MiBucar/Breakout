#pragma once
#include <iostream>
#include <vector>
#include <Brick.h>

class Bricks
{
public:
	~Bricks();
	// Functions
	void Init(SDL_Renderer* renderer, int topMargin, std::string level);
	std::vector<Brick>* GetBricksRef() { return &brickArr; };
	std::vector<Brick> GetBricks() { return brickArr; };
private:
	XMLDocument *mDoc;

	std::vector<Brick> brickArr;

	int mTopMargin;
};


