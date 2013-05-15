
#include <Windows.h>

#if defined( DEBUG ) || defined( _DEBUG )
#define _CRTDBG_AMP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#endif

#include "CameraController.h"
#include "CubeFactory.h"
#include "DebugGUI.h"
#include "HeightMap.h"
#include "InputHelper.h"
#include "ManagementDebug.h"
#include "XInputFetcher.h"
#include "camera.h"
#include "inputDesc.h"
#include "keyCodes.h"
#include "managementSprite.h"
#include "mathHelper.h"
#include "renderer.h"
#include "utility.h"
#include "window.h"
#include "ObjFileReader.h"

HRESULT initialize(HINSTANCE hInstance, int cmdShow);
void initDebugGui( float* p_dt, float* p_fps );
void handleInput(XInputFetcher* xinput, float dt);
void clean();

Window* g_window;
Renderer* g_renderer;
Camera* g_camera;
ManagementSprite* g_managementSprite;

//Temp: Using whilst testing XML-Loader.
//#include <LoaderXML.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
	//Temp: Using whilst testing XML-Loader.
	//Loader_XML::LoaderXML* loaderXML = new Loader_XML::LoaderXML();
	//loaderXML->init();
	//delete loaderXML;

#if defined( DEBUG ) || defined( _DEBUG )
	_CrtSetDbgFlag ( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
	_CrtSetReportMode ( _CRT_ERROR, _CRTDBG_MODE_DEBUG );
#endif

	AllocConsole();
	HRESULT hr = S_OK;
	hr = initialize(hInstance, cmdShow);
	DebugGUI::getInstance()->init( g_renderer->getD3DManagement(),
		g_window->getWindowHandle() );

	float dt = 0.0f;
	float fps = 0.0f;
	initDebugGui( &dt, &fps );
	XInputFetcher* xinput = new XInputFetcher();
	xinput->calibrate(0.4);

	HeightMap* heightMap = new HeightMap( g_renderer->getD3DManagement() );
	EntityBufferInfo* heightMapBuffers = heightMap->getEntityBufferInfo();
	g_renderer->addEntity( heightMapBuffers );

	ObjFileReader reader;
	EntityBufferInfo* barrel = reader.readFile( "../../resources/",
		"plastic_barrel_scaled.obj", false, g_renderer->getD3DManagement() );
	g_renderer->addEntity( barrel );
	
	CameraController* cameraControl = new CameraController( g_camera, xinput, heightMap );

	if(SUCCEEDED(hr))
	{
		LARGE_INTEGER freq, old, current;
		QueryPerformanceFrequency (&freq);
		QueryPerformanceCounter (&old);

		while(g_window->isActive())
		{
			QueryPerformanceCounter(&current);
			dt = float (current.QuadPart - old.QuadPart) / float(freq.QuadPart);
			fps = 1/dt;

			g_window->checkMessages();

			xinput->update();
			handleInput(xinput, dt);
			cameraControl->update( dt );

			DirectX::XMFLOAT4X4 finalMatrix = MathHelper::multiplyMatrix( 
				g_camera->getViewMatrix(), g_camera->getProjectionMatrix() );

			g_renderer->update( finalMatrix, cameraControl->getPosition() );
			g_renderer->beginRender();
			g_renderer->renderEntities();
			g_renderer->renderSprites(g_managementSprite);
			g_renderer->endRender();

			old.QuadPart = current.QuadPart;

			Sleep(8);
		}
	}

	delete cameraControl;
	delete heightMap;
	delete xinput;

	DebugGUI::getInstance()->terminate();
	

//#define DEBUG_D3D
#ifdef DEBUG_D3D
	ManagementDebug md;
	md.init(g_renderer->getD3DManagement()->getDevice());
#endif // DEBUG_D3D

	clean();

#ifdef DEBUG_D3D
	md.reportLiveObjects();
#endif // !DEBUG_D3D

	return 0;
}

HRESULT initialize(HINSTANCE hInstance, int cmdShow)
{
	HRESULT hr = S_OK;

	g_window = new Window(hInstance, cmdShow);
	hr = g_window->initialize();
	if(SUCCEEDED(hr))
	{
		g_renderer = new Renderer();
		hr = g_renderer->init(g_window->getWindowHandle());
	}
	if(SUCCEEDED(hr))
	{
		g_camera = new Camera();
		g_camera->setLens(DirectX::XM_PIDIV4, static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGHT), 1.0f, 1000.0f);
	}
	if(SUCCEEDED(hr))
	{
		g_managementSprite = new ManagementSprite();
		hr = g_managementSprite->init(g_renderer->getD3DManagement()->getDevice());
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

void handleInput(XInputFetcher* xinput, float dt)
{
	if(xinput->getBtnState(InputHelper::Xbox360Digitals_SHOULDER_PRESS_L) > 0)
		g_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_TEXT_MENU);
	else
		g_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_NONE);
}

void clean()
{
	SAFE_DELETE(g_window);
	SAFE_DELETE(g_renderer);
	SAFE_DELETE(g_camera);
	SAFE_DELETE(g_managementSprite);
}
