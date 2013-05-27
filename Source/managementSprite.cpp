#include "managementSprite.h"
#include "sprite.h"
#include "vertex.h"
#include "utility.h"

ManagementSprite::ManagementSprite()
{
	m_vertexBuffer = NULL;
	m_indexBuffer  = NULL;

	m_sprites		   = NULL;
	m_spriteCollection = NULL;
}
ManagementSprite::~ManagementSprite()
{
	SAFE_RELEASE(m_vertexBuffer);
	SAFE_RELEASE(m_indexBuffer);

	for(unsigned int i=0; i<m_sprites->size(); i++)
		SAFE_DELETE(m_sprites->at(i));
	SAFE_DELETE(m_sprites);
	SAFE_DELETE(m_spriteCollection);
}

void ManagementSprite::setSpriteCollection(SpriteCollectionIds spriteCollection)
{
	switch(spriteCollection)
	{
	case SpriteCollectionIds_TOOLS_MENU:
		setSpriteCollectionToolsMenu();
		break;
	case SpriteCollectionIds_TEXT_MENU:
		setSpriteCollectionTextMenu();
		break;
	case SpriteCollectionIds_SAND_PROPERTIES_MENU:
		setSpriteCollectionSandPropertiesMenu();
		break;
	case SpriteCollectionIds_OBJECT_PROPERTIES_MENU:
		setSpriteCollectionObjectPropertiesMenu();
		break;
	case SpriteCollectionIds_TEXTURE_BRUSH_PROPERTIES_MENU:
		setSpriteCollectionTextureBrushPropertiesMenu();
		break;
	default:
		setSpriteCollectionNone();
		break;
	}

	
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TOOL_ICON));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TOOL_PROPERTY_ICON));
}

std::vector<Sprite*>* ManagementSprite::getSprites()
{
	return m_sprites;
}
std::vector<Sprite*>* ManagementSprite::getSpriteCollection()
{
	return m_spriteCollection;
}
Sprite* ManagementSprite::getSprite(SpriteIds p_spriteId)
{
	Sprite* sprite = NULL;
	if(p_spriteId >= 0 && p_spriteId < SpriteIds_COUNT)
		sprite = m_sprites->at(p_spriteId);
	else
		sprite = m_sprites->at(SpriteIds_PLACEHOLDER);

	return sprite;
}

DirectX::XMFLOAT2 ManagementSprite::getSectorCoords(SectorIds p_sectorId)
{
	DirectX::XMFLOAT2 coord = DirectX::XMFLOAT2(0.0f, 0.0f);
	if(p_sectorId >= 0 && p_sectorId < SectorIds_COUNT)
		coord = m_sectorCoords[p_sectorId];

	return coord;
}

ID3D11Buffer* ManagementSprite::getVertexBuffer()
{
	return m_vertexBuffer;
}
ID3D11Buffer* ManagementSprite::getIndexBuffer()
{
	return m_indexBuffer;
}

HRESULT ManagementSprite::init(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	initSprites();
	initSpriteCollection();

	hr = initVertexBuffer(p_device);
	if(SUCCEEDED(hr))
		hr = initIndexBuffer(p_device);

	return hr;
}
HRESULT ManagementSprite::initVertexBuffer(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	std::vector<Vertex_PT> vertices = createVertices();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex_CP) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	hr = p_device->CreateBuffer(&vbd, &vinitData, &m_vertexBuffer);

	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetSprite::initVertexBuffer() | CreateBuffer() | Failed", L"Sprite VertexBuffer", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}
HRESULT ManagementSprite::initIndexBuffer(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;

	std::vector<DWORD> indices = createIndices();

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(DWORD) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	hr = p_device->CreateBuffer(&ibd, &iinitData, &m_indexBuffer);

	if(FAILED(hr))
		MessageBox(NULL, L"ManagmenetSprite::initIndexBuffer() | CreateBuffer() | Failed", L"Sprite IndexBuffer", MB_OK | MB_ICONEXCLAMATION);

	return hr;
}

void ManagementSprite::initSprites()
{
	
	m_sprites = new std::vector<Sprite*>();
	m_sprites->resize(SpriteIds_COUNT, NULL);
	
	
	float aspectRatio = static_cast<float>(SCREEN_WIDTH)/
						static_cast<float>(SCREEN_HEIGHT);

	initPlaceHolderSprite();
	initToolIconSprites(aspectRatio);
	initMenuSprites(aspectRatio);

	calcSectorCoords(aspectRatio);

	initToolSprites(m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT));
	initSandPropertySprites(m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT));
	initObjectPropertySprites(m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT));
	initTextureBrushPropertySprites(m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT));
	
	initTextMenuSprites(
		m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT),
		m_sprites->at(SpriteIds_CIRCLE_BACKGROUND));
}

void ManagementSprite::initPlaceHolderSprite()
{
	m_sprites->at(SpriteIds_PLACEHOLDER) = new Sprite(0.75f,
		0.75f,
		0.0f,
		0.0f,
		0.20f,
		0.20f,
		TextureIds::TextureIds_PLACEHOLDER);
}
void ManagementSprite::initToolIconSprites(float p_aspectRatio)
{
	m_sprites->at(SpriteIds_TOOL_ICON) = new Sprite(-0.9f,
						0.85f,
						0.0f,
						0.0f,
						0.1f/p_aspectRatio,
						0.1f,
						TextureIds::TextureIds_SANB_BUCKET);

	m_sprites->at(SpriteIds_TOOL_PROPERTY_ICON) = new Sprite(0.9f,
						0.85f,
						0.0f,
						0.0f,
						0.1f/p_aspectRatio,
						0.1f,
						TextureIds::TextureIds_SANB_BUCKET);
}
void ManagementSprite::initMenuSprites(float p_aspectRatio)
{
	Sprite* sprite;
	sprite = new Sprite(0.0f,
						0.0f,
						0.0f,
						0.0f,
						0.8f/p_aspectRatio,
						0.8f,
						TextureIds::TextureIds_CIRCLE_BACKGROUND);
	m_sprites->at(SpriteIds_CIRCLE_BACKGROUND) = sprite;
	
	sprite = new Sprite(0.0f,
						0.0f,
						0.0f,
						0.0f,
						0.2f/p_aspectRatio,
						0.2f,
						TextureIds::TextureIds_CIRCLE_HIGHLIGHT);
	m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT) = sprite;

}
void ManagementSprite::initToolSprites(Sprite* p_highlighter)
{
	m_sprites->at(SpriteIds_SAND_BUCKET) = new Sprite(m_sectorCoords[SectorIds_SECTOR_0].x,
		m_sectorCoords[SectorIds_SECTOR_0].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_SANB_BUCKET);
	
	m_sprites->at(SpriteIds_OBJECT) = new Sprite(m_sectorCoords[SectorIds_SECTOR_1].x,
		m_sectorCoords[SectorIds_SECTOR_1].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_OBJECT);

	m_sprites->at(SpriteIds_TEXTURE_BRUSH) = new Sprite(m_sectorCoords[SectorIds_SECTOR_2].x,
		m_sectorCoords[SectorIds_SECTOR_2].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_TEXTURE_BRUSH);
}
void ManagementSprite::initSandPropertySprites(Sprite* p_highlighter)
{
	m_sprites->at(SpriteIds_SAND_PROPERTY_0) = new Sprite(m_sectorCoords[SectorIds_SECTOR_0].x,
		m_sectorCoords[SectorIds_SECTOR_0].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_SANB_BUCKET);

	m_sprites->at(SpriteIds_SAND_PROPERTY_1) = new Sprite(m_sectorCoords[SectorIds_SECTOR_1].x,
		m_sectorCoords[SectorIds_SECTOR_1].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_SANB_BUCKET);

	m_sprites->at(SpriteIds_SAND_PROPERTY_2) = new Sprite(m_sectorCoords[SectorIds_SECTOR_2].x,
		m_sectorCoords[SectorIds_SECTOR_2].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_SANB_BUCKET);

	m_sprites->at(SpriteIds_SAND_PROPERTY_3) = new Sprite(m_sectorCoords[SectorIds_SECTOR_3].x,
		m_sectorCoords[SectorIds_SECTOR_3].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_SANB_BUCKET);
}
void ManagementSprite::initObjectPropertySprites(Sprite* p_highlighter)
{
	m_sprites->at(SpriteIds_OBJECT_PROPERTY_0) = new Sprite(
		m_sectorCoords[SectorIds_SECTOR_0].x,
		m_sectorCoords[SectorIds_SECTOR_0].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_OBJECT);

	m_sprites->at(SpriteIds_OBJECT_PROPERTY_1) = new Sprite(
		m_sectorCoords[SectorIds_SECTOR_1].x,
		m_sectorCoords[SectorIds_SECTOR_1].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_OBJECT);

	m_sprites->at(SpriteIds_OBJECT_PROPERTY_2) = new Sprite(
		m_sectorCoords[SectorIds_SECTOR_2].x,
		m_sectorCoords[SectorIds_SECTOR_2].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_OBJECT);

	m_sprites->at(SpriteIds_OBJECT_PROPERTY_3) = new Sprite(
		m_sectorCoords[SectorIds_SECTOR_3].x,
		m_sectorCoords[SectorIds_SECTOR_3].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_OBJECT);
}
void ManagementSprite::initTextureBrushPropertySprites(Sprite* p_highlighter)
{
	float scaleFactor = 0.65f;
	m_sprites->at(SpriteIds_TEXTURE_BRUSH_PROPERTY_0) = new Sprite(
		m_sectorCoords[SectorIds_SECTOR_0].x,
		m_sectorCoords[SectorIds_SECTOR_0].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x * scaleFactor,
		p_highlighter->getScale().y * scaleFactor,
		TextureIds::TextureIds_STONE);

	m_sprites->at(SpriteIds_TEXTURE_BRUSH_PROPERTY_1) = new Sprite(
		m_sectorCoords[SectorIds_SECTOR_1].x,
		m_sectorCoords[SectorIds_SECTOR_1].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x * scaleFactor,
		p_highlighter->getScale().y * scaleFactor,
		TextureIds::TextureIds_GRASS);

	m_sprites->at(SpriteIds_TEXTURE_BRUSH_PROPERTY_2) = new Sprite(
		m_sectorCoords[SectorIds_SECTOR_2].x,
		m_sectorCoords[SectorIds_SECTOR_2].y,
		0.0f,
		0.0f,
		p_highlighter->getScale().x * scaleFactor,
		p_highlighter->getScale().y * scaleFactor,
		TextureIds::TextureIds_GRAVEL);
}
void ManagementSprite::initTextMenuSprites(Sprite* p_highlighter, Sprite* p_circleBackground)
{
	m_sprites->at(SpriteIds_TEXT_BACK) = new Sprite(
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		1.0f,
		1.0f,
		TextureIds::TextureIds_TEXT_BACK);

	m_sprites->at(SpriteIds_TEXT_BACKGROUND) = new Sprite(
		p_circleBackground->getPosition().x,
		p_circleBackground->getPosition().y,
		0.0f,
		0.0f, 
		p_circleBackground->getScale().x,
		p_circleBackground->getScale().y,
		TextureIds::TextureIds_TEXT_BACKGROUND);

	m_sprites->at(SpriteIds_TEXT_HIGHLIGHTER) = new Sprite(
		0.0f,
		0.0f,
		0.0f,
		0.0f,
		p_highlighter->getScale().x,
		p_highlighter->getScale().y,
		TextureIds::TextureIds_TEXT_HIGHLIGHTER);

	m_sprites->at(SpriteIds_TEXT) = new Sprite(
		p_circleBackground->getPosition().x,
		p_circleBackground->getPosition().y,
		0.0f,
		0.0f,
		p_circleBackground->getScale().x,
		p_circleBackground->getScale().y,
		TextureIds::TextureIds_TEXT);

	m_sprites->at(SpriteIds_TEXT_OUTPUT_BACKGROUND) = new Sprite(
		0.0f,
		-0.9f,
		0.0f,
		0.0f,
		1.0f,
		0.1f,
		TextureIds::TextureIds_TEXT_OUTPUT_BACKGROUND);

	float cursorHeight = 24.0f;
	float cursorWidth = 1.0f;
	m_sprites->at(SpriteIds_TEXT_CURSOR) = new Sprite(
		m_sprites->at(SpriteIds_TEXT_OUTPUT_BACKGROUND)->getPosition().x,
		m_sprites->at(SpriteIds_TEXT_OUTPUT_BACKGROUND)->getPosition().y,
		0.0f,
		0.0f,
		cursorWidth/SCREEN_WIDTH,
		cursorHeight/SCREEN_HEIGHT,
		TextureIds::TextureIds_TEXT_CURSOR);


}
void ManagementSprite::initSpriteCollection()
{
	m_spriteCollection = new std::vector<Sprite*>();
}

std::vector<Vertex_PT> ManagementSprite::createVertices()
{
	std::vector<Vertex_PT> vertices;

	vertices.push_back(Vertex_PT(DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
	vertices.push_back(Vertex_PT(DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
	vertices.push_back(Vertex_PT(DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));
	vertices.push_back(Vertex_PT(DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));

	return vertices;
}
std::vector<DWORD> ManagementSprite::createIndices()
{
	std::vector<DWORD> indices;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);
	indices.push_back(3);
	indices.push_back(2);
	indices.push_back(1);

	return indices;
}


void ManagementSprite::setSpriteCollectionToolsMenu()
{
	m_spriteCollection->clear();
	
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_CIRCLE_BACKGROUND));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_SAND_BUCKET));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_OBJECT));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TEXTURE_BRUSH));
}
void ManagementSprite::setSpriteCollectionSandPropertiesMenu()
{
	m_spriteCollection->clear();

	m_spriteCollection->push_back(m_sprites->at(SpriteIds_CIRCLE_BACKGROUND));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_SAND_PROPERTY_0));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_SAND_PROPERTY_1));
}
void ManagementSprite::setSpriteCollectionObjectPropertiesMenu()
{
	m_spriteCollection->clear();

	m_spriteCollection->push_back(m_sprites->at(SpriteIds_CIRCLE_BACKGROUND));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_OBJECT_PROPERTY_0));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_OBJECT_PROPERTY_1));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_OBJECT_PROPERTY_2));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_OBJECT_PROPERTY_3));
}
void ManagementSprite::setSpriteCollectionTextureBrushPropertiesMenu()
{
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_CIRCLE_BACKGROUND));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TEXTURE_BRUSH_PROPERTY_0));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TEXTURE_BRUSH_PROPERTY_1));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TEXTURE_BRUSH_PROPERTY_2));

}
void ManagementSprite::setSpriteCollectionTextMenu()
{
	m_spriteCollection->clear();

	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TEXT_BACK));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TEXT_BACKGROUND));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TEXT_HIGHLIGHTER));

	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TEXT));

	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TEXT_OUTPUT_BACKGROUND));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_TEXT_CURSOR));
}
void ManagementSprite::setSpriteCollectionNone()
{
	m_spriteCollection->clear();
}

void ManagementSprite::calcSectorCoords(float p_aspectRatio)
{
	Sprite* circle		= m_sprites->at(ManagementSprite::SpriteIds_CIRCLE_BACKGROUND);
	Sprite* highlighter = m_sprites->at(ManagementSprite::SpriteIds_CIRCLE_HIGHLIGHT);

	float backgroundHeight = circle->getScale().y - highlighter->getScale().y;
	float backgroundWidth = backgroundHeight / p_aspectRatio;

	float highlighterHeight = highlighter->getScale().y;
	float highlighterWidth  = highlighterHeight / p_aspectRatio;

	m_sectorCoords.resize(SectorIds_COUNT);

	m_sectorCoords[SectorIds_CENTER].x = 0.0f;
	m_sectorCoords[SectorIds_CENTER].y = 0.0f;

	m_sectorCoords[SectorIds_SECTOR_0].x = 0.0f;
	m_sectorCoords[SectorIds_SECTOR_0].y = backgroundHeight;

	m_sectorCoords[SectorIds_SECTOR_1].x = backgroundWidth * sin(DirectX::XM_PIDIV4);
	m_sectorCoords[SectorIds_SECTOR_1].y = backgroundHeight * cos(DirectX::XM_PIDIV4);

	m_sectorCoords[SectorIds_SECTOR_2].x = backgroundWidth;
	m_sectorCoords[SectorIds_SECTOR_2].y = 0.0f;

	m_sectorCoords[SectorIds_SECTOR_3].x = backgroundWidth * sin(DirectX::XM_PIDIV4);
	m_sectorCoords[SectorIds_SECTOR_3].y = -backgroundHeight * cos(DirectX::XM_PIDIV4);

	m_sectorCoords[SectorIds_SECTOR_4].x = 0.0f;
	m_sectorCoords[SectorIds_SECTOR_4].y = -backgroundHeight;

	m_sectorCoords[SectorIds_SECTOR_5].x = -backgroundWidth * sin(DirectX::XM_PIDIV4);
	m_sectorCoords[SectorIds_SECTOR_5].y = -backgroundHeight * cos(DirectX::XM_PIDIV4);

	m_sectorCoords[SectorIds_SECTOR_6].x = -backgroundWidth;
	m_sectorCoords[SectorIds_SECTOR_6].y = 0.0f;

	m_sectorCoords[SectorIds_SECTOR_7].x = -backgroundWidth * sin(DirectX::XM_PIDIV4);
	m_sectorCoords[SectorIds_SECTOR_7].y = backgroundHeight * cos(DirectX::XM_PIDIV4);


	m_sectorCoords[SectorIds_SECTOR_0_A].x = m_sectorCoords[SectorIds_SECTOR_0].x + 0.0f;
	m_sectorCoords[SectorIds_SECTOR_0_A].y = m_sectorCoords[SectorIds_SECTOR_0].y - highlighterHeight;

	m_sectorCoords[SectorIds_SECTOR_0_B].x = m_sectorCoords[SectorIds_SECTOR_0].x + highlighterWidth;
	m_sectorCoords[SectorIds_SECTOR_0_B].y = m_sectorCoords[SectorIds_SECTOR_0].y + 0.0f;
	
	m_sectorCoords[SectorIds_SECTOR_0_Y].x = m_sectorCoords[SectorIds_SECTOR_0].x + 0.0f;
	m_sectorCoords[SectorIds_SECTOR_0_Y].y = m_sectorCoords[SectorIds_SECTOR_0].y + highlighterHeight;
	
	m_sectorCoords[SectorIds_SECTOR_0_X].x = m_sectorCoords[SectorIds_SECTOR_0].x - highlighterWidth;
	m_sectorCoords[SectorIds_SECTOR_0_X].y = m_sectorCoords[SectorIds_SECTOR_0].y + 0.0f;
}