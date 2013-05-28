#include "EntityBufferInfo.h"

#include "ObjectMold.h"
#include "managementD3D.h"
#include "mathHelper.h"
#include "vertex.h"

EntityBufferInfo::EntityBufferInfo()
{
	m_world = DirectX::XMFLOAT4X4( MathHelper::identityMatrix );
	m_position = XMFLOAT3( .0f, .0f, .0f );
	m_size = 1.0f;

	m_verticesCnt = -1;
	m_indicesCnt = -1;
	m_stride = -1;

	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;

	m_blendState = ManagementBS::BSTypes_DEFAULT;
}

EntityBufferInfo::~EntityBufferInfo()
{
	SAFE_RELEASE( m_vertexBuffer );
	SAFE_RELEASE( m_indexBuffer );

	m_stride = -1;
	m_verticesCnt = -1;
	m_indicesCnt = -1;
}

void EntityBufferInfo::setSize( const float p_size )
{
	m_size = p_size;
	m_world._44 = 1.0f/p_size;
}
float EntityBufferInfo::getSize() const { return m_size; }

void EntityBufferInfo::setPosition( const DirectX::XMFLOAT3& p_pos )
{
	m_position = p_pos;
	m_world._41 = m_position.x/m_size;
	m_world._42 = m_position.y/m_size;
	m_world._43 = m_position.z/m_size;
}
DirectX::XMFLOAT3 EntityBufferInfo::getPosition() const { return m_position; }

HRESULT EntityBufferInfo::setFromMold( const ObjectMold* p_mold,
	const ManagementD3D* p_managementD3D )
{
	HRESULT hr;

	hr = setVertexBuffer( sizeof(Vertex_PNT), p_mold->m_vertices.size(),
		&p_mold->m_vertices[0], p_managementD3D );
	hr = setIndexBuffer( p_mold->m_indices.size(), &p_mold->m_indices[0],
		p_managementD3D );
	m_textureId = p_mold->m_textureId;
	m_blendState = ManagementBS::BSTypes_TRANSPARENCY;

	return hr;
}


HRESULT EntityBufferInfo::setVertexBuffer( const int p_vertexSize, const int p_verticesCnt,
	const void* p_vertices, const ManagementD3D* p_managementD3D )
{
	m_verticesCnt = p_verticesCnt;
	m_stride = p_vertexSize;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = p_vertexSize * p_verticesCnt;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = p_vertices;
	ID3D11Device* device = p_managementD3D->getDevice();
	HRESULT hr = device->CreateBuffer(&vbd, &vinitData, &m_vertexBuffer);
	return hr;
}

HRESULT EntityBufferInfo::setIndexBuffer( const int p_indexCnt,
	const void* p_indices, const ManagementD3D* p_managementD3D )
{
	m_indicesCnt = p_indexCnt;

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(DWORD) * p_indexCnt;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = p_indices;
	ID3D11Device* device = p_managementD3D->getDevice();
	HRESULT hr = device->CreateBuffer(&ibd, &iinitData, &m_indexBuffer);
	return hr;
}