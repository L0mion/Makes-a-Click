#ifndef DRAWBEHAVIORINDEXED_H
#define DRAWBEHAVIORINDEXED_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include "DrawBehavior.h"
#include "Vertex.h"

class DrawBehaviorIndexed : public DrawBehavior
{
private:
	ID3D11Device		*device;
	ID3D11DeviceContext *devcon;
	ID3D11Buffer		*vertexBuffer;
	ID3D11Buffer		*indexBuffer;
	ID3D11VertexShader	*vertexShader;
	ID3D11PixelShader	*pixelShader;
	ID3D11InputLayout	*inputLayout;

	UINT indexCount;

public:
	DrawBehaviorIndexed(ID3D11Device		*device,
						ID3D11DeviceContext *devcon,
						ID3D11Buffer		*vertexBuffer,
						ID3D11Buffer		*indexBuffer,
						ID3D11VertexShader	*vertexShader,
						ID3D11PixelShader	*pixelShader,
						ID3D11InputLayout	*inputLayout,
						UINT				indexCount);

	~DrawBehaviorIndexed();

	virtual void draw();
};

#endif