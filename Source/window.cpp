#include "window.h"
#include "keyCodes.h"
#include "utility.h"
#include "inputDesc.h"

std::vector<bool> Window::keys_;
int Window::mouseDeltaX_;
int Window::mouseDeltaY_;
HWND Window::windowHandle_;

Window::Window(HINSTANCE hInstance, int cmdShow)
{
	hInstance_	= hInstance;
	cmdShow_	= cmdShow;

	active_ = true;

	keys_.resize(KeyCodes::NUM_KEYS, false);
}
Window::~Window()
{
	ReleaseCapture();
}

HRESULT Window::initialize()
{
	HRESULT hr = S_OK;
	hr = createWindow();
	if(SUCCEEDED(hr))
		initializeCursor();

	return hr;
}

void Window::checkMessages()
{
	MSG msg = {0};
	if(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);

		if(msg.message == WM_QUIT)
			active_ = false;
	}
}

bool Window::isActive() const
{
	return active_;
}

HWND Window::getWindowHandle() const
{
	return windowHandle_;
}

InputDesc Window::getInput() const
{
	return InputDesc(keys_, mouseDeltaX_, mouseDeltaY_);
}

HRESULT Window::createWindow()
{
	HRESULT hr = S_OK;

	WNDCLASSEX wc;
	ZeroMemory(&wc, sizeof(wc));

	wc.cbSize		 = sizeof(wc);
	wc.style		 = CS_HREDRAW |CS_VREDRAW;
	wc.lpfnWndProc	 = WindowProc;
	wc.hInstance	 = hInstance_;
	wc.hCursor		 = LoadCursor(NULL, IDC_ARROW);
	wc.lpszClassName = L"WindowClass";

	RegisterClassEx(&wc);

	windowHandle_ = CreateWindowEx(NULL,
								   L"WindowClass",
								   L"Raytracer",
								   WS_OVERLAPPEDWINDOW,
								   100,
								   100,
								   SCREEN_WIDTH,
								   SCREEN_HEIGHT,
								   NULL,
								   NULL,
								   hInstance_,
								   NULL);
	if(!windowHandle_)
	{
		hr = E_FAIL;
		MessageBox(NULL, L"Window could not be created!", L"Window Error!", MB_OK | MB_ICONEXCLAMATION);
	}
	else
		ShowWindow(windowHandle_, cmdShow_);

	return hr;
}

void Window::initializeCursor()
{
	SetCursorPos(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
	SetCapture(windowHandle_);
	ShowCursor(false);
}

void Window::mouseDeltaMove(LPARAM lParam)
{
	//Find the upper left corner of the window's client area in screen coordinates
	POINT point;
	point.x = 0;
	point.y = 0;
	MapWindowPoints(windowHandle_, NULL, &point, 1);
	
	//Get current mouse position
	int mouseX = GET_X_LPARAM(lParam)+point.x;
	int mouseY = GET_Y_LPARAM(lParam)+point.y;

	

	//Calculate relative mouse movement
	mouseDeltaX_ = mouseX - SCREEN_WIDTH/2;
	mouseDeltaY_ = mouseY - SCREEN_HEIGHT/2;

	//Return cursor to screen center
	SetCursorPos(SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
}

LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch(message)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
		break;
	case WM_KEYDOWN:
		if(wParam == VK_ESCAPE)
			DestroyWindow(hWnd);
		keys_[wParam] = true;
		return 0;
		break;
	case WM_KEYUP:
		keys_[wParam] = false;
		return 0;
		break;
	case WM_MOUSEMOVE:
		mouseDeltaMove(lParam);
		return 0;
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}