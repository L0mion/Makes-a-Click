
#include <Windows.h>

#if defined( DEBUG ) || defined( _DEBUG )
#define _CRTDBG_AMP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "DebugGUI.h"
#include "HeightMap.h"
#include "XInputFetcher.h"
#include "camera.h"
#include "inputDesc.h"
#include "keyCodes.h"
#include "mathHelper.h"
#include "renderer.h"
#include "utility.h"
#include "window.h"
#include "CubeFactory.h"

HRESULT initialize(HINSTANCE hInstance, int cmdShow);
void initDebugGui( float* p_dt, float* p_fps );
void handleInput(InputDesc inputDesc, float dt);
void clean();

Window* window;
Renderer* renderer;
Camera* camera;

//Temp: Using whilst testing XML-Loader.
//#include <LoaderXML.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	//Temp: Using whilst testing XML-Loader.
	//LoaderXML* loaderXML = new LoaderXML();
	//loaderXML->init();

#if defined( DEBUG ) || defined( _DEBUG )
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif

	AllocConsole();
	HRESULT hr = S_OK;
	hr = initialize(hInstance, cmdShow);
	DebugGUI::getInstance()->init( renderer->getD3DManagement(),
		window->getWindowHandle() );

	float dt = 0.0f;
	float fps = 0.0f;
	initDebugGui( &dt, &fps );
	XInputFetcher* xinput = new XInputFetcher();

	HeightMap* heightMap = new HeightMap( renderer->getD3DManagement() );
	EntityBufferInfo* heightMapBuffers = heightMap->getEntityBufferInfo();
	renderer->addEntity( heightMapBuffers );

	EntityBufferInfo* cube = NULL;
	CubeFactory::createCube( renderer->getD3DManagement(), &cube );
	renderer->addEntity( cube );

	if(SUCCEEDED(hr))
	{
		LARGE_INTEGER freq, old, current;
		QueryPerformanceFrequency (&freq);
		QueryPerformanceCounter (&old);

		while(window->isActive())
		{
			QueryPerformanceCounter(&current);
			dt = float (current.QuadPart - old.QuadPart) / float(freq.QuadPart);
			fps = 1/dt;

			window->checkMessages();
			InputDesc inputDesc = window->getInput();

			handleInput(inputDesc, dt);
			camera->rebuildView();

			DirectX::XMFLOAT4X4 finalMatrix = MathHelper::multiplyMatrix(camera->getViewMatrix(), camera->getProjectionMatrix());

			xinput->update();
			renderer->update(finalMatrix);
			renderer->beginRender();
			renderer->renderEntities();
			renderer->renderHeightMap( heightMap );
			renderer->renderSprites();
			renderer->endRender();

			old.QuadPart = current.QuadPart;

			Sleep(8);
		}
	}

	delete heightMap;
	delete xinput;

	DebugGUI::getInstance()->terminate();

	clean();
	return 0;
}

HRESULT initialize(HINSTANCE hInstance, int cmdShow)
{
	HRESULT hr = S_OK;

	window = new Window(hInstance, cmdShow);
	hr = window->initialize();
	if(SUCCEEDED(hr))
	{
		renderer = new Renderer();
		hr = renderer->init(window->getWindowHandle());
	}
	if(SUCCEEDED(hr))
	{
		camera = new Camera();
		camera->setLens(DirectX::XM_PIDIV4, static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGHT), 1.0f, 100.0f);
		camera->rebuildView();
	}

	return hr;
}

void initDebugGui( float* p_dt, float* p_fps )
{
	int answerToLifeTheUniverseAndEverything = 42;
	DebugGUI::getInstance()->addVar( "Main", DebugGUI::Types_INT,
		DebugGUI::Permissions_READ_ONLY,
		"Answer to life, the universe and everything",
		&answerToLifeTheUniverseAndEverything, "");

	DebugGUI::getInstance()->addVar( "Main", DebugGUI::Types_FLOAT,
		DebugGUI::Permissions_READ_ONLY, "dt", p_dt, "");

	DebugGUI::getInstance()->addVar( "Main", DebugGUI::Types_FLOAT,
		DebugGUI::Permissions_READ_ONLY, "fps", p_fps, "");
}

void handleInput(InputDesc inputDesc, float dt)
{
	float distance = 10.0f * dt;
	if(inputDesc.keys_[KeyCodes::VK_W])
		camera->walk(distance);
	if(inputDesc.keys_[KeyCodes::VK_S])
		camera->walk(-distance);
	if(inputDesc.keys_[KeyCodes::VK_A])
		camera->strafe(-distance);
	if(inputDesc.keys_[KeyCodes::VK_D])
		camera->strafe(distance);
	if(inputDesc.keys_[KeyCodes::VK_X])
		camera->verticalWalk(distance);
	if(inputDesc.keys_[KeyCodes::VK_Z])
		camera->verticalWalk(-distance);

	camera->yaw(inputDesc.mouseDeltaX_ * dt);
	camera->pitch(inputDesc.mouseDeltaY_ * dt);
}

void clean()
{
	SAFE_DELETE(window);
	SAFE_DELETE(renderer);
	SAFE_DELETE(camera);
}
