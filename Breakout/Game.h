#pragma once
#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

class Game
{
public:
	// Constructors
	Game();
	~Game();
	// Functions
	bool Initialize();

private:
	// Functions

	// Objects
	SDL_Window *mWindow;
	SDL_Renderer* mRenderer;
};

