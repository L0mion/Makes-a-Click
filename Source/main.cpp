
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
	//Temp: Using whilst testing XML-Loader.
	//Loader_XML::LoaderXML* loaderXML = new Loader_XML::LoaderXML();
	//loaderXML->init();
	//delete loaderXML;

	//Writer_XML::DescMAC descMac("Testing", "Testing", 0);
	//Writer_XML::WriterXML* writerXML 
	//	= new Writer_XML::WriterXML(descMac);
	//writerXML->init();
	//delete writerXML;

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

	PivotPoint* pivot = new PivotPoint( xinput );

	HeightMap* heightMap = new HeightMap( g_renderer->getD3DManagement() );
	EntityBufferInfo* heightMapBuffers = heightMap->getEntityBufferInfo();
	g_renderer->addEntity( heightMapBuffers );

	ObjFileReader reader;
	EntityBufferInfo* blueberry = reader.readFile( "../../resources/",
		"sphere.obj", false, g_renderer->getD3DManagement() );
	g_renderer->addEntity( blueberry );
	
	g_cameraControl = new CameraController( g_camera, xinput, heightMap, pivot );

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

			heightMap->update( g_renderer->getD3DManagement() );

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
	if(SUCCEEDED(hr))
	{
		g_managementMenu = new ManagementMenu();
		hr = g_managementMenu->init(g_renderer->getD3DManagement()->getDevice(), 
			g_renderer->getD3DManagement()->getDeviceContext());
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

	float leftAnalogSens  = 64.0f;
	float rightAnalogSens = 2.0f;

	double walkDistance		= xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE);
	double strafeDistance	= xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE);
	double yawAngle			= xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_RX_NEGATIVE);
	double pitchAngle		= xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_RY_NEGATIVE);
	
	
	if(xinput->getBtnState(InputHelper::Xbox360Digitals_SHOULDER_PRESS_L) > 0)
	{
		menuIsActive = true;
		g_managementMenu->useToolsMenu(strafeDistance, walkDistance);
	}
	else if(xinput->getBtnState(InputHelper::Xbox360Digitals_SHOULDER_PRESS_R) > 0)
	{
		menuIsActive = true;
		g_managementMenu->useToolPropertiesMenu(strafeDistance, walkDistance);
	}
	else
		g_managementMenu->useNoMenu();
	
	if(!menuIsActive)
	{
		g_managementMenu->setSelectedTool();
		g_cameraControl->update( dt );
	}
}

void clean()
{
	SAFE_DELETE(g_window);
	SAFE_DELETE(g_renderer);
	SAFE_DELETE(g_camera);
	SAFE_DELETE(g_managementMenu);
}
