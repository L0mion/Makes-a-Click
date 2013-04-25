#include "Game.h"

Game::Game(HINSTANCE hInstance, int cmdShow)
{
	createWindow(hInstance, cmdShow);
	createD3DHandler();
	createShaderManager();
	createSoundManagerManager();
	createObjects();

	velocityModifier = 1;
}

Game::~Game()
{
	delete window;
	delete d3dHandler;
	delete shaderManager;
	delete soundManager;

	delete camera;
	delete terrain;

	for(int i=0; i<cubes.size(); i++)
		delete cubes[i];

}

void Game::createWindow(HINSTANCE hInstance, int cmdShow)
{
	window = new Window(hInstance, cmdShow);
	window->initialize();
}

void Game::createD3DHandler()
{
	d3dHandler = new D3DHandler();
	d3dHandler->initialize(window->getWindowHandle());
}

void Game::createShaderManager()
{
	shaderManager = new ShaderManager(d3dHandler->getDevice(), d3dHandler->getDeviceContext());
	shaderManager->initialize();
}

void Game::createSoundManagerManager()
{
	soundManager = new SoundManager();
	soundManager->initialize();

	/*soundManager->startVoice(SOUND_BACKGROUND_MUSIC);
	soundManager->setSourceVoiceVolume(SOUND_BACKGROUND_MUSIC, 0.05f);

	soundManager->startVoice(SOUND_LASER_SWORD_HUM);
	soundManager->setSourceVoiceVolume(SOUND_LASER_SWORD_HUM, 0.0f);

	soundManager->startVoice(SOUND_TENSION_PIANO_DOOM_LOW);
	soundManager->setSourceVoiceVolume(SOUND_TENSION_PIANO_DOOM_LOW, 0.0f);

	soundManager->startVoice(SOUND_ZOMBIE_EATING);
	soundManager->setSourceVoiceVolume(SOUND_ZOMBIE_EATING, 0.0f);*/
}

void Game::createTerrain()
{
	int numRows = 250;
	int numColumns = 250;

	VertexBehavior *vertexBehavior = new VertexBehaviorHeightmap(d3dHandler->getDevice(), numRows, numColumns, 5, 0.5f, "../../Heightmaps/TerrainHeightmap.raw");
	vertexBehavior->constructVertexBuffer();
	
	IndexBehavior *indexBehavior = new IndexBehaviorGrid(d3dHandler->getDevice(), numRows, numColumns);
	indexBehavior->constructIndexBuffer();

	DrawBehavior *drawBehavior = new DrawBehaviorIndexed(	d3dHandler->getDevice(),
															d3dHandler->getDeviceContext(),
															vertexBehavior->getVertexBuffer(),
															indexBehavior->getIndexBuffer(),
															shaderManager->getVertexShader(),
															shaderManager->getPixelShader(),
															shaderManager->getInputLayout(),
															indexBehavior->getIndexCount());

	terrain = new Terrain(	d3dHandler->getDevice(),
							d3dHandler->getDeviceContext(),
							L"../../Textures/grass.png",
							drawBehavior,
							vertexBehavior,
							indexBehavior);	

	terrain->setTextureScale(D3DXVECTOR3(80.0f, 80.0f, 1.0f));
}

GameObject* Game::createCube(	LPCTSTR textureFilename,
								D3DXVECTOR3 position,
								D3DXVECTOR3 scale,
								D3DXVECTOR3 rotation,
								D3DXVECTOR3 textureScale)
{
	GameObject *cube;

	VertexBehavior *vertexBehavior = new VertexBehaviorCube(d3dHandler->getDevice());
	vertexBehavior->constructVertexBuffer();
	IndexBehavior *indexBehavior = new IndexBehaviorCube(d3dHandler->getDevice());
	indexBehavior->constructIndexBuffer();

	DrawBehavior *drawBehavior = new DrawBehaviorIndexed(	d3dHandler->getDevice(),
															d3dHandler->getDeviceContext(),
															vertexBehavior->getVertexBuffer(),
															indexBehavior->getIndexBuffer(),
															shaderManager->getVertexShader(),
															shaderManager->getPixelShader(),
															shaderManager->getInputLayout(),
															indexBehavior->getIndexCount());

	cube = new GameObject(	d3dHandler->getDevice(),
							d3dHandler->getDeviceContext(),
							textureFilename,
							drawBehavior,
							vertexBehavior,
							indexBehavior);

	cube->setTranslation(position);
	cube->setScale(scale);
	cube->setRotation(rotation);
	cube->setTextureScale(textureScale);

	return cube;
}

void Game::createCubes()
{
	cubes.push_back(createCube(	L"../../Textures/red.png",
								D3DXVECTOR3(500.0f, 45.0f, 500.0f),
								D3DXVECTOR3(2.0f, 2.0f, 2.0f),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXVECTOR3(1.0f, 1.0f, 1.0f)));

	cubes.push_back(createCube(	L"../../Textures/teal.png",
								D3DXVECTOR3(-500.0f, 70.0f, 500.0f),
								D3DXVECTOR3(2.0f, 2.0f, 2.0f),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXVECTOR3(1.0f, 1.0f, 1.0f)));

	cubes.push_back(createCube(	L"../../Textures/blue.png",
								D3DXVECTOR3(500.0f, 85.0f, -500.0f),
								D3DXVECTOR3(2.0f, 2.0f, 2.0f),
								D3DXVECTOR3(0.0f, 0.0f, 0.0f),
								D3DXVECTOR3(1.0f, 1.0f, 1.0f)));
}

void Game::createCamera()
{
	camera = new Camera;
	camera->setLens(PI/4, (float)SCREEN_WIDTH/(float)SCREEN_HEIGHT, 1.0f, 600.f);
	camera->rebuildView();
}

void Game::createLights()
{
	//Parallel Light
	//light.clear();
	//light.dir		= D3DXVECTOR3(0.57735f, -0.57735f, 0.57735f);
	//light.ambient	= D3DXCOLOR(0.2f, 0.2f, 0.2f, 1.0f);
	//light.diffuse	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	//light.specular	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 0.0f);

	//Point Light
	light.pos		= D3DXVECTOR3(0.0f, 350.0f, 0.0f);
	light.ambient	= D3DXCOLOR(0.5f, 0.5f, 0.5f, 1.0f);
	light.diffuse	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.specular	= D3DXCOLOR(1.0f, 1.0f, 1.0f, 1.0f);
	light.att		= D3DXVECTOR3(0.0f, 0.005f, 0.0f);
	light.range		= 1000.0f;
}

void Game::createObjects()
{
	createTerrain();
	createCubes();
	createCamera();
	createLights();
}

void Game::updateSoundVolume(GameObject* object, int voiceIndex)
{
	D3DXVECTOR3 direction = camera->getPosition() - object->getPosition();
	D3DXVECTOR3 look = camera->getLook();
	D3DXVECTOR3 right = camera->getRight();
	
	bool soundFromRight = false;

	float distance = sqrt(direction.x*direction.x + direction.y*direction.y + direction.z*direction.z);
	float lookLength = sqrt(look.x*look.x + look.y*look.y + look.z*look.z);
	float rightLength = sqrt(right.x*right.x + right.y*right.y + right.z*right.z);

	float angle = acos((direction.x*right.x + direction.y*right.y + direction.z*right.z)/(distance*rightLength));
	if(angle < ((float)(PI/2)))
		soundFromRight = true;

	angle = acos((direction.x*look.x + direction.y*look.y + direction.z*look.z)/(distance*lookLength));
	soundManager->setSourceVoicePanning(voiceIndex, angle, soundFromRight);
	soundManager->setSourceVoiceVolumeDistance(voiceIndex, distance);
}


void Game::handleInput(float deltaTime)
{
	float velocity = 40.0f * velocityModifier * deltaTime;

	InputContainer input = window->getInput();

	if(input.keys[VK_W])
		camera->walk(velocity);
	if(input.keys[VK_S])
		camera->walk(-velocity);
	if(input.keys[VK_D])
		camera->strafe(velocity);
	if(input.keys[VK_A])
		camera->strafe(-velocity);
	if(input.keys[VK_Z])
		camera->verticalWalk(velocity);
	if(input.keys[VK_X])
		camera->verticalWalk(-velocity);

	if(input.keys[VK_1])
		velocityModifier = 1;
	if(input.keys[VK_2])
		velocityModifier = 2;
	if(input.keys[VK_3])
		velocityModifier = 3;
	if(input.keys[VK_4])
		velocityModifier = 4;

	float angleX = (input.mouseDeltaX) * 0.001f;
	float angleY = (input.mouseDeltaY) * 0.001f;

	camera->rotateY(angleX);
	camera->pitch(angleY);
}

void Game::update(float deltaTime)
{
	handleInput(deltaTime);

	float height = terrain->getHeight(camera->getPosition().x, camera->getPosition().z)+10.0f;
	camera->setHeight(height);
	camera->rebuildView();

	for(int i=0; i<cubes.size(); i++)
	{
		D3DXVECTOR3 rotation = cubes[i]->getRotation();
		rotation.y += 0.001f;
		cubes[i]->setRotation(rotation);
	}


	/*updateSoundVolume(cubes[0], SOUND_ZOMBIE_EATING);
	updateSoundVolume(cubes[1], SOUND_LASER_SWORD_HUM);
	updateSoundVolume(cubes[2], SOUND_TENSION_PIANO_DOOM_LOW);*/
}

void Game::draw()
{
	d3dHandler->clearRenderTargetsAndZBuffer();

	D3DXMATRIX viewProjection, final; 
	viewProjection = camera->getView()*camera->getProjection();
	shaderManager->updateCBufferLights(light, camera->getPosition());
	

	final = terrain->getWorldMatrix() * viewProjection;
	shaderManager->updateConstantBuffer(terrain->getWorldMatrix(), final, terrain->getTextureScaleMatrix());
	terrain->draw();

	for(int i=0; i<cubes.size(); i++)
	{
		final = cubes[i]->getWorldMatrix() * viewProjection;
		shaderManager->updateConstantBuffer(cubes[i]->getWorldMatrix(), final, cubes[i]->getTextureScaleMatrix());
		cubes[i]->draw();
	}


	d3dHandler->present();
}

void Game::run()
{
	bool gameOver = false;

	LARGE_INTEGER freq, old, current;
	QueryPerformanceFrequency (&freq);
	QueryPerformanceCounter (&old);

	while(!gameOver)
	{
		gameOver = window->checkMessages();

		QueryPerformanceCounter(&current);
		float dt = float (current.QuadPart - old.QuadPart) / float(freq.QuadPart);
		
		update(dt);
		draw();

		old.QuadPart = current.QuadPart;
	}
}