#pragma once

#include <vector>
#include <fstream>
using namespace std;

#include "Vertex.h"

struct EntityBufferInfo;
class ManagementD3D;

class HeightMap
{
public:
	HeightMap( ManagementD3D* p_managementD3D );
	~HeightMap();
	//vector<Model> createModels(/*int pTechNr = 0, int pPassNr = 0, */int maxSize = 256);
	float getHeight(float p_x, float p_z);

	EntityBufferInfo* getEntityBufferInfo();

private:
	// Heightmap specific
	void loadHeightMap(int p_vertexCnt);
	void smoothHeightMap();
	float average(int p_posRow, int p_posCol);
	bool inBounds(int i, int j);

	// Generate EntityBufferInfo
	void createEntityBufferInfo();
	vector<Vertex_PNT> defineVertexBuffer( int p_vertexCnt );
	vector<int> defineIndexBuffer( int p_indexCnt );

private: // Variables
	ManagementD3D* m_managementD3D;

	float m_cellSize;
	int m_colCnt;
	int m_rowCnt;
	int m_faceCnt;
	vector<float> m_heightMap;

	EntityBufferInfo* m_bufferInfo;
};