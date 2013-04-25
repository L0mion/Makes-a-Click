#include "VertexBehaviorHeightmap.h"

VertexBehaviorHeightmap::VertexBehaviorHeightmap(	ID3D11Device* device,
													int numRows,
													int numColumns,
													float cellSpacing,
													float heightmapScale,
													string heightmapFilename)
{
	this->device			= device;
	this->numRows			= numRows;
	this->numColumns		= numRows;
	this->cellSpacing		= cellSpacing;
	this->heightmapScale	= heightmapScale;
	this->heightmapFilename	= heightmapFilename;

	this->numFaces = (numRows-1)*(numColumns-1)*2;
}

VertexBehaviorHeightmap::~VertexBehaviorHeightmap()
{
	device->Release();
	vertexBuffer->Release();
}

void VertexBehaviorHeightmap::loadHeightmap()
{
	vector<unsigned char> in(numRows*numColumns);

	ifstream infile;
	infile.open(heightmapFilename.c_str(), ios_base::binary);

	if(infile)
	{
		infile.read((char*)&in[0], (std::streamsize)in.size());
		infile.close();
	}
	heightmap.resize(numRows*numColumns, 0);
	for(unsigned int i=0; i<(numRows*numColumns); i++)
	{
		heightmap[i] = (float)in[i] * heightmapScale;
	}
}

float VertexBehaviorHeightmap::average(UINT i, UINT j)
{
	float avg = 0.0f;
	float num = 0.0f;

	for(UINT m = i-1; m <= i+1; m++)
	{
		for(UINT n = j-1; n <= j+1; n++)
		{
			if(inBounds(m, n))
			{
				avg += heightmap[m*numColumns + n];
				num += 1.0f;
			}
		}
	}
	return avg / num;
}

bool VertexBehaviorHeightmap::inBounds(UINT i, UINT j)
{
	return i >= 0 && i < numRows &&
		   j >= 0 && j < numColumns;
}

void VertexBehaviorHeightmap::smooth()
{
	std::vector<float> dest( heightmap.size() );

	for(UINT i = 0; i < numRows; i++)
	{
		for(UINT j = 0; j < numColumns; j++)
		{
			dest[i*numColumns+j] = average(i, j);
		}
	}

	heightmap = dest;
}

void VertexBehaviorHeightmap::createGrid()
{
	int numVertices = numRows*numColumns;
	int numFaces = (numRows-1)*(numColumns-1);

	vertices.resize(numVertices);

	float du = 1.0f/(numColumns-1);
	float dv = 1.0f/(numRows-1);

	float halfWidth = (numColumns-1) * cellSpacing * 0.5f;
	float halfDepth = (numRows-1) * cellSpacing * 0.5f;

	for(int i=0; i<numRows; i++)
	{
		float z = halfDepth - i*cellSpacing;
		for(int j=0; j<numColumns; j++)
		{
			float x = -halfWidth + j*cellSpacing;
			float y = heightmap[i*numColumns+j];

			vertices[i*numColumns+j].position	= D3DXVECTOR3(x, y, z);
			vertices[i*numColumns+j].normal		= D3DXVECTOR3(0.0f, 1.0f, 0.0f);
			vertices[i*numColumns+j].texcoord	= D3DXVECTOR2(j*du, i*dv);
		}
	}
}

void VertexBehaviorHeightmap::createVertexBuffer()
{
	int numVertices = numRows*numColumns;

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * numVertices;
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	device->CreateBuffer(&vbd, &vinitData, &vertexBuffer);
}


void VertexBehaviorHeightmap::constructVertexBuffer()
{
	loadHeightmap();
	smooth();
	createGrid();
	createVertexBuffer();
}

ID3D11Buffer* VertexBehaviorHeightmap::getVertexBuffer()
{
	return vertexBuffer;
}

vector<Vertex> VertexBehaviorHeightmap::getVertices()
{
	return vertices;
}

float VertexBehaviorHeightmap::getHeight(float x, float z)
{
	float c = (x + 0.5f*(numColumns-1)*cellSpacing) / cellSpacing;
	float d = (z - 0.5f*(numRows-1)*cellSpacing) / -cellSpacing;

	int row = (int)floorf(d);
	int col = (int)floorf(c);

	float A = heightmap[row*numColumns + col];
	float B = heightmap[row*numColumns + col + 1];
	float C = heightmap[(row+1)*numColumns + col];
	float D = heightmap[(row+1)*numColumns + col + 1];

	float s = c - (float)col;
	float t = d - (float)row;

	float height;

	if(s+t <= 1.0f)
	{
		float uy = B - A;
		float vy = C - A;
		height = A + s*uy + t*vy;
	}
	else
	{
		float uy = C - D;
		float vy = B - D;

		height = D + (1.0f-s)*uy + (1.0f-1)*vy;
	}

	return height;
}