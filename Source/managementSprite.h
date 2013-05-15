#ifndef MANAGEMENT_SPRITE_H
#define MANAGEMENT_SPRITE_H

#include <vector>

#include <d3d11.h>
#include <DirectXMath.h>


class Sprite;
struct Vertex_PT;

class ManagementSprite
{
public:
	ManagementSprite();
	~ManagementSprite();

	enum SpriteIds
	{
		SpriteIds_PLACEHOLDER,
		SpriteIds_CIRCLE_BACKGROUND,
		SpriteIds_CIRCLE_HIGHLIGHT,
		SpriteIds_COUNT
	};
	enum SpriteCollectionIds
	{
		SpriteCollectionIds_NONE,
		SpriteCollectionIds_TEXT_MENU
	};

	void setSpriteCollection(SpriteCollectionIds spriteCollection);

	std::vector<Sprite*>* getSprites();
	std::vector<Sprite*>* getSpriteCollection();
	Sprite* getSprite(SpriteIds p_spriteId);


	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();

	HRESULT init(ID3D11Device* p_device);
private:

	HRESULT initVertexBuffer(ID3D11Device* p_device);
	HRESULT initIndexBuffer(ID3D11Device* p_device);

	void initSprites();
	void initSpriteCollection();

	std::vector<Vertex_PT> createVertices();
	std::vector<DWORD> createIndices();
	
	void setSpriteCollectionTextMenu();
	void setSpriteCollectionNone();

	std::vector<Sprite*>* m_sprites;
	std::vector<Sprite*>* m_spriteCollection;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
};

#endif //MANAGEMENT_SPRITE_H