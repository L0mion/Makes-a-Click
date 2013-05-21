#include "HeightMap.h"

#include "EntityBufferInfo.h"
#include "managementD3D.h"
#include "vertex.h"
#include "PivotPoint.h"
#include <math.h>

HeightMap::HeightMap(
	ManagementD3D*		p_managementD3D,
	std::vector<float>	p_heightmap,
	float				p_cellSize,
	unsigned int		p_cntCol,
	unsigned int		p_cntRow)
{
	m_managementD3D = p_managementD3D;
	m_heightMap		= p_heightmap;
	m_cellSize		= p_cellSize;
	m_colCnt		= p_cntCol;
	m_rowCnt		= p_cntRow;
	m_faceCnt		= (m_rowCnt-1) * (m_colCnt-1) * 2;

	int vertexCnt = m_rowCnt*m_colCnt;

	//system = pSystem;
	m_bufferInfo = NULL;

	createEntityBufferInfo();
}

HeightMap::~HeightMap()
{
	// This is done by the renderer 
	//delete m_bufferInfo;
}

//vector<Model> HeightMap::createModels(/*int pTechNr, int pPassNr, */int maxSize)
//{
//	// Not used or tested yet
//	////calculate number of models to produce:
//	//int numColModels = 0;
//	//int numRowModels = 0;
//	//unsigned int tmpNumCols = (unsigned int)nrOfCols;
//	//unsigned int tmpNumRows = (unsigned int)nrOfRows;
//	//while(tmpNumCols > maxSize)
//	//{
//	//	numColModels++;
//	//	tmpNumCols >>= 2; //2 shifts = 1/4
//	//}
//	//while(tmpNumRows > maxSize)
//	//{
//	//	numRowModels++;
//	//	tmpNumRows >>= 2; //2 shifts = 1/4
//	//}
//
//	int numVertices = nrOfRows*nrOfCols;
//	int numIndices = nrOfFaces*3;
//
//	vector<HeightMapVertex> vertices = defineVertexBuffer(numVertices);
//	vector<int> indices = defineIndexBuffer(numIndices);
//
//	
//	string blendMapPath = "assets/textures/blendmap3.png";
//	//vector<string> texFileNames(5);
//	//for(int i=0; i<5; i++)
//	//{
//	//	texFileNames[i] = "assets/textures/";
//	//}
//
//	//texFileNames[0] += "wood.png";
//	//texFileNames[1] += "bricks.jpg";
//	//texFileNames[2] += "perlin.jpg";
//	//texFileNames[3] += "tileBrick.jpg";
//	//texFileNames[4] += "stones.jpg";
//	
//	//Mesh* mesh = new Mesh(system, vertices, indices, pTechNr, pPassNr);
//	//Surface* surface = new Surface(system, texFileNames, uvScales, assetFolders, true);
//	vector<Model> models;
//	models.push_back(Model());
//	//Model model;
//	models[0].setVertices(vertices);
//	models[0].setIndices(indices);
//
//	//HACK: hardcoded textures and stuph
//	//No normalmaps or specularmaps
//	models[0].addMaterial("assets/textures/wood.png",		"assets/textures/blendmap3.png", "");
//	models[0].addMaterial("assets/textures/bricks.jpg",		"", "");
//	models[0].addMaterial("assets/textures/perlin.jpg",		"", "");
//	models[0].addMaterial("assets/textures/tileBrick.jpg",	"", "");
//	models[0].addMaterial("assets/textures/stones.jpg",		"", "");
//	models[0].name = "World heightmap";
//
//	//models.push_back(new Model(system, mesh, surface, TransformInfo()));
//	
//	return models;
//}

float HeightMap::getHeight( float p_x, float p_z )
{
	if((p_x > (m_colCnt-1)*m_cellSize*0.5f) || (p_x < -1*(m_colCnt-1)*m_cellSize*0.5f))
		return 5.0f; //HACK:
	if((p_z > (m_rowCnt-1)*m_cellSize*0.5f) || (p_z < -1*(m_rowCnt-1)*m_cellSize*0.5f))
		return 5.0f; //HACK:
	
	int col = getCol( p_x );
	int row = getRow( p_z );

	// Grab the heights of the cell we are in.
	// A 0--0 B
	//   | /|
	//   |/ |
	// C 0--0 D
	float A = getHeight(col,   row);
	float B = getHeight(col+1, row);
	float C = getHeight(col,   row+1);
	float D = getHeight(col+1, row+1);

	// Where we are relative to the cell.
	float s = getColDiff( p_x );
	float t = getRowDiff( p_z );

	if( s+t <= 1.0f )		// If upper triangle ABC.
	{
		float uy = B - A;
		float vy = C - A;
		return A + s*uy + t*vy;
	}
	else	// lower triangle DCB.
	{
		float uy = C - D;
		float vy = B - D;
		return D + (1.0f-s)*uy + (1.0f-t)*vy;
	}
}

float HeightMap::getHeight( int p_col, int p_row )
{
	return getHeight( p_row*m_colCnt + p_col );
}

float HeightMap::getHeight( int p_idx )
{
	return m_vertices[p_idx].position[Coords::Y];
}

EntityBufferInfo* HeightMap::getEntityBufferInfo()
{
	return m_bufferInfo;
}

void HeightMap::update( ManagementD3D* p_managementD3D, PivotPoint* p_pivot, float p_dt )
{

	// Modify heightmap 
	//float rad = 10.0f;
	float heightLimit = 25.60f;
	float lowLimit = 0.0f;
	float speedFac = 30.0f;
	p_pivot->m_speed *= speedFac;
	if( p_pivot->m_speed > 0.01f )
	{
		int rad = 10;
		int col = getCol( p_pivot->m_position.x );
		int row = getRow( p_pivot->m_position.z );

		int xStart	= col-rad;
		int xStop	= col+rad;
		int zStart	= row-rad;
		int zStop	= row+rad;

		for( int x=-rad; x<rad; x++ ) {
			for( int z=-rad; z<rad; z++ ) 
			{
				int idx = (z+row)*m_colCnt + (x + col);
				float height = m_vertices[idx].position[Coords::Y];
				if( height < heightLimit && height > lowLimit )
				{
					float xAbs = fabs((float)x/(float)rad);
					float zAbs = fabs((float)z/(float)rad);
					float amount = max( 0.0f, 1-(xAbs*xAbs + zAbs*zAbs) );
					amount *= p_dt * p_pivot->m_speed;

					m_vertices[idx].position[Coords::Y] += amount;
				}
			}
		}
		estimateNormals();
		//smoothHeightMap();

		// Copy to gpu
		ID3D11DeviceContext* devcon = p_managementD3D->getDeviceContext();
		D3D11_MAPPED_SUBRESOURCE resource;
		ZeroMemory( &resource, sizeof(D3D11_MAPPED_SUBRESOURCE) );
		HRESULT hResult = devcon->Map( m_bufferInfo->m_vertexBuffer, 0,
			D3D11_MAP_WRITE_DISCARD, 0, &resource);
		memcpy( resource.pData,& m_vertices[0], sizeof(Vertex_PNT)*m_vertexCnt );
		devcon->Unmap( m_bufferInfo->m_vertexBuffer, 0 );
	}
}

float HeightMap::getColDiff( float p_x )
{
	return getColAsFloat( p_x ) - (float)getCol( p_x );
}

int HeightMap::getCol( float p_x )
{
	// Transform from terrain local space to "cell" space.
	float col = getColAsFloat( p_x );
	return (int)floorf(col);
}

float HeightMap::getColAsFloat( float p_x )
{
	return (p_x + 0.5f*(m_colCnt-1)*m_cellSize) / m_cellSize;
}

float HeightMap::getRowDiff( float p_z )
{
	return getRowAsFloat( p_z ) - (float)getRow( p_z );
}

int HeightMap::getRow( float p_z )
{
	float row = getRowAsFloat( p_z );
	return (int)floorf(row);
}

float HeightMap::getRowAsFloat( float p_z )
{
	return (p_z - 0.5f*(m_rowCnt-1)*m_cellSize) / -m_cellSize;
}

void HeightMap::createEntityBufferInfo()
{
	m_vertexCnt = m_rowCnt*m_colCnt;
	m_indecCnt = m_faceCnt*3;
	
	defineVertexBuffer(m_vertexCnt);
	defineIndexBuffer(m_indecCnt);

	m_bufferInfo = new EntityBufferInfo();
	m_bufferInfo->setVertexBuffer( sizeof(Vertex_PNT), m_vertexCnt,
		&m_vertices[0], m_managementD3D );
	m_bufferInfo->setIndexBuffer( m_indecCnt, &m_indices[0], m_managementD3D );
	m_bufferInfo->m_textureId = TextureIds::TextureIds_HEIGHTMAP;
}

void HeightMap::defineVertexBuffer( int p_vertexCnt )
{
	int pos;
	float x, y, z;

	//create geometry
	m_vertices.resize( p_vertexCnt );
	float halfWidth = (m_colCnt-1)*m_cellSize*0.5f;
	float halfDepth = (m_rowCnt-1)*m_cellSize*0.5f;

	float du = 1.0f /*/ (nrOfCols-1)*/;
	float dv = 1.0f /*/ (nrOfRows-1)*/;

	for( int rowIdx=0; rowIdx<m_rowCnt; rowIdx++ ) {
		z = halfDepth - rowIdx*m_cellSize;
		for( int colIdx=0; colIdx<m_colCnt; colIdx++ ) {
			x = -halfWidth + colIdx*m_cellSize;

			//get height
			pos = rowIdx*m_colCnt + colIdx;
			y = m_heightMap[pos];

			m_vertices[pos].position[Coords::X]	= x;
			m_vertices[pos].position[Coords::Y]	= y;
			m_vertices[pos].position[Coords::Z]	= z;
			m_vertices[pos].normal[Coords::X]		= 0.0f; //HACK: hardcoded
			m_vertices[pos].normal[Coords::Y]		= 1.0f; //HACK: hardcoded
			m_vertices[pos].normal[Coords::Z]		= 0.0f; //HACK: hardcoded
			m_vertices[pos].texCoord[Coords::U]	= (float)(colIdx*du);
			m_vertices[pos].texCoord[Coords::V]	= (float)(rowIdx*dv);
		}
	}
	estimateNormals();
}

void HeightMap::estimateNormals()
{
	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f*m_cellSize);
	float invTwoDZ = 1.0f / (2.0f*m_cellSize);

	for( int rowIdx = 2; rowIdx <m_rowCnt-1; rowIdx++ ) {
		for( int colIdx = 2; colIdx < m_colCnt-1; colIdx++ ) {

			float t = getHeight(colIdx,   rowIdx-1);
			float b = getHeight(colIdx,   rowIdx+1);
			float l = getHeight(colIdx-1, rowIdx+1);
			float r = getHeight(colIdx+1, rowIdx+1);

			/*float t = m_heightMap[(rowIdx-1)*m_colCnt + colIdx];
			float b = m_heightMap[(rowIdx+1)*m_colCnt + colIdx];
			float l = m_heightMap[rowIdx*m_colCnt + colIdx - 1];
			float r = m_heightMap[rowIdx*m_colCnt + colIdx + 1];*/

			/*D3DXVECTOR3 tanZ(0.0f, (t-b)*invTwoDZ, 1.0f);
			D3DXVECTOR3 tanX(1.0f, (r-l)*invTwoDX, 0.0f);

			D3DXVECTOR3 N;
			D3DXVec3Cross(&N, &tanZ, &tanX);
			D3DXVec3Normalize(&N, &N);*/

			DirectX::XMVECTOR tanZ = DirectX::XMVectorSet( 0.0f, (t-b)*invTwoDZ, 1.0f, 0.0f );
			DirectX::XMVECTOR tanX = DirectX::XMVectorSet( 1.0f, (r-l)*invTwoDX, 0.0f, 0.0f );
			DirectX::XMVECTOR N = DirectX::XMVector3Cross( tanZ, tanX );
			N = DirectX::XMVector3Normalize(N);
			DirectX::XMFLOAT3 normal;
			DirectX::XMStoreFloat3( &normal, N );

			m_vertices[rowIdx*m_colCnt+colIdx].normal[Coords::X] = normal.x;
			m_vertices[rowIdx*m_colCnt+colIdx].normal[Coords::Y] = normal.y;
			m_vertices[rowIdx*m_colCnt+colIdx].normal[Coords::Z] = normal.z;
		}
	}
}

void HeightMap::defineIndexBuffer( int p_indexCnt )
{
	int k = 0;

	m_indices.resize( p_indexCnt );
	for( int rowIdx = 0; rowIdx<m_rowCnt-1; rowIdx++ ) {
		for( int colIdx = 0; colIdx<m_colCnt-1; colIdx++ ) {
			m_indices[k]   = rowIdx*m_colCnt+colIdx;
			m_indices[k+1] = rowIdx*m_colCnt+colIdx+1;
			m_indices[k+2] = (rowIdx+1)*m_colCnt+colIdx;

			m_indices[k+3] = (rowIdx+1)*m_colCnt+colIdx;
			m_indices[k+4] = rowIdx*m_colCnt+colIdx+1;
			m_indices[k+5] = (rowIdx+1)*m_colCnt+colIdx+1;

			k += 6; // next quad
		}
	}
}