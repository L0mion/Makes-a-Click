#include "CubeFactory.h"

#include "EntityBufferInfo.h"
#include "managementD3D.h"
#include "vertex.h"
#include <DirectXMath.h>
#include <vector>

HRESULT CubeFactory::createCube( ManagementD3D* p_managementD3D,
	EntityBufferInfo** out_cube )
{
	HRESULT hr = S_FALSE;
	if( out_cube != NULL )
	{
		EntityBufferInfo* cube = new EntityBufferInfo();
		cube->m_stride = sizeof(Vertex);

		hr = createVertices( p_managementD3D, cube );
		if( !FAILED(hr) ) {
			hr = createIndices( p_managementD3D, cube );
		}

		if( !FAILED(hr) ) {
			*out_cube = cube; 
		} else {
			delete cube;
		}
	}
	return hr;
}

HRESULT CubeFactory::createVertices( ManagementD3D* p_managementD3D,
	EntityBufferInfo* inout_cube )
{
	std::vector<Vertex> vertices;

	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)) );   
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)) );

	// side 2															 										   
	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)) );    
	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)) );

	// side 3															 										   
	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)) ); 
	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)) );

	// side 4																 									   
	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)) );  
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)) );

	// side 5																	 								   
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)) );  
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)) );

	// side 6				  																					   
	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)) );  
	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)) );
	vertices.push_back( Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)) );

	HRESULT vertexResult = inout_cube->setVertexBuffer( sizeof(Vertex),
		vertices.size(), &vertices[0], p_managementD3D );

	return vertexResult;
}

HRESULT CubeFactory::createIndices( ManagementD3D* p_managementD3D,
								EntityBufferInfo* inout_cube )
{
	std::vector<DWORD> indices;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(7);

	indices.push_back(8);
	indices.push_back(9);
	indices.push_back(10);

	indices.push_back(10);
	indices.push_back(9);
	indices.push_back(11);

	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(14);

	indices.push_back(14);
	indices.push_back(13);
	indices.push_back(15);

	indices.push_back(16);
	indices.push_back(17);
	indices.push_back(18);

	indices.push_back(18);
	indices.push_back(17);
	indices.push_back(19);

	indices.push_back(20);
	indices.push_back(21);
	indices.push_back(22);

	indices.push_back(22);
	indices.push_back(21);
	indices.push_back(23);

	HRESULT indexResult = inout_cube->setIndexBuffer( indices.size(),
		&indices[0], p_managementD3D );

	return indexResult;
}