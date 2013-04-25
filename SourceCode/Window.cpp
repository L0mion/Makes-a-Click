#include "Window.h"

vector<bool> Window::keys;
int	Window::mouseDeltaX;
int	Window::mouseDeltaY;
HWND Window::hWnd;

Window::Window(HINSTANCE hInstance, int cmdShow)
{
	this->hInstance = hInstance;
	this->cmdShow = cmdShow;

	keys.resize(NUM_KEYS, false);

	SetCursorPos(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
}

Window::~Window()
{
	//Release the cursor back tobthe user
	ReleaseCapture();
}

void Window::mouseDeltaMove(LPARAM lParam)
{
	//Find the upper left corner of the window's client area in screen coordinates
	POINT point;
	point.x = 0;
	point.y = 0;
	MapWindowPoints(hWnd, NULL, &point, 1);
	
	//Get current mouse position
	int mouseX = GET_X_LPARAM(lParam)+point.x;
	int mouseY = GET_Y_LPARAM(lParam)+point.y;

	//Calculate relative mouse movement
	mouseDeltaX = mouseX - SCREEN_WIDTH/2;
	mouseDeltaY = mouseY - SCREEN_HEIGHT/2;

	//Return cursor to screen center
	SetCursorPos(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		return 0;
	}break;

	case WM_KEYDOWN:
	{
		if(wParam == VK_ESCAPE)
		{
			DestroyWindow(hWnd);
		}
		
		keys[wParam] = true;

		return 0;
	}break;

	case WM_KEYUP:
	{
		keys[wParam] = false;

		return 0;
	}break;

	case WM_MOUSEMOVE:
	{
		mouseDeltaMove(lParam);
		return 0;
	}break;
	
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}

void Window::createWindow()
{
	WNDCLASSEX wc;

	ZeroMemory(&wc, sizeof(WNDCLASSEX));

	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = WindowProc;
	wc.hInstance = hInstance;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	hWnd = CreateWindowEx(NULL,
						  L"WindowClass",
						  L"Wonderland",
						  WS_OVERLAPPEDWINDOW,
						  10,
						  10,
						  SCREEN_WIDTH,
						  SCREEN_HEIGHT,
						  NULL,
						  NULL,
						  hInstance,
						  NULL);

	ShowWindow(hWnd, cmdShow);
}

void Window::initCursor()
{
	//center the cursor
	SetCursorPos(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	
	//Capture the cursor to this window
	SetCapture(hWnd);

	//Hide the cursor from the user
	ShowCursor(false);
}

void Window::initialize()
{
	createWindow();
	initCursor();
}

bool Window::checkMessages()
{
	//Returns true if the applications is to be destroyed

	bool quit = false;

	MSG msg = {0};

	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(msg.message == WM_QUIT)
			quit = true;
	}

	return quit;
}

InputContainer Window::getInput()
{
	return InputContainer(keys, mouseDeltaX, mouseDeltaY);

}

HWND Window::getWindowHandle()
{
	return hWnd;
}