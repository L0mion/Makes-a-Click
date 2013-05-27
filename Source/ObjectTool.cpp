#include "ObjectTool.h"

#include "EntityBufferInfo.h"
#include "PivotPoint.h"
#include "renderer.h"
#include "ObjectMold.h"

ObjectTool::ObjectTool( Renderer* p_renderer )
{
	readObjects( p_renderer );
}

ObjectTool::~ObjectTool()
{
	//done by reader
	//delete m_barrelMold;
}

void ObjectTool::placeObject( Renderer* p_renderer,
	const ObjectTypes p_objectType, const PivotPoint* p_pivot )
{
	if( p_pivot->getSpeed() > 0.5f ) {
		
		EntityBufferInfo* barrel = new EntityBufferInfo();
		barrel->setFromMold( m_barrelMold, p_renderer->getD3DManagement() );

		barrel->m_world._41 = p_pivot->m_avatar->m_world._41;
		barrel->m_world._42 = p_pivot->m_avatar->m_world._42;
		barrel->m_world._43 = p_pivot->m_avatar->m_world._43;
		p_renderer->addEntity( barrel );
	}
}

void ObjectTool::readObjects( Renderer* p_renderer )
{
	readBarrel( p_renderer );
} 

void ObjectTool::readBarrel( Renderer* p_renderer )
{
	m_barrelMold = m_reader.omFromFilename( "../../resources/",
		"plastic_barrel_scaled.obj", false, p_renderer->getD3DManagement() );
}
