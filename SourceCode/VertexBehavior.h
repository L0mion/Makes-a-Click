#ifndef VERTEXBEHAVIOR_H
#define VERTEXBEHAVIOR_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include <vector>

#include "Vertex.h"

using namespace std;

class VertexBehavior
{
public:
	virtual void constructVertexBuffer() = 0;
	virtual ID3D11Buffer* getVertexBuffer() = 0;
	virtual vector<Vertex> getVertices() = 0;
};

#endif