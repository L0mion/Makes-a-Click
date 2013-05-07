#include "managementSS.h"
#include "utility.h"

ManagementSS::ManagementSS()
{
	m_ssDefault = NULL;
}
ManagementSS::~ManagementSS()
{
	SAFE_RELEASE(m_ssDefault);
}

void ManagementSS::setSS(ID3D11DeviceContext* devcon, SSTypes ssType, unsigned int shaderRegister)
{
	switch(ssType)
	{
	case SSTypes_DEFAULT:
		devcon->PSSetSamplers(shaderRegister, 1, &m_ssDefault);
		break;
	}
}

HRESULT ManagementSS::init(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	hr = initSSDefault(p_device);

	return hr;
}
HRESULT ManagementSS::initSSDefault(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sampDesc;
    ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    sampDesc.AddressU		= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressV		= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.AddressW		= D3D11_TEXTURE_ADDRESS_CLAMP;
    sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
    sampDesc.MinLOD			= 0;
    sampDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	hr = p_device->CreateSamplerState(&sampDesc, &m_ssDefault);
	if(FAILED(hr))
		MessageBox(NULL, L"ManagementSS::initSSDefault() | device->CreateSamplerState() | Failed", L"ssDefault", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}