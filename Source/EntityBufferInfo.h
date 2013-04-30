#pragma once
#include <d3d11.h>
#include "utility.h"

struct EntityBufferInfo
{
	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	EntityBufferInfo()
	{
		m_vertexBuffer = NULL;
		m_indexBuffer = NULL;
	}

	~EntityBufferInfo()
	{
		SAFE_RELEASE( m_vertexBuffer );
		SAFE_RELEASE( m_indexBuffer );
	}
};