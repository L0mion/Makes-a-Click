#pragma once
#include <wtypesbase.h>

class ManagementD3D;

struct EntityBufferInfo;

class CubeFactory
{
public:
	static HRESULT createCube( ManagementD3D* p_managementD3D,
		EntityBufferInfo** out_cube );

private:
	static HRESULT createVertices( ManagementD3D* p_managementD3D,
		EntityBufferInfo* inout_cube );
	static HRESULT createIndices( ManagementD3D* p_managementD3D,
		EntityBufferInfo* inout_cube );
};