#pragma once

#include <vector>

#include <DirectXMath.h>
#include "ObjFileReader.h"

namespace Util
{
	struct MacObject;
}

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

	ObjectTool(  Renderer* p_renderer, std::vector<Util::MacObject> p_macObjects );
	virtual ~ObjectTool();

	void placeObject( Renderer* p_renderer,
		const ObjectTypes p_objectType, const PivotPoint* p_pivot );

	std::vector<Util::MacObject> getMacObjects();
private:
	void readObjects( Renderer* p_renderer );
	void readBarrel( Renderer* p_renderer );

private:
	ObjFileReader m_reader;

	ObjectMold* m_barrelMold;
	std::vector<Util::MacObject> m_macObjects;
};
