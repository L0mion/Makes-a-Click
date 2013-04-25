#include "managementCB.h"
#include "cbDesc.h"
#include "utility.h"

ManagementCB::ManagementCB()
{
	cbFrame_ = NULL;
}
ManagementCB::~ManagementCB()
{
	SAFE_RELEASE(cbFrame_);
}

void ManagementCB::updateCBFrame(ID3D11DeviceContext* devcon, DirectX::XMFLOAT4X4 viewMatrix, DirectX::XMFLOAT4X4 projectionMatrix)
{
	CBFrame cBuffer;
	cBuffer.viewMatrix_ = viewMatrix;
	cBuffer.projectionMatrix_ = projectionMatrix;

	devcon->UpdateSubresource(cbFrame_, 0, 0, &cBuffer, 0, 0);
}

HRESULT ManagementCB::init(ID3D11Device* device)
{
	HRESULT hr = S_OK;

	hr = initCBFrame(device);

	return hr;
}
HRESULT ManagementCB::initCBFrame(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	D3D11_BUFFER_DESC bufferDesc;
	ZeroMemory(&bufferDesc, sizeof(bufferDesc));
	
	bufferDesc.Usage			= D3D11_USAGE_DEFAULT;
	bufferDesc.ByteWidth		= CB_FRAME_SIZE;
	bufferDesc.BindFlags		= D3D11_BIND_CONSTANT_BUFFER;
	bufferDesc.CPUAccessFlags	= 0;
	
	hr = device->CreateBuffer(&bufferDesc, NULL, &cbFrame_);
	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetCB::initCBFrame() | CreateBuffer() | Failed", L"ConstantBuffer fail", MB_OK | MB_ICONEXCLAMATION); 
	
	return hr;
}