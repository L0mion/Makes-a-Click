#include "EntityBufferInfo.h"
#include "managementD3D.h"

EntityBufferInfo::~EntityBufferInfo()
{
	SAFE_RELEASE( m_vertexBuffer );
	SAFE_RELEASE( m_indexBuffer );

	m_stride = -1;
	m_verticesCnt = -1;
	m_indicesCnt = -1;
}

EntityBufferInfo::EntityBufferInfo()
{
	m_verticesCnt = -1;
	m_indicesCnt = -1;
	m_stride = -1;

	m_vertexBuffer = NULL;
	m_indexBuffer = NULL;
}

HRESULT EntityBufferInfo::setVertexBuffer( const int p_vertexSize, const int p_verticesCnt,
	const void* p_vertices, const ManagementD3D* p_managementD3D )
{
	m_verticesCnt = p_verticesCnt;

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