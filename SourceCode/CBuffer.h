#ifndef CBUFFER_H
#define CBUFFER_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include "Light.h"

/*
Constant Buffers must be 16 byte alligned!
Padding variables are used to make sure that
variables don't cross 16 byte borders.
*/

struct CBuffer
{
	D3DXMATRIX final;
	D3DXMATRIX world;
	D3DXMATRIX textureScale;
};

struct CBufferLights
{
	Light light;
	D3DXVECTOR3 eyePosition;
};



#endif