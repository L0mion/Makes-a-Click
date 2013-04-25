#include "VertexBehaviorCube.h"

VertexBehaviorCube::VertexBehaviorCube(ID3D11Device* device)
{
	this->device = device;
}

VertexBehaviorCube::~VertexBehaviorCube()
{
	device->Release();
	vertexBuffer->Release();
}

void VertexBehaviorCube::createVertices()
{
	 // side 1
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, -1.0f, 1.0f),	D3DXVECTOR3(0.0f, 0.0f, 1.0f),	D3DXVECTOR2(0.0f, 0.0f)));   
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, -1.0f, 1.0f),	D3DXVECTOR3(0.0f, 0.0f, 1.0f),	D3DXVECTOR2(0.0f, 1.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, 1.0f, 1.0f),	D3DXVECTOR3(0.0f, 0.0f, 1.0f),	D3DXVECTOR2(1.0f, 0.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, 1.0f, 1.0f),	D3DXVECTOR3(0.0f, 0.0f, 1.0f),	D3DXVECTOR2(1.0f, 1.0f)));
										
	// side 2																				 
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, -1.0f, -1.0f),	D3DXVECTOR3(0.0f, 0.0f, -1.0f),	D3DXVECTOR2(0.0f, 0.0f)));    
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, 1.0f, -1.0f),	D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(0.0f, 1.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, -1.0f, -1.0f),	D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR2(1.0f, 0.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, 1.0f, -1.0f),	D3DXVECTOR3(0.0f, 0.0f, -1.0f),	D3DXVECTOR2(1.0f, 1.0f)));
										
	// side 3																			 	 
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, 1.0f, -1.0f),	D3DXVECTOR3(0.0f, 1.0f, 0.0f),	D3DXVECTOR2(0.0f, 0.0f)));    
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, 1.0f, 1.0f),	D3DXVECTOR3(0.0f, 1.0f, 0.0f),	D3DXVECTOR2(0.0f, 1.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, 1.0f, -1.0f),	D3DXVECTOR3(0.0f, 1.0f, 0.0f),	D3DXVECTOR2(1.0f, 0.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, 1.0f, 1.0f),	D3DXVECTOR3(0.0f, 1.0f, 0.0f),	D3DXVECTOR2(1.0f, 1.0f)));
									
	// side 4																				 	 
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, -1.0f, -1.0f),	D3DXVECTOR3(0.0f, -1.0f, 0.0f),	D3DXVECTOR2(0.0f, 0.0f)));    
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, -1.0f, -1.0f),	D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, -1.0f, 1.0f),	D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, -1.0f, 1.0f),	D3DXVECTOR3(0.0f, -1.0f, 0.0f),	D3DXVECTOR2(1.0f, 1.0f)));
								
	// side 5																						 
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, -1.0f, -1.0f),	D3DXVECTOR3(1.0f, 0.0f, 0.0f),	D3DXVECTOR2(0.0f, 0.0f)));    
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, 1.0f, -1.0f),	D3DXVECTOR3(1.0f, 0.0f, 0.0f),	D3DXVECTOR2(0.0f, 1.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, -1.0f, 1.0f),	D3DXVECTOR3(1.0f, 0.0f, 0.0f),	D3DXVECTOR2(1.0f, 0.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(1.0f, 1.0f, 1.0f),	D3DXVECTOR3(1.0f, 0.0f, 0.0f),	D3DXVECTOR2(1.0f, 1.0f)));
	
	// side 6
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, -1.0f, -1.0f),	D3DXVECTOR3(-1.0f, 0.0f, 0.0f),	D3DXVECTOR2(0.0f, 0.0f)));    
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, -1.0f, 1.0f),	D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(0.0f, 1.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, 1.0f, -1.0f),	D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR2(1.0f, 0.0f)));
	vertices.push_back(Vertex(D3DXVECTOR3(-1.0f, 1.0f, 1.0f),	D3DXVECTOR3(-1.0f, 0.0f, 0.0f),	D3DXVECTOR2(1.0f, 1.0f)));
}

void VertexBehaviorCube::createVertexBuffer()
{
	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	device->CreateBuffer(&vbd, &vinitData, &vertexBuffer);
}

void VertexBehaviorCube::constructVertexBuffer()
{
	createVertices();
	createVertexBuffer();
}

ID3D11Buffer* VertexBehaviorCube::getVertexBuffer()
{
	return vertexBuffer;
}

vector<Vertex> VertexBehaviorCube::getVertices()
{
	return vertices;
}