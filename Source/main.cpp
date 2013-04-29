
#include <Windows.h>

#if defined( DEBUG ) || defined( _DEBUG )
#define _CRTDBG_AMP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "window.h"
#include "renderer.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
#if defined( DEBUG ) || defined( _DEBUG )
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif

	AllocConsole();

	HRESULT hr = S_OK;

	Window window(hInstance, cmdShow);
	Renderer renderer;

	hr = window.initialize();
	if(SUCCEEDED(hr))
		hr = renderer.init(window.getWindowHandle());
	
	if(SUCCEEDED(hr))
	{
		while(window.isActive())
		{
			window.checkMessages();

			renderer.render();
		}
	}

	return 0;
}