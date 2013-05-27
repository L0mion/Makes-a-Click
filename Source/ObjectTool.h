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
		ObjectTypes_BARRELL
	};

	ObjectTool(  Renderer* p_renderer );
	virtual ~ObjectTool();

	void placeObject( Renderer* p_renderer,
		const ObjectTypes p_objectType, const PivotPoint* p_pivot );

private:
	void readObjects( Renderer* p_renderer );
	void readBarrel( Renderer* p_renderer );

private:
	ObjFileReader m_reader;

	ObjectMold* m_barrelMold;

};
