#ifndef VERTEX_H
#define VERTEX_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

struct Vertex
{
	D3DXVECTOR3 position;
	D3DXVECTOR3 normal;
	D3DXVECTOR2	texcoord;

	Vertex()
	{
		position	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		normal		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
		texcoord	= D3DXVECTOR2(0.0f, 0.0f);
	}

	Vertex(D3DXVECTOR3 position, D3DXVECTOR3 normal, D3DXVECTOR2 texcoord)
	{
		this->position	= position;
		this->normal	= normal;
		this->texcoord	= texcoord;
	}
};
#endif