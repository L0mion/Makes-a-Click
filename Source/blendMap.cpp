#include "blendMap.h"
#include "utility.h"

BlendMap::BlendMap()
{
	m_texBlendMap = NULL;
	m_srvBlendMap = NULL;
}
BlendMap::~BlendMap()
{
	SAFE_RELEASE(m_texBlendMap);
	SAFE_RELEASE(m_srvBlendMap);
}

void BlendMap::setTexel(ID3D11DeviceContext* p_devcon, Texel p_texel, int p_x, int p_y)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	ZeroMemory(&resource, sizeof(resource));
	
	m_texels[p_x * m_width + p_y] = p_texel;

	HRESULT hr = S_OK;
	hr = p_devcon->Map(m_texBlendMap, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if(FAILED(hr))
		MessageBox(NULL, L"BlendMap::setPixel() | devcon->Map() | Failed", L"BlendMap", MB_OK | MB_ICONEXCLAMATION);

	memcpy( resource.pData,& m_texels[0], sizeof(m_texels[0])*m_texels.size() );

	p_devcon->Unmap(m_texBlendMap, 0);
}
void BlendMap::setAllTexels(ID3D11DeviceContext* p_devcon, Texel p_texel)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	ZeroMemory(&resource, sizeof(resource));
	
	for(unsigned int i=0; i<m_texels.size(); i++)
		m_texels[i] = p_texel;

	HRESULT hr = S_OK;
	hr = p_devcon->Map(m_texBlendMap, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if(FAILED(hr))
		MessageBox(NULL, L"BlendMap::setPixel() | devcon->Map() | Failed", L"BlendMap", MB_OK | MB_ICONEXCLAMATION);

	memcpy( resource.pData,& m_texels[0], sizeof(m_texels[0])*m_texels.size() );

	p_devcon->Unmap(m_texBlendMap, 0);
}

void BlendMap::psSetBlendMap(ID3D11DeviceContext* p_devcon, unsigned int startSlott)
{
	p_devcon->PSSetShaderResources(startSlott, 1, &m_srvBlendMap);
}

ID3D11Texture2D* BlendMap::getTexBlendMap() const
{
	return m_texBlendMap;
}
ID3D11ShaderResourceView* BlendMap::getSrvBlendMap() const
{
	return m_srvBlendMap;
}

HRESULT BlendMap::init(ID3D11Device* p_device, int p_width, int p_height)
{
	int m_width  = p_width;
	int m_height = p_height;
	m_texels.resize(p_width*p_height, Texel(25, 0, 0, 0));

	HRESULT hr = S_OK;
	hr = initTexBlendMap(p_device, p_width, p_height);
	if(SUCCEEDED(hr))
		hr = initSrvBlendMap(p_device);

	return hr;
}
HRESULT BlendMap::initTexBlendMap(ID3D11Device* p_device, int p_width, int p_height)
{
	HRESULT hr = S_OK;

	D3D11_TEXTURE2D_DESC texDesc;
	ZeroMemory(&texDesc, sizeof(texDesc));

	texDesc.Width				= p_width;
	texDesc.Height				= p_height;
	texDesc.MipLevels			= 1;
	texDesc.ArraySize			= 1;
	texDesc.Format				= DXGI_FORMAT_R8G8B8A8_UNORM;
	texDesc.SampleDesc.Count	= 1;
	texDesc.SampleDesc.Quality	= 0;
	texDesc.Usage				= D3D11_USAGE_DYNAMIC;
	texDesc.BindFlags			= D3D11_BIND_SHADER_RESOURCE;
	texDesc.CPUAccessFlags		= D3D11_CPU_ACCESS_WRITE;
	texDesc.MiscFlags			= 0;

	hr = p_device->CreateTexture2D(&texDesc, NULL, &m_texBlendMap);
	if(FAILED(hr))
		MessageBox(NULL, L"BlendMap::initTexBlendMap() | device->CreateTexture2D() | Failed", L"BlendMap", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}
HRESULT BlendMap::initSrvBlendMap(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc;
	ZeroMemory(&srvDesc, sizeof(srvDesc));

	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = p_device->CreateShaderResourceView(m_texBlendMap, &srvDesc, &m_srvBlendMap);
	if(FAILED(hr))
		MessageBox(NULL, L"BlendMap::initSrvBlendMap() | device->CreateShaderResourceView() | Failed", L"BlendMap", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}
