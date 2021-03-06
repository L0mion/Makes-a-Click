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
		
		SpriteIds_SAND_BUCKET,
		SpriteIds_SAND_PROPERTY_0,
		SpriteIds_SAND_PROPERTY_1,
		SpriteIds_SAND_PROPERTY_2,
		SpriteIds_SAND_PROPERTY_3,

		SpriteIds_OBJECT,
		SpriteIds_OBJECT_PROPERTY_0,
		SpriteIds_OBJECT_PROPERTY_1,
		SpriteIds_OBJECT_PROPERTY_2,
		SpriteIds_OBJECT_PROPERTY_3,
		SpriteIds_OBJECT_PROPERTY_4,
		SpriteIds_OBJECT_PROPERTY_5,
		SpriteIds_OBJECT_PROPERTY_6,
		SpriteIds_OBJECT_PROPERTY_7,

		SpriteIds_TEXTURE_BRUSH,
		SpriteIds_TEXTURE_BRUSH_PROPERTY_0,
		SpriteIds_TEXTURE_BRUSH_PROPERTY_1,
		SpriteIds_TEXTURE_BRUSH_PROPERTY_2,

		SpriteIds_TOOL_ICON,
		SpriteIds_TOOL_PROPERTY_ICON,
		SpriteIds_TOOL_ICON_BACKGROUND,
		SpriteIds_TOOL_PROPERTY_ICON_BACKGROUND,

		SpriteIds_TEXT_BACK,
		SpriteIds_TEXT_BACKGROUND,
		SpriteIds_TEXT_HIGHLIGHTER,
		SpriteIds_TEXT,
		SpriteIds_TEXT_CURSOR,
		SpriteIds_TEXT_OUTPUT_BACKGROUND,

		SpriteIds_COUNT
	};
	enum SpriteCollectionIds
	{
		SpriteCollectionIds_NONE,
		SpriteCollectionIds_TOOLS_MENU,
		SpriteCollectionIds_SAND_PROPERTIES_MENU,
		SpriteCollectionIds_OBJECT_PROPERTIES_MENU,
		SpriteCollectionIds_TEXTURE_BRUSH_PROPERTIES_MENU,
		SpriteCollectionIds_TEXT_MENU
	};
	enum SectorIds
	{
		SectorIds_CENTER,
		SectorIds_SECTOR_0,
		SectorIds_SECTOR_1,
		SectorIds_SECTOR_2,
		SectorIds_SECTOR_3,
		SectorIds_SECTOR_4,
		SectorIds_SECTOR_5,
		SectorIds_SECTOR_6,
		SectorIds_SECTOR_7,

		SectorIds_SECTOR_0_A,
		SectorIds_SECTOR_0_B,
		SectorIds_SECTOR_0_X,
		SectorIds_SECTOR_0_Y,

		SectorIds_COUNT
	};

	void setSpriteCollection(SpriteCollectionIds spriteCollection);

	std::vector<Sprite*>* getSprites();
	std::vector<Sprite*>* getSpriteCollection();
	Sprite* getSprite(SpriteIds p_spriteId);

	DirectX::XMFLOAT2 getSectorCoords(SectorIds p_sectorId);

	ID3D11Buffer* getVertexBuffer();
	ID3D11Buffer* getIndexBuffer();

	HRESULT init(ID3D11Device* p_device);
private:

	HRESULT initVertexBuffer(ID3D11Device* p_device);
	HRESULT initIndexBuffer(ID3D11Device* p_device);

	void initSprites();
	void initPlaceHolderSprite();
	void initToolIconSprites(float p_aspectRatio);
	void initMenuSprites(float p_aspectRatio);
	void initToolSprites(Sprite* p_highlighter);
	void initSandPropertySprites(Sprite* p_highlighter);
	void initObjectPropertySprites(Sprite* p_highlighter);
	void initTextureBrushPropertySprites(Sprite* p_highlighter);
	void initTextMenuSprites(Sprite* p_highlighter, Sprite* p_circleBackground);
	void initSpriteCollection();

	std::vector<Vertex_PT> createVertices();
	std::vector<DWORD> createIndices();
	
	void setSpriteCollectionToolsMenu();
	void setSpriteCollectionSandPropertiesMenu();
	void setSpriteCollectionObjectPropertiesMenu();
	void setSpriteCollectionTextureBrushPropertiesMenu();
	void setSpriteCollectionTextMenu();
	void setSpriteCollectionNone();
	

	void calcSectorCoords(float p_aspectRatio);

	std::vector<Sprite*>* m_sprites;
	std::vector<Sprite*>* m_spriteCollection;

	ID3D11Buffer* m_vertexBuffer;
	ID3D11Buffer* m_indexBuffer;

	std::vector<DirectX::XMFLOAT2> m_sectorCoords;
};

#endif //MANAGEMENT_SPRITE_H