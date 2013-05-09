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
		setSpriteCollectionTextMenu();
		break;
	default:
		setSpriteCollectionNone();
		break;
	}
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

	std::vector<SpriteVertex> vertices = createVertices();

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * vertices.size();
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
	float aspectRatio = static_cast<float>(SCREEN_WIDTH)/static_cast<float>(SCREEN_HEIGHT);

	m_sprites = new std::vector<Sprite*>();
	m_sprites->resize(SpriteIds_COUNT, NULL);

	m_sprites->at(SpriteIds_PLACEHOLDER) = new Sprite(0.75f, 0.75f, 0.0f, 0.0f, 0.20f, 0.20f, TextureIds::TextureIds_PLACEHOLDER);
	m_sprites->at(SpriteIds_CIRCLE_BACKGROUND) = new Sprite(0.0f, 0.0f, 0.0f, 0.0f, 1.0f/aspectRatio, 1.0f, TextureIds::TextureIds_CIRCLE_BACKGROUND);
	m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT) = new Sprite(0.0f, 0.0f, 0.0f, 0.0f, 0.2f/aspectRatio, 0.2f, TextureIds::TextureIds_CIRCLE_HIGHLIGHT);

	Sprite* circle		= m_sprites->at(ManagementSprite::SpriteIds_CIRCLE_BACKGROUND);
	Sprite* highlighter = m_sprites->at(ManagementSprite::SpriteIds_CIRCLE_HIGHLIGHT);

	float height = circle->getScale().y - highlighter->getScale().y;
	float width = height / aspectRatio;

	m_sprites->at(SpriteIds_SAND_BUCKET) = new Sprite(0.0f,
		height,
		0.0f,
		0.0f,
		highlighter->getScale().x,
		highlighter->getScale().y,
		TextureIds::TextureIds_SANB_BUCKET);
	
	m_sprites->at(SpriteIds_OBJECT) = new Sprite(width*sin(DirectX::XM_PIDIV4),
		height * cos(DirectX::XM_PIDIV4),
		0.0f,
		0.0f,
		highlighter->getScale().x,
		highlighter->getScale().y,
		TextureIds::TextureIds_OBJECT);
}
void ManagementSprite::initSpriteCollection()
{
	m_spriteCollection = new std::vector<Sprite*>();
}

std::vector<SpriteVertex> ManagementSprite::createVertices()
{
	std::vector<SpriteVertex> vertices;

	vertices.push_back(SpriteVertex(DirectX::XMFLOAT3(-1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 0.0f)));
	vertices.push_back(SpriteVertex(DirectX::XMFLOAT3(1.0f, 1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 0.0f)));
	vertices.push_back(SpriteVertex(DirectX::XMFLOAT3(-1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(0.0f, 1.0f)));
	vertices.push_back(SpriteVertex(DirectX::XMFLOAT3(1.0f, -1.0f, 0.0f), DirectX::XMFLOAT2(1.0f, 1.0f)));

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

void ManagementSprite::setSpriteCollectionTextMenu()
{
	m_spriteCollection->clear();
	
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_CIRCLE_BACKGROUND));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_CIRCLE_HIGHLIGHT));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_SAND_BUCKET));
	m_spriteCollection->push_back(m_sprites->at(SpriteIds_OBJECT));
}
void ManagementSprite::setSpriteCollectionNone()
{
	m_spriteCollection->clear();
}