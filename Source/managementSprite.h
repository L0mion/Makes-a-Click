#ifndef MANAGEMENT_SPRITE_H
#define MANAGEMENT_SPRITE_H

#include <vector>

#include <d3d11.h>
#include <DirectXMath.h>


class Sprite;
struct SpriteVertex;

class ManagementSprite
{
public:
	ManagementSprite();
	~ManagementSprite();

	enum SpriteIds
	{
		SpriteIds_PLACEHOLDER,
		SpriteIds_COUNT
	};

	std::vector<Sprite*>* getSprites();
	Sprite* getSprite(SpriteIds p_spriteId);

	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();

	HRESULT init(ID3D11Device* p_device);
private:

	HRESULT initVertexBuffer(ID3D11Device* p_device);
	HRESULT initIndexBuffer(ID3D11Device* p_device);

	void initSprites();

	std::vector<SpriteVertex> createVertices();
	std::vector<DWORD> createIndices();

	std::vector<Sprite*>* m_sprites;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;
};

#endif //MANAGEMENT_SPRITE_H