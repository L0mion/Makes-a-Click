
#include <Windows.h>

#if defined( DEBUG ) || defined( _DEBUG )
#define _CRTDBG_AMP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "window.h"
#include "renderer.h"
#include "camera.h"
#include "utility.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{

#if defined( DEBUG ) || defined( _DEBUG )
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif

	AllocConsole();

	HRESULT hr = S_OK;

	Window	 window(hInstance, cmdShow);
	Renderer renderer;
	Camera	 camera;

	hr = window.initialize();
	if(SUCCEEDED(hr))
		hr = renderer.init(window.getWindowHandle());

	camera.setLens(DirectX::XM_PIDIV4, static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGHT), 1.0f, 100.0f);
	camera.rebuildView();

	if(SUCCEEDED(hr))
	{
		while(window.isActive())
		{
			window.checkMessages();

			DirectX::XMFLOAT4X4 finalMatrix;
			DirectX::XMFLOAT4X4 view = camera.getViewMatrix();
			DirectX::XMFLOAT4X4 projection = camera.getProjectionMatrix();
			DirectX::XMMATRIX xmView = DirectX::XMLoadFloat4x4(&view);
			DirectX::XMMATRIX xmProjection = DirectX::XMLoadFloat4x4(&projection);
			DirectX::XMMATRIX xmFinal = DirectX::XMMatrixMultiply(xmView, xmProjection);
			DirectX::XMStoreFloat4x4(&finalMatrix, xmFinal);
			
			renderer.update(finalMatrix);
			renderer.render();
		}
	}

	return 0;
}