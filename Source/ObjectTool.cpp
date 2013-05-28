#include "ObjectTool.h"

#include "EntityBufferInfo.h"
#include "PivotPoint.h"
#include "renderer.h"
#include "ObjectMold.h"
#include <MacDesc.h>

ObjectTool::ObjectTool( Renderer* p_renderer, std::vector<Util::MacObject> p_macObjects )
{
	readObjects( p_renderer );
	m_macObjects = p_macObjects;
	placeObjects(p_renderer);
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

		Util::MacObject object;
		object.name = Util::Object_Type_Barrel;
		object.posX = barrel->m_world._41;
		object.posY = barrel->m_world._42;
		object.posZ = barrel->m_world._43;
		m_macObjects.push_back(object);
	}
}

std::vector<Util::MacObject> ObjectTool::getMacObjects()
{
	return m_macObjects;
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

void ObjectTool::placeObjects( Renderer* p_renderer )
{
	for(unsigned int i=0; i<m_macObjects.size(); i++)
	{
		if(m_macObjects[i].name == Util::Object_Type_Barrel)
		{
			EntityBufferInfo* barrel = new EntityBufferInfo();
			barrel->setFromMold( m_barrelMold, p_renderer->getD3DManagement() );

			barrel->m_world._41 = m_macObjects[i].posX;
			barrel->m_world._42 = m_macObjects[i].posY;
			barrel->m_world._43 = m_macObjects[i].posZ;
			p_renderer->addEntity( barrel );
		}
	}
}