#pragma once
#include <d3d11.h>
#include "utility.h"

struct EntityBufferInfo
{
	int m_stride;
	int m_verticesCnt;
	int m_indicesCnt;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	EntityBufferInfo()
	{
		m_verticesCnt = -1;
		m_indicesCnt = -1;
		m_stride = -1;

		m_vertexBuffer = NULL;
		m_indexBuffer = NULL;
	}

	~EntityBufferInfo()
	{
		SAFE_RELEASE( m_vertexBuffer );
		SAFE_RELEASE( m_indexBuffer );

		m_stride = -1;
		m_verticesCnt = -1;
		m_indicesCnt = -1;
	}
};