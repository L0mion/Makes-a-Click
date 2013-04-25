#ifndef WINDOW_H
#define WINDOW_H

#include <Windows.h>
#include <WindowsX.h>

#include <vector>

#include "Utility.h"
#include "InputContainer.h"


using namespace std;

class Window
{
private:
	static vector<bool> keys;
	static int mouseDeltaX;
	static int mouseDeltaY;

	static HWND hWnd;
	HINSTANCE hInstance;
	int cmdShow;

	void createWindow();
	void initCursor();

	static void mouseDeltaMove(LPARAM lParam);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

public:
	Window(HINSTANCE hInstance, int cmdShow);
	~Window();

	void initialize();
	
	bool checkMessages();
	InputContainer getInput();
	HWND getWindowHandle();
};

#endif