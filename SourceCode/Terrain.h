#ifndef TERRAIN_H
#define TERRAIN_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include <fstream>
#include <vector>

#include "Utility.h"
#include "Vertex.h"

#include "CBuffer.h"

#include "GameObject.h"

using namespace std;

class Terrain : public GameObject
{
private:

public:

	Terrain(	ID3D11Device *device,
				ID3D11DeviceContext *devcon,
				LPCTSTR textureFilename,
				DrawBehavior *drawBehavior,
				VertexBehavior *vertexBehavior,
				IndexBehavior *indexBehavior);

	float getHeight(float x, float z);
};

#endif