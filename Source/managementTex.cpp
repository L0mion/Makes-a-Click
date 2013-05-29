#include "managementTex.h"
#include <DDSTextureLoader.h>
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
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/grass.dds",
			TextureIds::TextureIds_GRASS);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/gravel.dds",
			TextureIds::TextureIds_GRAVEL);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/stone.dds",
			TextureIds::TextureIds_STONE);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/sand-bucket-icon.dds",
			TextureIds::TextureIds_SANB_BUCKET);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/AddRemove.dds",
			TextureIds::TextureIds_ADD_REMOVE);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/Smooth.dds",
			TextureIds::TextureIds_SMOOTH);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/object-icon.dds",
			TextureIds::TextureIds_OBJECT);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/brush-icon.dds",
			TextureIds::TextureIds_TEXTURE_BRUSH);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/textOutputBackground.dds",
			TextureIds::TextureIds_TEXT_OUTPUT_BACKGROUND);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/textBackground.dds",
			TextureIds::TextureIds_TEXT_BACKGROUND);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/textBack.dds",
			TextureIds::TextureIds_TEXT_BACK);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/textHighlighterButtons.dds",
			TextureIds::TextureIds_TEXT_HIGHLIGHTER_BUTTONS);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/textHighlighter.dds",
			TextureIds::TextureIds_TEXT_HIGHLIGHTER);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/text.dds",
			TextureIds::TextureIds_TEXT);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/textCaps.dds",
			TextureIds::TextureIds_TEXT_CAPS);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/textNumbers.dds",
			TextureIds::TextureIds_TEXT_NUMBERS);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Textures/cursor.dds",
			TextureIds::TextureIds_TEXT_CURSOR);

	// Objects
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/hemps/hemp.dds",
		TextureIds::TextureIds_OBJ_HEMP);

	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/shrubs/trop_shrub_01.dds",
		TextureIds::TextureIds_OBJ_SHRUB1);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/shrubs/trop_shrub_02.dds",
		TextureIds::TextureIds_OBJ_SHRUB2);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/shrubs/trop_shrub_03.dds",
		TextureIds::TextureIds_OBJ_SHRUB3);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/shrubs/trop_shrub_04.dds",
		TextureIds::TextureIds_OBJ_SHRUB4);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/shrubs/trop_shrub_05.dds",
		TextureIds::TextureIds_OBJ_SHRUB5);

	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/hesco/diffuse.dds",
		TextureIds::TextureIds_OBJ_HESCO);

	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/money/money.dds",
		TextureIds::TextureIds_OBJ_MONEY);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/palms/palm.dds",
		TextureIds::TextureIds_OBJ_PALM);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/plasticBarrel/plastic_barrel.dds",
		TextureIds::TextureIds_OBJ_PLASTIC_BARREL);
	
	if(SUCCEEDED(hr)) //HACK: use barrel's texture as sphere doesn't have one.
		hr = loadTexture(p_device, L"../../Resources/objects/plasticBarrel/plastic_barrel.dds",
		TextureIds::TextureIds_OBJ_SPHERE);

	// Previews
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/shrubs/preview.dds",
		TextureIds::TextureIds_PREVIEW_SHRUBS);

	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/hemps/preview.dds",
		TextureIds::TextureIds_PREVIEW_HEMPS);

	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/hesco/preview.dds",
		TextureIds::TextureIds_PREVIEW_HESCO);

	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/money/preview.dds",
		TextureIds::TextureIds_PREVIEW_MONEY);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/palms/preview.dds",
		TextureIds::TextureIds_PREVIEW_PALM);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/plasticBarrel/plastic_barrel.dds",
		TextureIds::TextureIds_PREVIEW_PLASTIC_BARREL);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/shrubs/trop_shrub_05.dds",
		TextureIds::TextureIds_PREVIEW_SHRUB1);
	if(SUCCEEDED(hr))
		hr = loadTexture(p_device, L"../../Resources/objects/shrubs/trop_shrub_03.dds",
		TextureIds::TextureIds_PREVIEW_SHRUB2);
	if(SUCCEEDED(hr)) //HACK: use barrel's texture as sphere doesn't have one.
		hr = loadTexture(p_device, L"../../Resources/objects/plasticBarrel/plastic_barrel.dds",
		TextureIds::TextureIds_PREVIEW_SPHERE);


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