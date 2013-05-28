#pragma once
#include <d3d11.h>
#include "utility.h"
#include "textureIds.h"
#include "DirectXMath.h"
#include "managementBS.h"

class ManagementD3D;
struct ObjectMold;

struct EntityBufferInfo
{
public:
	EntityBufferInfo();
	~EntityBufferInfo();

	void setSize( const float p_size );
	float getSize() const;
	void setPosition( const DirectX::XMFLOAT3& p_pos );
	DirectX::XMFLOAT3 getPosition() const;

	HRESULT setFromMold( const ObjectMold* p_mold, const ManagementD3D* p_managementD3D  );

	HRESULT setVertexBuffer( const int p_vertexSize, const int p_verticesCnt,
		const void* p_vertices, const ManagementD3D* p_managementD3D );

	HRESULT setIndexBuffer( const int p_indexCnt, const void* p_indices,
		const ManagementD3D* p_managementD3D );
	
public:
	DirectX::XMFLOAT4X4 m_world;
	DirectX::XMFLOAT3 m_position;
	float m_size;

	TextureIds::Id m_textureId;
	int m_stride;
	int m_verticesCnt;
	int m_indicesCnt;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	// States
	ManagementBS::BSTypes m_blendState;

};