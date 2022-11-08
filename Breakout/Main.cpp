#include "Game.h"

int main(int argc, char* args[]) {
	Game myGame;

	if (myGame.Initialize()) {
		myGame.RunLoop();
	}
	myGame.Shutdown();

	return 0; 
}