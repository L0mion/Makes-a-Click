#pragma once

#include <vector>
#include <fstream>
using namespace std;

#include "Vertex.h"
#include "managementMenu.h"

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
	float getHeight( float p_x, float p_z );
	float getHeight( int p_col, int p_row );
	float getHeight( int p_idx );

	EntityBufferInfo* getEntityBufferInfo();

	void update( ManagementD3D* p_managementD3D, PivotPoint* p_pivot,
		float p_dt, ManagementMenu::ToolPropertyIds p_toolProperty );

	int getCol( float p_x );
	int getRow( float p_z );

	vector<float>& getHeightmap() { return m_heightMap; }

	void updateHeightmap();

private:
	void modifyHeight( int p_radius, int p_row, int p_col,
		float p_speed, float p_lowLimit, float p_hightLimit, float p_dt);
	void smoothHeight( int p_radius, int p_row, int p_col,
		float p_speed, float p_dt );

	// Heightmap specific
	float getColDiff( float p_x );
	float getColAsFloat( float p_x );
	float getRowDiff( float p_z );
	float getRowAsFloat( float p_z );

	void loadHeightMap(int p_vertexCnt);
	void smoothHeightMap();
	void smoothHeightMap( int p_colBegin, int p_colEnd, int p_rowBegin,
		int p_rowEnd );
	float average(int p_posRow, int p_posCol);

	bool inBounds( int p_idx );
	bool inBounds( int p_row, int p_col );

	void toInBounds( int* inout_colBegin, int* inout_colEnd,
		int* inout_rowBegin, int* inout_rowEnd );
	void toInBounds( int* inout_col, int* inout_row);

	// Generate EntityBufferInfo
	void createEntityBufferInfo();
	void defineVertexBuffer( int p_vertexCnt );
	void estimateNormals();
	void estimateNormals( int p_colBegin, int p_colEnd, int rowBegin, int rowEnd );
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