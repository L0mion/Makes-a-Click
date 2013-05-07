#pragma once
#include <d3d11.h>
#include "utility.h"

class ManagementD3D;

struct EntityBufferInfo
{
	int m_stride;
	int m_verticesCnt;
	int m_indicesCnt;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	EntityBufferInfo();

	~EntityBufferInfo();

	HRESULT setVertexBuffer( const int p_vertexSize, const int p_verticesCnt,
		const void* p_vertices, const ManagementD3D* p_managementD3D );

	HRESULT setIndexBuffer( const int p_indexCnt, const void* p_indices,
		const ManagementD3D* p_managementD3D );
};