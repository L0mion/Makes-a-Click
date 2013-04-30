#include "HeightMap.h"

#include "EntityBufferInfo.h"

HeightMap::HeightMap(/*System* pSystem*/)
{
	//system = pSystem;
	m_bufferInfo = NULL;

	//Vars for the HeightMap
	m_cellSize	= 1.0f;
	m_colCnt	= 257;
	m_rowCnt	= 257;
	m_faceCnt	= (m_rowCnt-1) * (m_colCnt-1) * 2;

	int vertexCnt = m_rowCnt*m_colCnt;
	loadHeightMap( vertexCnt );
	smoothHeightMap();
}

HeightMap::~HeightMap()
{
	delete m_bufferInfo;
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


	// Transform from terrain local space to "cell" space.
	float c = (p_x + 0.5f*(m_colCnt-1)*m_cellSize) / m_cellSize;
	float d = (p_z - 0.5f*(m_rowCnt-1)*m_cellSize) / -m_cellSize;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A 0--0 B
	//   | /|
	//   |/ |
	// C 0--0 D
	float A = m_heightMap[row*m_colCnt + col];
	float B = m_heightMap[row*m_colCnt + col + 1];
	float C = m_heightMap[(row+1)*m_colCnt + col];
	float D = m_heightMap[(row+1)*m_colCnt + col + 1];

	// Where we are relative to the cell.
	float s = c - (float)col;
	float t = d - (float)row;

	if( s + t <= 1.0f)		// If upper triangle ABC.
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

EntityBufferInfo* HeightMap::getEntityBufferInfo()
{
	return m_bufferInfo;
}

void HeightMap::loadHeightMap( int p_vertexCnt )
{
	float scale = 0.1f;
	float offset = 0.0f;

	vector<unsigned char> in(p_vertexCnt);
	ifstream inFile;
	inFile.open("assets/heightmaps/korb.raw",  std::ios_base::binary);

	if(inFile)
	{
		inFile.read((char*)&in[0], in.size());
		inFile.close();
	}

	m_heightMap.resize(p_vertexCnt, 0);
	for(int i=0; i<p_vertexCnt; i++) {
		m_heightMap[i] = in[i]*scale + offset;
	}
}

void HeightMap::smoothHeightMap()
{
	vector<float> dest(m_heightMap.size());

	for( int rowIdx=0; rowIdx<m_rowCnt; rowIdx++ ) {
		for( int colIdx=0; colIdx<m_colCnt; colIdx++ ) {
			dest[m_colCnt*rowIdx + colIdx] = average(rowIdx,colIdx);
		}
	}
	m_heightMap = dest;
}

float HeightMap::average(int p_posRow, int p_posCol)
{
	float avg = 0.0f;
	float num = 0.0f;

	for( int rowIdx = p_posRow-1; rowIdx <= p_posRow+1; rowIdx++ ) {
		for( int colIdx = p_posCol-1; colIdx <= p_posCol+1; colIdx++ ) {
			if(inBounds(rowIdx,colIdx)) {
				avg += m_heightMap[rowIdx*m_colCnt + colIdx];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

bool HeightMap::inBounds( int i, int j )
{
	return i >= 0 && i < m_rowCnt &&
		   j >= 0 && j < m_colCnt;
}



void HeightMap::createEntityBufferInfo()
{
	int numVertices = m_rowCnt*m_colCnt;
	int numIndices = m_faceCnt*3;
	
	vector<HeightMapVertex> vertices = defineVertexBuffer(numVertices);
	vector<int> indices = defineIndexBuffer(numIndices);
}

vector<HeightMapVertex> HeightMap::defineVertexBuffer( int p_vertexCnt )
{
	int pos;
	float x, y, z;

	//create geometry
	vector<HeightMapVertex> vertices( p_vertexCnt );
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

			vertices[pos].position[Coords::X]	= x;
			vertices[pos].position[Coords::Y]	= y;
			vertices[pos].position[Coords::Z]	= z;
			vertices[pos].normal[Coords::X]		= 0.0f; //HACK: hardcoded
			vertices[pos].normal[Coords::Y]		= 1.0f; //HACK: hardcoded
			vertices[pos].normal[Coords::Z]		= 0.0f; //HACK: hardcoded
			vertices[pos].texCoord[Coords::U]	= (float)(colIdx*du);
			vertices[pos].texCoord[Coords::V]	= (float)(rowIdx*dv);
		}
	}

	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f*m_cellSize);
	float invTwoDZ = 1.0f / (2.0f*m_cellSize);

	for( int rowIdx = 2; rowIdx <m_rowCnt-1; rowIdx++ ) {
		for( int colIdx = 2; colIdx < m_colCnt-1; colIdx++ ) {
			float t = m_heightMap[(rowIdx-1)*m_colCnt + colIdx];
			float b = m_heightMap[(rowIdx+1)*m_colCnt + colIdx];
			float l = m_heightMap[rowIdx*m_colCnt + colIdx - 1];
			float r = m_heightMap[rowIdx*m_colCnt + colIdx + 1];

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

			vertices[rowIdx*m_colCnt+colIdx].normal[Coords::X] = normal.x;
			vertices[rowIdx*m_colCnt+colIdx].normal[Coords::Y] = normal.y;
			vertices[rowIdx*m_colCnt+colIdx].normal[Coords::Z] = normal.z;
		}
	}
	return vertices;
}

vector<int> HeightMap::defineIndexBuffer( int p_indexCnt )
{
	int k = 0;

	vector<int> indices(p_indexCnt);
	for( int rowIdx = 0; rowIdx<m_rowCnt-1; rowIdx++ ) {
		for( int colIdx = 0; colIdx<m_colCnt-1; colIdx++ ) {
			indices[k]   = rowIdx*m_colCnt+colIdx;
			indices[k+1] = rowIdx*m_colCnt+colIdx+1;
			indices[k+2] = (rowIdx+1)*m_colCnt+colIdx;

			indices[k+3] = (rowIdx+1)*m_colCnt+colIdx;
			indices[k+4] = rowIdx*m_colCnt+colIdx+1;
			indices[k+5] = (rowIdx+1)*m_colCnt+colIdx+1;

			k += 6; // next quad
		}
	}
	return indices;
}