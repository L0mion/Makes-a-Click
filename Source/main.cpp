
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
#include "ObjFileReader.h"
#include "XInputFetcher.h"
#include "camera.h"
#include "inputDesc.h"
#include "keyCodes.h"
#include "managementMenu.h"
#include "managementSprite.h"
#include "managementWrite.h"
#include "mathHelper.h"
#include "renderer.h"
#include "utility.h"
#include "window.h"
#include "EntityBufferInfo.h"
#include "PivotPoint.h"

//Loading
#include "LoaderMAC.h"
#include "Mac.h"

HRESULT initialize(HINSTANCE hInstance, int cmdShow);
void initDebugGui( float* p_dt, float* p_fps );
void handleInput(XInputFetcher* xinput, float dt);
void clean();

Window* g_window;
Renderer* g_renderer;
Camera* g_camera;
ManagementMenu* g_managementMenu;
CameraController* g_cameraControl;

//Temp: Using whilst testing XML-Loader unt Writer-.
//#include <LoaderXML.h>
//#include <WriterXML.h>

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int cmdShow)
{
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

	//Detta �r kanske typ n�stan helt och h�llet lite sm�tt tempor�rt.
	Mac mac;
	LoaderMAC* loaderMAC = new LoaderMAC();
	bool sucessfulLoad = loaderMAC->init( mac );
	delete loaderMAC;

	PivotPoint* pivot = new PivotPoint( xinput );

	HeightMap* heightMap = new HeightMap(
		g_renderer->getD3DManagement(),
		mac.heightmap,
		(float)std::atof(			mac.macDesc.heightmap.cellSize.c_str()	),	//Jag ska fixa detta. Lovar.
		(float)std::atoi(			mac.macDesc.heightmap.cntCol.c_str()	),	//Jag ska fixa detta. Lovar.
		(unsigned int)std::atoi(	mac.macDesc.heightmap.cntRow.c_str()	));	//Jag ska fixa detta. Lovar.
	EntityBufferInfo* heightMapBuffers = heightMap->getEntityBufferInfo();
	g_renderer->addEntity( heightMapBuffers );

	ObjFileReader reader;
	EntityBufferInfo* blueberry = reader.readFile( "../../resources/",
		"sphere.obj", false, g_renderer->getD3DManagement() );
	g_renderer->addEntity( blueberry );
	
	g_cameraControl = new CameraController( g_camera, xinput, heightMap, pivot );

	g_managementMenu = new ManagementMenu(xinput);
		hr = g_managementMenu->init(g_renderer->getD3DManagement()->getDevice(), 
			g_renderer->getD3DManagement()->getDeviceContext());

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

			heightMap->update( g_renderer->getD3DManagement(), pivot );

			DirectX::XMFLOAT3 pivotPos = pivot->m_position;
			blueberry->m_world._41 = pivotPos.x;
			blueberry->m_world._42 = pivotPos.y;
			blueberry->m_world._43 = pivotPos.z;

			DirectX::XMFLOAT4X4 finalMatrix = MathHelper::multiplyMatrix( 
				g_camera->getViewMatrix(), g_camera->getProjectionMatrix() );

			g_renderer->update( finalMatrix, g_cameraControl->getPosition() );
			g_renderer->beginRender();
			g_renderer->renderEntities();
			g_renderer->renderSprites(g_managementMenu->getManagementSprite());
			g_renderer->renderText(g_managementMenu->getTextStrings());
			g_renderer->endRender();

			g_managementMenu->clearTextStrings();

			old.QuadPart = current.QuadPart;

			//Sleep(8);
		}
	}

	delete g_cameraControl;
	delete heightMap;
	delete xinput;
	delete pivot;

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
	bool menuIsActive = false;
	
	static bool s_textInput		 = false;
	static bool s_alreadyPressed = false;
	static float s_timer		 = 0.0f;

	float leftAnalogSens  = 64.0f;
	float rightAnalogSens = 2.0f;

	double walkDistance		= xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE);
	double strafeDistance	= xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE);
	double yawAngle			= xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_RX_NEGATIVE);
	double pitchAngle		= xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_RY_NEGATIVE);
	
	if(xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_BACK) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(!s_textInput)
			s_textInput = true;
		else
			s_textInput = false;
	}

	if(!s_textInput)
	{
		if(xinput->getBtnState(InputHelper::Xbox360Digitals_SHOULDER_PRESS_L) == InputHelper::KeyStates_KEY_DOWN)
		{
			menuIsActive = true;
			g_managementMenu->useToolsMenu();
		}
		else if(xinput->getBtnState(InputHelper::Xbox360Digitals_SHOULDER_PRESS_R) == InputHelper::KeyStates_KEY_DOWN)
		{
			menuIsActive = true;
			g_managementMenu->useToolPropertiesMenu();
		}
		else
			g_managementMenu->useNoMenu();
		
		if(!menuIsActive)
		{
			g_managementMenu->setSelectedTool();
			g_cameraControl->update( dt );
		}
	}
	else
		g_managementMenu->useTextMenu();
}

void clean()
{
	SAFE_DELETE(g_window);
	SAFE_DELETE(g_renderer);
	SAFE_DELETE(g_camera);
	SAFE_DELETE(g_managementMenu);
}
