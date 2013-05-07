#include "managementCB.h"
#include "cbDesc.h"
#include "utility.h"

ManagementCB::ManagementCB()
{
	cbFrame_  = NULL;
	cbSprite_ = NULL;
}
ManagementCB::~ManagementCB()
{
	SAFE_RELEASE(cbFrame_);
	SAFE_RELEASE(cbSprite_);
}

void ManagementCB::vsSetCB(ID3D11DeviceContext* devcon, CBTypes cbType)
{
	switch(cbType)
	{
	case CBTypes_FRAME:
		devcon->VSSetConstantBuffers(CB_FRAME_REGISTER, 1, &cbFrame_);
		break;
	case CBTypes_SPRITE:
		devcon->VSSetConstantBuffers(CB_SPRITE_REGISTER, 1, &cbSprite_);
		break;
	default:
		devcon->VSSetConstantBuffers(0, 0, NULL);
		break;
	}
}
void ManagementCB::psSetCB(ID3D11DeviceContext* devcon, CBTypes cbType)
{
	switch(cbType)
	{
	case CBTypes_FRAME:
		devcon->PSSetConstantBuffers(CB_FRAME_REGISTER, 1, &cbFrame_);
		break;
	case CBTypes_SPRITE:
		devcon->PSSetConstantBuffers(CB_SPRITE_REGISTER, 1, &cbSprite_);
		break;
	default:
		devcon->PSSetConstantBuffers(0, 0, NULL);
		break;
	}
}

void ManagementCB::updateCBFrame(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4 finalMatrix)
{
	CBFrame cBuffer;
	cBuffer.finalMatrix_ = finalMatrix;

	devcon->UpdateSubresource(cbFrame_, 0, 0, &cBuffer, 0, 0);
}
void ManagementCB::updateCBSprite(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4 transformMatrix)
{
	CBSprite cBuffer;
	cBuffer.spriteTransform_ = transformMatrix;

	devcon->UpdateSubresource(cbSprite_, 0, 0, &cBuffer, 0, 0);
}

HRESULT ManagementCB::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initConstantBuffer(device, &cbFrame_, CB_FRAME_SIZE);
	if(SUCCEEDED(hr))
		hr = initConstantBuffer(device, &cbSprite_, CB_SPRITE_SIZE);

	return hr;
}
HRESULT ManagementCB::initConstantBuffer(ID3D11Device* device, ID3D11Buffer** constantBuffer, unsigned int size)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= size;
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;
	
	hr = device->CreateBuffer(&bufferDesc, NULL, constantBuffer);
	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetCB::initConstantBuffer() | CreateBuffer() | Failed", L"ConstantBuffer fail", MB_OK | MB_ICONEXCLAMATION); 
	
	return hr;
}