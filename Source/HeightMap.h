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
	HeightMap( ManagementD3D* p_managementD3D );
	~HeightMap();
	//vector<Model> createModels(/*int pTechNr = 0, int pPassNr = 0, */int maxSize = 256);
	float getHeight( float p_x, float p_z );
	float getHeight( int p_col, int p_row );
	float getHeight( int p_idx );

	EntityBufferInfo* getEntityBufferInfo();

	void update( ManagementD3D* p_managementD3D, PivotPoint* p_pivot, float p_dt );

private:
	// Heightmap specific
	float getColDiff( float p_x );
	int getCol( float p_x );
	float getColAsFloat( float p_x );
	float getRowDiff( float p_z );
	int getRow( float p_z );
	float getRowAsFloat( float p_z );

	void loadHeightMap(int p_vertexCnt);
	void smoothHeightMap();
	float average(int p_posRow, int p_posCol);
	bool inBounds(int i, int j);

	// Generate EntityBufferInfo
	void createEntityBufferInfo();
	void defineVertexBuffer( int p_vertexCnt );
	void estimateNormals();
	void defineIndexBuffer( int p_indexCnt );

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