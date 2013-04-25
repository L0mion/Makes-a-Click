#include <Windows.h>
#include <WindowsX.h>

#include "Utility.h"
#include "Game.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	Game game(hInstance, cmdShow);
	game.run();
	return 0;
}