#include "managementBS.h"
#include "utility.h"

ManagementBS::ManagementBS()
{

	for( int bsIdx=0; bsIdx<BSTypes_CNT; bsIdx++ ) {
		m_blendStates[bsIdx] = NULL;
	}
}
ManagementBS::~ManagementBS()
{

	for( int bsIdx=0; bsIdx<BSTypes_CNT; bsIdx++ ) {
		SAFE_RELEASE( m_blendStates[bsIdx] );
	}
}

void ManagementBS::setBlendState(ID3D11DeviceContext* p_devcon, BSTypes p_type)
{
	FLOAT blendFactor[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	UINT  sampleMask	 = 0xffffffff;

	p_devcon->OMSetBlendState( m_blendStates[p_type], blendFactor, sampleMask );
}

HRESULT ManagementBS::init(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;
	
	hr = initBSDefault(p_device);
	if(SUCCEEDED(hr))
		hr = initBSTransparency(p_device);
	
	if(SUCCEEDED(hr))
		hr = initBSAdditive(p_device);

	return hr;
}
HRESULT ManagementBS::initBSDefault(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	blendDesc.RenderTarget[0].BlendEnable = FALSE;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = p_device->CreateBlendState(&blendDesc, &m_blendStates[BSTypes_DEFAULT]);

	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetBS::initBSDefault() | CreateBlendState() | Failed", L"Blend State Default", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}
HRESULT ManagementBS::initBSTransparency(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	blendDesc.RenderTarget[0].BlendEnable			= true;
	blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_INV_SRC_ALPHA;
	blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ZERO;
	blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = 0x0f;

	hr = p_device->CreateBlendState(&blendDesc, &m_blendStates[BSTypes_TRANSPARENCY]);

	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetBS::initBSTransparency() | CreateBlendState() | Failed", L"Blend State Transparency", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}

HRESULT ManagementBS::initBSAdditive(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	D3D11_BLEND_DESC blendDesc;
	ZeroMemory(&blendDesc, sizeof(blendDesc));

	blendDesc.RenderTarget[0].BlendEnable			= true;
	blendDesc.RenderTarget[0].SrcBlend				= D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlend				= D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOp				= D3D11_BLEND_OP_ADD;

	blendDesc.RenderTarget[0].SrcBlendAlpha			= D3D11_BLEND_SRC_ALPHA;
	blendDesc.RenderTarget[0].DestBlendAlpha		= D3D11_BLEND_ONE;
	blendDesc.RenderTarget[0].BlendOpAlpha			= D3D11_BLEND_OP_ADD;
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	hr = p_device->CreateBlendState(&blendDesc, &m_blendStates[BSTypes_ADDITIVE]);

	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetBS::initBSAdditive() | CreateBlendState() | Failed", L"Blend State Additive", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}