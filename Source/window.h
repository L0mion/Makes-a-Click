#ifndef WINDOW_H
#define WINDOW_H

#include <vector>

#include <Windows.h>
#include <windowsx.h>

struct InputDesc;

class Window
{
public:
	Window(HINSTANCE hInstance, int cmdShow);
	~Window();

	HRESULT initialize();
	void checkMessages();

	bool isActive() const;

	HWND getWindowHandle() const;
	InputDesc getInput() const;

private:
	HRESULT createWindow();
	void initializeCursor();

	static void mouseDeltaMove(LPARAM lParam);
	static LRESULT CALLBACK WindowProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

	static std::vector<bool> keys_;
	static int mouseDeltaX_;
	static int mouseDeltaY_;

	static HWND windowHandle_;

	bool active_;

	HINSTANCE hInstance_;
	int cmdShow_;
};

#endif //WINDOW_H