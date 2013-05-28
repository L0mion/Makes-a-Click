#include "ObjectTool.h"

#include "EntityBufferInfo.h"
#include "PivotPoint.h"
#include "renderer.h"
#include "ObjectMold.h"

ObjectTool::ObjectTool( Renderer* p_renderer )
{
	m_timeSinceLastPlacement = 0.0f;

	readObjects( p_renderer );
}

ObjectTool::~ObjectTool()
{
	//done by reader
	//delete m_barrelMold;
}

void ObjectTool::update( float p_dt, Renderer* p_renderer, const int p_objectType, const PivotPoint* p_pivot )
{
	float placeSpeed = 0.2f;

	m_timeSinceLastPlacement += p_dt;

	if( m_timeSinceLastPlacement > placeSpeed ){
		if( p_pivot->getSpeed() > 0.5f ) {
			placeObject( p_renderer, (ObjectTypes)p_objectType, p_pivot );
			m_timeSinceLastPlacement = 0.0f;
		}
	}
}

void ObjectTool::placeObject( Renderer* p_renderer, const ObjectTypes p_objectType, const PivotPoint* p_pivot )
{
		EntityBufferInfo* barrel = new EntityBufferInfo();
		barrel->setFromMold( m_molds[p_objectType], p_renderer->getD3DManagement() );

		XMFLOAT3 pivotPos = p_pivot->getPosition();
		barrel->setSize( p_pivot->getSize() );
		barrel->setPosition( pivotPos );
		p_renderer->addEntity( barrel );
}

void ObjectTool::readObjects( Renderer* p_renderer )
{
	m_molds.resize( ObjectTypes_CNT );

	m_molds[ObjectTypes_FLOWER] = m_reader.omFromFilename( 
		"../../resources/objects/flower/", "trop_shrub_01.obj",
		false, p_renderer->getD3DManagement() );
	m_molds[ObjectTypes_FLOWER]->m_textureId =
		TextureIds::TextureIds_OBJ_FLOWER;

	m_molds[ObjectTypes_HEMP] = m_reader.omFromFilename( 
		"../../resources/objects/hemp/", "hemp.obj",
		false, p_renderer->getD3DManagement() );
	m_molds[ObjectTypes_HEMP]->m_textureId =
		TextureIds::TextureIds_OBJ_HEMP;

	m_molds[ObjectTypes_HESCO] = m_reader.omFromFilename( 
		"../../resources/objects/hesco/", "hesco_scraps.obj",
		false, p_renderer->getD3DManagement() );
	m_molds[ObjectTypes_HESCO]->m_textureId =
		TextureIds::TextureIds_OBJ_HESCO;

	m_molds[ObjectTypes_MONEY] = m_reader.omFromFilename( 
		"../../resources/objects/money/", "box.obj",
		false, p_renderer->getD3DManagement() );
	m_molds[ObjectTypes_MONEY]->m_textureId =
		TextureIds::TextureIds_OBJ_MONEY;

	m_molds[ObjectTypes_PALM] = m_reader.omFromFilename( 
		"../../resources/objects/palm/", "palm_straight.obj",
		false, p_renderer->getD3DManagement() );
	m_molds[ObjectTypes_PALM]->m_textureId =
		TextureIds::TextureIds_OBJ_PALM;

	m_molds[ObjectTypes_PLASTIC_BARRELL] = m_reader.omFromFilename( 
		"../../resources/objects/plasticBarrel/", "plastic_barrel_scaled.obj",
		false, p_renderer->getD3DManagement() );
	m_molds[ObjectTypes_PLASTIC_BARRELL]->m_textureId =
		TextureIds::TextureIds_OBJ_PLASTIC_BARREL;

	m_molds[ObjectTypes_SHRUB1] = m_reader.omFromFilename( 
		"../../resources/objects/shrub1/", "trop_shrub_05.obj",
		false, p_renderer->getD3DManagement() );
	m_molds[ObjectTypes_SHRUB1]->m_textureId =
		TextureIds::TextureIds_OBJ_SHRUB1;

	m_molds[ObjectTypes_SHRUB2] = m_reader.omFromFilename( 
		"../../resources/objects/shrub2/", "trop_shrub_03.obj",
		false, p_renderer->getD3DManagement() );
	m_molds[ObjectTypes_SHRUB2]->m_textureId =
		TextureIds::TextureIds_OBJ_SHRUB2;
} 
