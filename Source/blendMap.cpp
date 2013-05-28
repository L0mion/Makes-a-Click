#include "blendMap.h"
#include "HeightMap.h"
#include "PivotPoint.h"
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

void BlendMap::update(ID3D11DeviceContext* p_devcon,
					  PivotPoint* p_pivot,
					  HeightMap* p_heightMap,
					  ManagementMenu::ToolPropertyIds toolProperty,
					  float p_dt)
{
	int col		 = p_heightMap->getCol(p_pivot->getPosition().x);
	int row		 = p_heightMap->getRow(p_pivot->getPosition().z);
	float radius = p_pivot->getSize();
	float speed  = p_pivot->getSpeed();
	float strength = 10.0f*p_dt;

	int lowX  = col - static_cast<int>(radius);
	int highX = col + static_cast<int>(radius);
	int lowZ  = row - static_cast<int>(radius);
	int highZ = row + static_cast<int>(radius);
	if(fabs(speed) > 0)
	{
		for( int x=-radius; x<radius; x++ ) 
		{
			for( int z=-radius; z<radius; z++ ) 
			{
				int index = (z+row)*m_width + (x + col);
				if(index < m_numTexels)
				{
					float amount = (float)radius - sqrt((float)(x*x+z*z));
					amount = amount / (float)radius;
					if(amount < 0.0f)
						amount = 0.0f;
					amount *= strength * speed;

					if(index >= 0 && index < m_numTexels)
					{
						if(toolProperty == ManagementMenu::ToolPropertyIds_PROPERTY_0)
							modifyTexelChannel(&(m_texels[index].m_red), amount);
						else if(toolProperty == ManagementMenu::ToolPropertyIds_PROPERTY_1)
							modifyTexelChannel(&(m_texels[index].m_green), amount);
						else if(toolProperty == ManagementMenu::ToolPropertyIds_PROPERTY_2)
							modifyTexelChannel(&(m_texels[index].m_blue), amount);
					}
				}
			}
		}
		updateTexture(p_devcon);
	}
}

void BlendMap::setTexel(ID3D11DeviceContext* p_devcon, Util::Texel p_texel, int p_x, int p_y)
{
	m_texels[p_x * m_width + p_y] = p_texel;
	updateTexture(p_devcon);
}
void BlendMap::setAllTexels(ID3D11DeviceContext* p_devcon, Util::Texel p_texel)
{
	for(unsigned int i=0; i<m_texels.size(); i++)
		m_texels[i] = p_texel;

	updateTexture(p_devcon);
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
	m_width		= p_width;
	m_height	= p_height;
	m_numTexels = m_width * m_height;
	m_texels.resize(p_width*p_height, Util::Texel(0, 0, 0, 0));

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
	texDesc.Format				= DXGI_FORMAT_R32G32B32A32_FLOAT;
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

	srvDesc.Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	srvDesc.ViewDimension = D3D11_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MostDetailedMip = 0;
	srvDesc.Texture2D.MipLevels = 1;

	hr = p_device->CreateShaderResourceView(m_texBlendMap, &srvDesc, &m_srvBlendMap);
	if(FAILED(hr))
		MessageBox(NULL, L"BlendMap::initSrvBlendMap() | device->CreateShaderResourceView() | Failed", L"BlendMap", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}

HRESULT BlendMap::updateTexture(ID3D11DeviceContext* p_devcon)
{
	D3D11_MAPPED_SUBRESOURCE resource;
	ZeroMemory(&resource, sizeof(resource));

	HRESULT hr = S_OK;
	hr = p_devcon->Map(m_texBlendMap, 0, D3D11_MAP_WRITE_DISCARD, 0, &resource);
	if(FAILED(hr))
		MessageBox(NULL, L"BlendMap::setPixel() | devcon->Map() | Failed", L"BlendMap", MB_OK | MB_ICONEXCLAMATION);

	memcpy( resource.pData,& m_texels[0], sizeof(m_texels[0])*m_texels.size() );

	p_devcon->Unmap(m_texBlendMap, 0);

	return hr;
}

void BlendMap::modifyTexelChannel(float* p_channel, float p_value)
{
	(*p_channel) += p_value;
	if((*p_channel) > 1.0f)
		(*p_channel) = 1.0f;
	if((*p_channel) < 0.0f)
		(*p_channel) = 0.0f;
}

bool BlendMap::insideCircle(float p_radius, int p_x, int p_z, int p_col, int p_row)
{
	int temp = (p_col-p_x)*(p_col-p_x)+(p_row-p_z)*(p_row-p_z);
	float hyp = sqrtf(static_cast<float>(temp));

	return hyp < p_radius;
}