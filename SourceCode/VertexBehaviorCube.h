#ifndef VERTEXBEHAVIORCUBE_H
#define VERTEXBEHAVIORCUBE_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include <vector>

#include "VertexBehavior.h"
#include "Vertex.h"

using namespace std;

class VertexBehaviorCube : public VertexBehavior
{
private:

	ID3D11Device *device;
	ID3D11Buffer *vertexBuffer;

	vector<Vertex> vertices;

	void createVertices();
	void createVertexBuffer();

public:
	VertexBehaviorCube(ID3D11Device *device);
	~VertexBehaviorCube();

	virtual void constructVertexBuffer();
	virtual ID3D11Buffer* getVertexBuffer();
	virtual vector<Vertex> getVertices();
};
#endif