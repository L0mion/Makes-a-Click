#include "managementSS.h"
#include "utility.h"

ManagementSS::ManagementSS()
{
}
ManagementSS::~ManagementSS()
{
	for( unsigned int i=0; i<m_samplerStates.size(); i++ ) {
		SAFE_RELEASE( m_samplerStates[i] );
	}
	
}

void ManagementSS::setSS( ID3D11DeviceContext* p_devcon, SSTypes p_ssType,
	unsigned int p_shaderRegister )
{
	if( 0 <= p_ssType && p_ssType < (int)m_samplerStates.size() ) {
		p_devcon->PSSetSamplers( p_shaderRegister, 1,
			&m_samplerStates[p_ssType] );
	} else {
		p_devcon->PSSetSamplers( p_shaderRegister, 1,
			&m_samplerStates[SSTypes_DEFAULT] );
	}

}

HRESULT ManagementSS::init(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	hr = initSSDefault( p_device );
	hr = initSSWrap( p_device );

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

	ID3D11SamplerState* ss;
	hr = p_device->CreateSamplerState(&sampDesc, &ss);

	if(FAILED(hr)) {
		MessageBox( NULL,
			L"ManagementSS::initSSDefault() | device->CreateSamplerState() | Failed",
			L"ssDefault", MB_OK | MB_ICONEXCLAMATION );
		SAFE_RELEASE( ss );
	} else {
		auto it = m_samplerStates.begin();
		it += SSTypes_DEFAULT;
		m_samplerStates.insert( it, ss );
		//m_samplerStates.push_back( defaultSs );
	}

	return hr;
}
HRESULT ManagementSS::initSSWrap( ID3D11Device* p_device )
{
	HRESULT hr = S_OK;

	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter			= D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW		= D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD			= 0;
	sampDesc.MaxLOD			= D3D11_FLOAT32_MAX;

	ID3D11SamplerState* ss;
	hr = p_device->CreateSamplerState(&sampDesc, &ss);

	if(FAILED(hr)) {
		MessageBox( NULL,
			L"ManagementSS::initSSDefault() | device->CreateSamplerState() | Failed",
			L"ssDefault", MB_OK | MB_ICONEXCLAMATION );
		SAFE_RELEASE( ss );
	} else {
		auto it = m_samplerStates.begin();
		it += SSTypes_WRAP;
		m_samplerStates.insert( it, ss );
		//m_samplerStates.push_back( ss );
	}

	return hr;
}
