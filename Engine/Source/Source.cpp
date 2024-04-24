#include <iostream>
#include "Game.h"
using namespace std;

int main(int argc, char* argv[]) {
	
	Game::GetGame()->Run();
	
	Game::DestroyGame();

	return 0;

}