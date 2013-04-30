#include "HeightMap.h"
HeightMap::HeightMap(/*System* pSystem*/)
{
	//system = pSystem;

	//Vars for the HeightMap
	cellSize = 1.0f;
	nrOfCols = 257;
	nrOfRows = 257;
	nrOfFaces = (nrOfRows-1) * (nrOfCols-1) * 2;

	int numVertices = nrOfRows*nrOfCols;
	loadHeightMap(numVertices);
	smoothHeightMap();
}

void HeightMap::loadHeightMap(int numVertices)
{
	float scale = 0.1f;
	float offset = 0.0f;

	vector<unsigned char> in(numVertices);
	ifstream inFile;
	inFile.open("assets/heightmaps/korb.raw",  std::ios_base::binary);

	if(inFile)
	{
		inFile.read((char*)&in[0], in.size());
		inFile.close();
	}

	heightMap.resize(numVertices, 0);
	for(int i=0; i<numVertices; i++)
		heightMap[i] = in[i]*scale + offset;

}

void HeightMap::smoothHeightMap()
{
	vector<float> dest(heightMap.size());

	for(int i=0; i < nrOfRows; i++)
		for(int j=0; j < nrOfCols; j++)
			dest[nrOfCols*i + j] = average(i,j);

	heightMap = dest;
}

float HeightMap::average(int posRow, int posCol)
{
	float avg = 0.0f;
	float num = 0.0f;

	for(int i = posRow-1; i <= posRow+1; i++)
	{
		for(int j = posCol-1; j <= posCol+1; j++)
		{
			if(inBounds(i,j))
			{
				avg += heightMap[i*nrOfCols + j];
				num += 1.0f;
			}
		}
	}

	return avg / num;
}

bool HeightMap::inBounds(int i, int j)
{
	return i >= 0 && i < nrOfRows &&
		   j >= 0 && j < nrOfCols;
}

vector<HeightMapVertex> HeightMap::defineVertexBuffer(int numVertices)
{
	int pos;
	float x, y, z;

	//create geometry
	vector<HeightMapVertex> vertices(numVertices);
	float halfWidth = (nrOfCols-1)*cellSize*0.5f;
	float halfDepth = (nrOfRows-1)*cellSize*0.5f;

	float du = 1.0f /*/ (nrOfCols-1)*/;
	float dv = 1.0f /*/ (nrOfRows-1)*/;


	for(int i=0; i<nrOfRows; i++)
	{
		z = halfDepth - i*cellSize;
		for(int j=0; j<nrOfCols; j++)
		{
			x = -halfWidth + j*cellSize;

			//get height
			pos = i*nrOfCols + j;
			y = heightMap[pos];

			vertices[pos].position[Coords::X]	= x;
			vertices[pos].position[Coords::Y]	= y;
			vertices[pos].position[Coords::Z]	= z;
			vertices[pos].normal[Coords::X]		= 0.0f; //HACK: hardcoded
			vertices[pos].normal[Coords::Y]		= 1.0f; //HACK: hardcoded
			vertices[pos].normal[Coords::Z]		= 0.0f; //HACK: hardcoded
			vertices[pos].texCoord[Coords::U]	= (float)(j*du);
			vertices[pos].texCoord[Coords::V]	= (float)(i*dv);
		}
	}

	// Estimate normals for interior nodes using central difference.
	float invTwoDX = 1.0f / (2.0f*cellSize);
	float invTwoDZ = 1.0f / (2.0f*cellSize);
	for(int i = 2; i <nrOfRows-1; ++i)
	{
		for(int j = 2; j < nrOfCols-1; ++j)
		{
			float t = heightMap[(i-1)*nrOfCols + j];
			float b = heightMap[(i+1)*nrOfCols + j];
			float l = heightMap[i*nrOfCols + j - 1];
			float r = heightMap[i*nrOfCols + j + 1];

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

			vertices[i*nrOfCols+j].normal[Coords::X] = normal.x;
			vertices[i*nrOfCols+j].normal[Coords::Y] = normal.y;
			vertices[i*nrOfCols+j].normal[Coords::Z] = normal.z;
		}
	}
	return vertices;
}

vector<int> HeightMap::defineIndexBuffer(int numIndices)
{
	int k = 0;

	vector<int> indices(numIndices);
	for(int i = 0; i<nrOfRows-1; i++)
	{
		for(int j = 0; j<nrOfCols-1; j++)
		{
			indices[k]   = i*nrOfCols+j;
			indices[k+1] = i*nrOfCols+j+1;
			indices[k+2] = (i+1)*nrOfCols+j;

			indices[k+3] = (i+1)*nrOfCols+j;
			indices[k+4] = i*nrOfCols+j+1;
			indices[k+5] = (i+1)*nrOfCols+j+1;

			k += 6; // next quad
		}
	}
	return indices;
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

float HeightMap::getHeight(float x, float z)
{
	if((x > (nrOfCols-1)*cellSize*0.5f) || (x < -1*(nrOfCols-1)*cellSize*0.5f))
		return 5.0f; //HACK:
	if((z > (nrOfRows-1)*cellSize*0.5f) || (z < -1*(nrOfRows-1)*cellSize*0.5f))
		return 5.0f; //HACK:


	// Transform from terrain local space to "cell" space.
	float c = (x + 0.5f*(nrOfCols-1)*cellSize) / cellSize;
	float d = (z - 0.5f*(nrOfRows-1)*cellSize) / -cellSize;

	// Get the row and column we are in.
	int row = (int)floorf(d);
	int col = (int)floorf(c);

	// Grab the heights of the cell we are in.
	// A 0--0 B
	//   | /|
	//   |/ |
	// C 0--0 D
	float A = heightMap[row*nrOfCols + col];
	float B = heightMap[row*nrOfCols + col + 1];
	float C = heightMap[(row+1)*nrOfCols + col];
	float D = heightMap[(row+1)*nrOfCols + col + 1];

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