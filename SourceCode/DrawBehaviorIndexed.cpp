#include "DrawBehaviorIndexed.h"

DrawBehaviorIndexed::DrawBehaviorIndexed(	ID3D11Device		*device,
											ID3D11DeviceContext *devcon,
											ID3D11Buffer		*vertexBuffer,
											ID3D11Buffer		*indexBuffer,
											ID3D11VertexShader	*vertexShader,
											ID3D11PixelShader	*pixelShader,
											ID3D11InputLayout	*inputLayout,
											UINT				indexCount)
{
	this->device		= device;
	this->devcon		= devcon;
	this->vertexBuffer	= vertexBuffer;
	this->indexBuffer	= indexBuffer;
	this->vertexShader	= vertexShader;
	this->pixelShader	= pixelShader;
	this->inputLayout	= inputLayout;
	this->indexCount	= indexCount;
}

DrawBehaviorIndexed::~DrawBehaviorIndexed()
{
	device->Release();
	devcon->Release();
	vertexBuffer->Release();
	indexBuffer->Release();
	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();
}

void DrawBehaviorIndexed::draw()
{
	devcon->VSSetShader(vertexShader, 0, 0);
	devcon->PSSetShader(pixelShader, 0, 0);
	devcon->IASetInputLayout(inputLayout);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	devcon->OMSetDepthStencilState(0, 0);

	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->DrawIndexed(indexCount, 0, 0);
}