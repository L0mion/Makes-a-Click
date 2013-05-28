#include "ObjectTool.h"

#include "EntityBufferInfo.h"
#include "HeightMap.h"
#include "ObjectMold.h"
#include "PivotPoint.h"
#include "renderer.h"
#include <DirectXMath.h>
#include <MacDesc.h>
#include <cstdlib>
#include <ctime>

ObjectTool::ObjectTool( Renderer* p_renderer, std::vector<Util::MacObject> p_macObjects )
{
	m_timeSinceLastPlacement = 0.0f;

	readObjects( p_renderer );
	m_macObjects = p_macObjects;
	placeObjects( p_renderer );

	srand((unsigned int)time(NULL));
	m_macObjects = p_macObjects;

}

ObjectTool::~ObjectTool()
{
	//done by reader
	//delete m_barrelMold;
	for( int i=0; i<ObjectTypes_CNT; i++ ) {
		for( unsigned int j=0; j<m_molds[i].size(); j++ ) {
			SAFE_DELETE(m_molds[i][j]);
		}
	}

}

void ObjectTool::update( float p_dt, Renderer* p_renderer,
	const int p_objectType, const PivotPoint* p_pivot, const HeightMap* p_heightmap )
{
	float placeSpeed = 0.2f;

	m_timeSinceLastPlacement += p_dt;

	if( m_timeSinceLastPlacement > placeSpeed ){
		placeObject( p_renderer, (ObjectTypes)p_objectType, p_pivot, p_heightmap );
		m_timeSinceLastPlacement = 0.0f;
	}
}

void ObjectTool::placeObject( Renderer* p_renderer,
	const ObjectTypes p_objectType, const PivotPoint* p_pivot,
	const HeightMap* p_heightmap )
{
	if( m_molds[p_objectType].size() > 0 )
	{
		float diameter = p_pivot->getSize()*2;
		float area = diameter * DirectX::XM_PI; 
		int numObjsToPlace = (int) area * p_pivot->getSpeed();
		for( int i=0; i<numObjsToPlace; i++ ) {
			int idx = rand() % m_molds[p_objectType].size();

			EntityBufferInfo* obj = new EntityBufferInfo();
			obj->setFromMold( m_molds[p_objectType][idx], p_renderer->getD3DManagement() );


			XMFLOAT3 pos = getRandomPos( p_heightmap, p_pivot->getPosition(), p_pivot->getSize() );
			obj->setPosition( pos );

			Util::MacObject object;

			// EVIL HACK! SAVE THEZE F*****S! 
			int categoryIdx = p_objectType; 
			int modelIdx = idx;

			object.name = Util::Object_Type_Barrel;
			object.posX = pos.x;
			object.posY = pos.y;
			object.posZ = pos.z;
			m_macObjects.push_back(object);

			/*obj->setSize( p_pivot->getSize() );*/
			p_renderer->addEntity( obj );
		}
	}
}

std::vector<Util::MacObject> ObjectTool::getMacObjects()
{
	return m_macObjects;
}

void ObjectTool::readObjects( Renderer* p_renderer )
{
	//readHemp( p_renderer );
	readShrubs( p_renderer ); 
	//readMoney( p_renderer );
	//readStones( p_renderer ); //hesco
	//readPalms( p_renderer );
	//readBarrels( p_renderer );
} 

void ObjectTool::readHemp( Renderer* p_renderer )
{
	addMold( ObjectTypes_HEMPS, TextureIds::TextureIds_OBJ_HEMP, 
		"../../resources/objects/hemps/", "hemp.obj", 2.0f, p_renderer );
	addMold( ObjectTypes_HEMPS, TextureIds::TextureIds_OBJ_HEMP, 
		"../../resources/objects/hemps/", "hemp_flowering.obj", 4.0f, p_renderer );
}

void ObjectTool::readShrubs( Renderer* p_renderer )
{
	addMold( ObjectTypes_SHRUBS, TextureIds::TextureIds_OBJ_SHRUB1, 
		"../../resources/objects/shrubs/", "trop_shrub_01.obj", 0.3f, p_renderer );
	addMold( ObjectTypes_SHRUBS, TextureIds::TextureIds_OBJ_SHRUB2, 
		"../../resources/objects/shrubs/", "trop_shrub_02.obj", 0.3f, p_renderer );
	addMold( ObjectTypes_SHRUBS, TextureIds::TextureIds_OBJ_SHRUB3, 
		"../../resources/objects/shrubs/", "trop_shrub_03.obj", 0.7f, p_renderer );
	addMold( ObjectTypes_SHRUBS, TextureIds::TextureIds_OBJ_SHRUB4, 
		"../../resources/objects/shrubs/", "trop_shrub_04.obj", 0.6f, p_renderer );
	addMold( ObjectTypes_SHRUBS, TextureIds::TextureIds_OBJ_SHRUB5, 
		"../../resources/objects/shrubs/", "trop_shrub_05.obj", 0.5f, p_renderer );
}

void ObjectTool::readMoney( Renderer* p_renderer )
{
	addMold( ObjectTypes_MONEY, TextureIds::TextureIds_OBJ_MONEY, 
		"../../resources/objects/money/", "batch_clean.obj", 0.15f, p_renderer );
	addMold( ObjectTypes_MONEY, TextureIds::TextureIds_OBJ_MONEY, 
		"../../resources/objects/money/", "batch_used.obj", 0.15f, p_renderer );
	addMold( ObjectTypes_MONEY, TextureIds::TextureIds_OBJ_MONEY, 
		"../../resources/objects/money/", "box.obj", 1.0f, p_renderer );
	addMold( ObjectTypes_MONEY, TextureIds::TextureIds_OBJ_MONEY, 
		"../../resources/objects/money/", "pack.obj", 1.5f, p_renderer );
	addMold( ObjectTypes_MONEY, TextureIds::TextureIds_OBJ_MONEY, 
		"../../resources/objects/money/", "single_burned.obj", 0.15f, p_renderer );
	addMold( ObjectTypes_MONEY, TextureIds::TextureIds_OBJ_MONEY, 
		"../../resources/objects/money/", "single_burned_bended.obj", 0.12f, p_renderer );
	addMold( ObjectTypes_MONEY, TextureIds::TextureIds_OBJ_MONEY, 
		"../../resources/objects/money/", "single_clean.obj", 0.15f, p_renderer );
	addMold( ObjectTypes_MONEY, TextureIds::TextureIds_OBJ_MONEY, 
		"../../resources/objects/money/", "single_clean_bended.obj", 0.12f, p_renderer );
}

void ObjectTool::readStones( Renderer* p_renderer )
{
	addMold( ObjectTypes_HESCO, TextureIds::TextureIds_OBJ_HESCO, 
		"../../resources/objects/hesco/", "hesco_custom.obj", 1.0f, p_renderer );
	addMold( ObjectTypes_HESCO, TextureIds::TextureIds_OBJ_HESCO, 
		"../../resources/objects/hesco/", "hesco_empty.obj", 1.0f, p_renderer );
	addMold( ObjectTypes_HESCO, TextureIds::TextureIds_OBJ_HESCO, 
		"../../resources/objects/hesco/", "hesco_sand.obj", 1.0f, p_renderer );
	addMold( ObjectTypes_HESCO, TextureIds::TextureIds_OBJ_HESCO, 
		"../../resources/objects/hesco/", "hesco_scraps.obj", 1.0f, p_renderer );
	addMold( ObjectTypes_HESCO, TextureIds::TextureIds_OBJ_HESCO, 
		"../../resources/objects/hesco/", "hesco_stones.obj", 1.0f, p_renderer );
}

void ObjectTool::readPalms( Renderer* p_renderer )
{
	addMold( ObjectTypes_PALM, TextureIds::TextureIds_OBJ_PALM, 
		"../../resources/objects/palms/", "palm_bend.obj", 2.0f, p_renderer );
	addMold( ObjectTypes_PALM, TextureIds::TextureIds_OBJ_PALM, 
		"../../resources/objects/palms/", "palm_dual.obj", 3.5f, p_renderer );
	addMold( ObjectTypes_PALM, TextureIds::TextureIds_OBJ_PALM, 
		"../../resources/objects/palms/", "palm_dual_bend.obj", 2.5f, p_renderer );
	addMold( ObjectTypes_PALM, TextureIds::TextureIds_OBJ_PALM, 
		"../../resources/objects/palms/", "palm_straight.obj", 5.0f, p_renderer );
	addMold( ObjectTypes_PALM, TextureIds::TextureIds_OBJ_PALM, 
		"../../resources/objects/palms/", "palm_trio.obj", 2.0f, p_renderer );
}

void ObjectTool::readBarrels( Renderer* p_renderer )
{
	addMold( ObjectTypes_BARRELS, TextureIds::TextureIds_OBJ_PLASTIC_BARREL, 
		"../../resources/objects/plasticBarrel/", "plastic_barrel_scaled.obj", 1.0f, p_renderer );
}

void ObjectTool::addMold( const ObjectTypes p_type,
	const TextureIds::Id p_texture, const string& p_folder,
	const string& p_fileName, const float p_stdSize, Renderer* p_renderer )
{
	ObjectMold* mold = m_reader.omFromFilename( 
		p_folder, p_fileName,
		false, p_renderer->getD3DManagement() );
	mold->m_textureId = p_texture;
	mold->m_stdSize = p_stdSize;
	m_molds[p_type].push_back(mold); 
}

DirectX::XMFLOAT3 ObjectTool::getRandomPos( const HeightMap* p_heightmap,
	const XMFLOAT3& p_centerPos, const float p_size )
{
	int degrees = rand()%360;
	float rads = DirectX::XMConvertToRadians((float)degrees);
	float resMult = 100.0f;
	float hiResSize = (int)p_size*resMult;
	float hiResOffset = (float)( rand() % (int)hiResSize );
	float offset = hiResOffset/resMult;

	XMFLOAT3 pos;
	pos.x = p_centerPos.x + cos(rads)*offset;
	pos.z = p_centerPos.z + sin(rads)*offset;
	float height = p_heightmap->getHeight( (float)pos.x, (float)pos.z );
	pos.y = height;

	return pos;
}

void ObjectTool::placeObjects( Renderer* p_renderer )
{
	for(unsigned int i=0; i<m_macObjects.size(); i++)
	{
		if(m_macObjects[i].name == Util::Object_Type_Barrel)
		{
			EntityBufferInfo* barrel = new EntityBufferInfo();

			// EVIL HACK! HARDCODEING FROM HELL!
			int categoryIdx = 0;
			int modelIdx = 0;

			barrel->setFromMold( m_molds[categoryIdx][modelIdx], p_renderer->getD3DManagement() );

			XMFLOAT3 pos( m_macObjects[i].posX, m_macObjects[i].posY, m_macObjects[i].posZ );
			barrel->setPosition(pos);

			p_renderer->addEntity( barrel );
		}
	}
}
