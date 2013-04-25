#ifndef GAME_H
#define GAME_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include <Windows.h>
#include <WindowsX.h>

#include <math.h>
#include <vector>

#include "Utility.h"
#include "Terrain.h"
#include "Camera.h"
#include "Window.h"
#include "InputContainer.h"
#include "D3DHandler.h"
#include "ShaderManager.h"
#include "Light.h"
#include "SoundManager.h"

#include "DrawBehaviorIndexed.h"
#include "VertexBehaviorHeightmap.h"
#include "VertexBehaviorCube.h"
#include "IndexBehaviorGrid.h"
#include "IndexBehaviorCube.h"

using namespace std;

class Game
{
private:
	Window *window;
	D3DHandler *d3dHandler;
	ShaderManager *shaderManager;
	SoundManager *soundManager;

	Camera *camera;
	Terrain *terrain;
	
	
	vector<GameObject*> cubes;

	Light light;

	float velocityModifier;

	void createWindow(HINSTANCE hInstance, int cmdShow);
	void createD3DHandler();
	void createShaderManager();
	void createSoundManagerManager();
	void createTerrain();
	GameObject* createCube(	LPCTSTR textureFilename,
							D3DXVECTOR3 position,
							D3DXVECTOR3 scale,
							D3DXVECTOR3 rotation,
							D3DXVECTOR3 textureScale);
	void createCubes();
	void createCamera();
	void createLights();
	void createObjects();

	void updateSoundVolume(GameObject* object, int voiceIndex);

public:
	Game(HINSTANCE hInstance, int cmdShow);
	~Game();

	void handleInput(float deltaTime);
	void update(float deltaTime);
	void draw();

	void run();
};

#endif