#pragma once

#include <DirectXMath.h>
#include "ObjFileReader.h"

using namespace DirectX;

class PivotPoint;
class Renderer;
struct ObjectMold;

class ObjectTool
{
public:

	enum ObjectTypes {
		ObjectTypes_FLOWER,
		ObjectTypes_HEMP,
		ObjectTypes_HESCO,
		ObjectTypes_MONEY,
		ObjectTypes_PALM,
		ObjectTypes_PLASTIC_BARRELL,
		ObjectTypes_SHRUB1,
		ObjectTypes_SHRUB2,
		ObjectTypes_CNT
	};

	ObjectTool(  Renderer* p_renderer );
	virtual ~ObjectTool();

	void update( float p_dt, Renderer* p_renderer, const int p_objectType, const PivotPoint* p_pivot );

private:
	void placeObject( Renderer* p_renderer,
		const ObjectTypes p_objectType, const PivotPoint* p_pivot );
	void readObjects( Renderer* p_renderer );


private:
	ObjFileReader m_reader;
	vector<ObjectMold*> m_molds;
	//ObjectMold* m_barrelMold;
	float m_timeSinceLastPlacement;

};
