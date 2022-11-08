#include "Bricks.h"
#include <iostream>

Bricks::~Bricks()
{
	for (int i = 0; i < brickArr.size(); i++) {
		brickArr[i].Destroy();
	}
	mDoc = nullptr;
}

void Bricks::Init(SDL_Renderer* renderer, int topMargin, std::string level)
{
	brickArr.clear();

	mDoc = new XMLDocument();
	mDoc->LoadFile(level.c_str());

	mTopMargin = topMargin;

	int rows;
	std::stringstream row(mDoc->FirstChildElement("Level")->FirstChildElement("RowCount")->GetText());
	row >> rows;

	int rowSpacing;
	int rowT;
	std::stringstream rowS(mDoc->FirstChildElement("Level")->FirstChildElement("RowSpacing")->GetText());
	rowS >> rowSpacing;
	rowT = rowSpacing;

	int columns;
	std::stringstream col(mDoc->FirstChildElement("Level")->FirstChildElement("ColumnCount")->GetText());
	col >> columns;

	const char* brickP = mDoc->FirstChildElement("Level")->FirstChildElement("Bricks")->GetText();
	std::string brickPlan = brickP;

	// Remove all the unnecessary blanks from the brickPlan
	std::string temp = "";
	for (int i = 0; i < brickPlan.size(); ++i) {
		if ((brickPlan[i] >= 'a' && brickPlan[i] <= 'z') || (brickPlan[i] >= 'A' && brickPlan[i] <= 'Z')) {
			temp = temp + brickPlan[i];
		}
	}
	brickPlan = temp;
	int index = 0;

	// Create bricks based on rows, columns and spacings
	for (int i = 0; i < rows; i++) {

		int columnsSpacing;
		int colT;
		std::stringstream colS(mDoc->FirstChildElement("Level")->FirstChildElement("ColumnSpacing")->GetText());
		colS >> columnsSpacing;
		colT = columnsSpacing;

		for (int j = 0; j < columns; j++) {
			Brick brick(columnsSpacing ,rowSpacing + topMargin, std::string(1, brickPlan[index]).c_str(), renderer, level);
			brickArr.push_back(brick);
			columnsSpacing = BRICK_WIDTH + columnsSpacing + colT;
			index++;
		}
		rowSpacing = BRICK_HEIGHT + rowSpacing + rowT;
	}
}






