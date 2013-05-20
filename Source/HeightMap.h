#pragma once

#include <vector>
#include <fstream>
using namespace std;

#include "Vertex.h"

struct EntityBufferInfo;
class ManagementD3D;
class PivotPoint;

class HeightMap
{
public:
	HeightMap(
		ManagementD3D*		p_managementD3D,
		std::vector<float>	p_heightmap,
		float				p_cellSize,
		unsigned int		p_cntCol,
		unsigned int		p_cntRow);
	~HeightMap();
	//vector<Model> createModels(/*int pTechNr = 0, int pPassNr = 0, */int maxSize = 256);
	float getHeight(float p_x, float p_z);

	EntityBufferInfo* getEntityBufferInfo();

	void update( ManagementD3D* p_managementD3D, PivotPoint* p_pivot );

private:
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

	int m_vertexCnt;
	int m_indecCnt;
	vector<Vertex_PNT> m_vertices;
	vector<int> m_indices;
	vector<float> m_heightMap;

	EntityBufferInfo* m_bufferInfo;
};