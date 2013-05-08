#include "managementTex.h"
#include "DDSTextureLoader.h"
#include "utility.h"

ManagementTex::ManagementTex()
{
}
ManagementTex::~ManagementTex()
{
	for(unsigned int i=0; i<m_srvTextures.size(); i++)
		SAFE_RELEASE(m_srvTextures[i]);
}

void ManagementTex::psSetTexture( ID3D11DeviceContext* p_devcon,
	TextureIds::Id p_textureId, unsigned int p_shaderRegister )
{
	if( 0 <= p_textureId && p_textureId < TextureIds::TextureIds_COUNT ) {
		p_devcon->PSSetShaderResources( p_shaderRegister, 1,
			&m_srvTextures[p_textureId] );
	} else {
		p_devcon->PSSetShaderResources( p_shaderRegister, 1,
			&m_srvTextures[TextureIds::TextureIds_PLACEHOLDER] );
	}
}

HRESULT ManagementTex::init(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;
	m_srvTextures.resize(TextureIds::TextureIds_COUNT, NULL);

	hr = loadTexture(p_device, L"../../Textures/placeholderTex.dds",
		TextureIds::TextureIds_PLACEHOLDER);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/circleBackground.dds",
			TextureIds::TextureIds_CIRCLE_BACKGROUND);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/circleHighlight.dds",
			TextureIds::TextureIds_CIRCLE_HIGHLIGHT);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/heightmap/sand/Sand_4_Diffuse.dds",
			TextureIds::TextureIds_HEIGHTMAP);
	return hr;
}
HRESULT ManagementTex::loadTexture( ID3D11Device* p_device,
	std::wstring p_filename, TextureIds::Id textureId )
{
	HRESULT hr = S_OK;
	ID3D11Resource* texture = NULL;
	ID3D11ShaderResourceView* srvTexture = NULL;

	hr = DirectX::CreateDDSTextureFromFile( p_device, p_filename.c_str(),
		&texture, &srvTexture );
	if(FAILED(hr))
		MessageBox(NULL,
		L"ManagmenetTex::loadTexture() | CreateDDSTextureFromFile() | Failed",
		p_filename.c_str(), MB_OK | MB_ICONEXCLAMATION);

	m_srvTextures[textureId] = srvTexture;
	SAFE_RELEASE(texture);

	return hr;
}