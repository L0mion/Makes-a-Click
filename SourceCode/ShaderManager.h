#ifndef SHADERMANAGER_H
#define SHADERMANAGER_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include <D3Dcompiler.h>

#include <Windows.h>
#include <WindowsX.h>

#include "Utility.h"
#include "CBuffer.h"

class ShaderManager
{
private:
	ID3D11Device *device;
	ID3D11DeviceContext *devcon;

	ID3D11Buffer *constantBuffer;
	ID3D11Buffer *cBufferLights;


	ID3D11VertexShader *vertexShader;
	ID3D11PixelShader *pixelShader;
	ID3D11InputLayout *inputLayout;

	ID3D10Blob *vs;
	ID3D10Blob *ps;
	ID3D10Blob *error;

	void createShaders();
	void createInputLayout();
	void createConstantBuffer();

	int calcConstantBufferSize(int structSize);
	
public:
	ShaderManager(ID3D11Device *device, ID3D11DeviceContext *devcon);
	~ShaderManager();

	void initialize();
	void updateConstantBuffer(D3DXMATRIX world, D3DXMATRIX final, D3DXMATRIX textureScale);
	void updateCBufferLights(Light light, D3DXVECTOR3 eyePosition);

	ID3D11VertexShader* getVertexShader() const;
	ID3D11PixelShader* getPixelShader() const;
	ID3D11InputLayout* getInputLayout() const;
	ID3D11Buffer *getConstantBuffer() const;
};

#endif