#include "ShaderManager.h"

ShaderManager::ShaderManager(ID3D11Device *device, ID3D11DeviceContext *devcon)
{
	this->device = device;
	this->devcon = devcon;
}

ShaderManager::~ShaderManager()
{
	device->Release();
	devcon->Release();

	constantBuffer->Release();
	cBufferLights->Release();

	vertexShader->Release();
	pixelShader->Release();
	inputLayout->Release();

	vs->Release();
	ps->Release();
	//error->Release();
}

void ShaderManager::createShaders()
{
	D3DX11CompileFromFile(L"Files/Shaders/BasicShader.hlsl", 0, 0, "VShader", "vs_5_0", SHADER_COMPILE_FLAG, 0, 0, &vs, &error, 0);
	if(error != NULL)
	{
		MessageBox(NULL, L"Vertex shader failed to compile", L"Vertex shader error!", MB_OK | MB_ICONEXCLAMATION);
	}
	
	error = NULL;
	D3DX11CompileFromFile(L"Files/Shaders/BasicShader.hlsl", 0, 0, "PShader", "ps_5_0", SHADER_COMPILE_FLAG, 0, 0, &ps, &error, 0);
	if(error != NULL)
	{
		MessageBox(NULL, L"Pixel shader failed to compile", L"Pixel shader error!", MB_OK | MB_ICONEXCLAMATION);
	}

	device->CreateVertexShader(vs->GetBufferPointer(), vs->GetBufferSize(), NULL, &vertexShader);
	device->CreatePixelShader(ps->GetBufferPointer(), ps->GetBufferSize(), NULL, &pixelShader);

	devcon->VSSetShader(vertexShader, 0, 0);
	devcon->PSSetShader(pixelShader, 0, 0);
}

void ShaderManager::createInputLayout()
{
	/*D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};*/

	D3D11_INPUT_ELEMENT_DESC ied[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0},
	};

	device->CreateInputLayout(ied, 3, vs->GetBufferPointer(), vs->GetBufferSize(), &inputLayout);
	devcon->IASetInputLayout(inputLayout);
}

void ShaderManager::createConstantBuffer()
{
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = calcConstantBufferSize(sizeof(CBuffer)); //Must be a  multiple of 16 bytes for constant buffers!
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	device->CreateBuffer(&bd, NULL, &constantBuffer);
	devcon->VSSetConstantBuffers(0, 1, &constantBuffer);

	ZeroMemory(&bd, sizeof(bd));
	bd.Usage = D3D11_USAGE_DEFAULT;
	bd.ByteWidth = calcConstantBufferSize(sizeof(CBufferLights)); //Must be a  multiple of 16 bytes for constant buffers!
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	HRESULT hr;
	hr = device->CreateBuffer(&bd, NULL, &cBufferLights);
	if(FAILED(hr))
	{
		MessageBox(NULL, L"Constant buffer creation failed", L"Constant buffer error!", MB_OK | MB_ICONEXCLAMATION);
	}
	devcon->PSSetConstantBuffers(0, 1, &cBufferLights);
}

int ShaderManager::calcConstantBufferSize(int structSize)
{
	return structSize + (16 - (structSize%16));
}

void ShaderManager::initialize()
{
	createShaders();
	createInputLayout();
	createConstantBuffer();
}

void ShaderManager::updateConstantBuffer(D3DXMATRIX world, D3DXMATRIX final, D3DXMATRIX textureScale)
{
	CBuffer cBuffer;
	cBuffer.final = final;
	cBuffer.world = world;
	cBuffer.textureScale = textureScale;
	devcon->UpdateSubresource(constantBuffer, 0, 0, &cBuffer, 0, 0);
}

void ShaderManager::updateCBufferLights(Light light, D3DXVECTOR3 eyePosition)
{
	CBufferLights cBuffer;
	cBuffer.light = light;
	cBuffer.eyePosition = eyePosition;
	devcon->UpdateSubresource(cBufferLights, 0, 0, &cBuffer, 0, 0);
}

ID3D11VertexShader* ShaderManager::getVertexShader() const
{
	return vertexShader;
}

ID3D11PixelShader* ShaderManager::getPixelShader() const
{
	return pixelShader; 
}

ID3D11InputLayout* ShaderManager::getInputLayout() const
{
	return inputLayout;
}

ID3D11Buffer* ShaderManager::getConstantBuffer() const
{
	return constantBuffer;
}
