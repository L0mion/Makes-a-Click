#pragma once

#include <vector>

#include <DirectXMath.h>
#include "ObjFileReader.h"
#include "textureIds.h"

namespace Util
{
	struct MacObject;
}

using namespace DirectX;

class HeightMap;
class PivotPoint;
class Renderer;
struct ObjectMold;


class ObjectTool
{
public:

	enum ObjectTypes {
		ObjectTypes_SHRUBS,
		ObjectTypes_HEMPS,
		ObjectTypes_HESCO,
		ObjectTypes_MONEY,
		ObjectTypes_PALM,
		ObjectTypes_BARRELS,
		ObjectTypes_SHRUB1,
		ObjectTypes_SHRUB2,
		ObjectTypes_CNT
	};

	ObjectTool(  Renderer* p_renderer, std::vector<Util::MacObject> p_macObjects );
	virtual ~ObjectTool();

	void update( float p_dt, Renderer* p_renderer, const int p_objectType,
		const PivotPoint* p_pivot, const HeightMap* p_heightmap);

	std::vector<Util::MacObject> getMacObjects();
private:
	void placeObject( Renderer* p_renderer, const ObjectTypes p_objectType,
		const PivotPoint* p_pivot, const HeightMap* p_heightmap );
	void readObjects( Renderer* p_renderer );
//<<<<<<< HEAD

	void readHemp( Renderer* p_renderer );
	void readShrubs( Renderer* p_renderer );
	void readMoney( Renderer* p_renderer );
	void readStones( Renderer* p_renderer );
	void readPalms( Renderer* p_renderer );
	void readBarrels( Renderer* p_renderer );

	void addMold( const ObjectTypes p_type, const TextureIds::Id p_texture,
		const string& p_folder, const string& p_fileName,
		const float p_stdSize, Renderer* p_renderer );

	XMFLOAT3 getRandomPos( const HeightMap* p_heightmap,
		const XMFLOAT3& p_centerPos, const float p_size );


private:
	ObjFileReader m_reader;
	vector<ObjectMold*> m_molds[ObjectTypes_CNT];
	//ObjectMold* m_barrelMold;
	float m_timeSinceLastPlacement;

//=======
//	void readBarrel( Renderer* p_renderer );
	void placeObjects( Renderer* p_renderer );
//
//private:
//	ObjFileReader m_reader;
//
//	ObjectMold* m_barrelMold;
	std::vector<Util::MacObject> m_macObjects;
//>>>>>>> 8eca735681a7a8a03481fcc59d55b77697601a7a
};
