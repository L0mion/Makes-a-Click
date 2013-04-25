#ifndef VERTEXBEHAVIORHEIGHTMAP_H
#define VERTEXBEHAVIORHEIGHTMAP_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include <vector>
#include <fstream>
#include <string>

#include "VertexBehavior.h"
#include "Vertex.h"

using namespace std;

class VertexBehaviorHeightmap : public VertexBehavior
{
private:

	ID3D11Device* device;
	ID3D11Buffer* vertexBuffer;

	vector<float> heightmap;
	vector<Vertex> vertices;

	int numRows;
	int numColumns;
	int numFaces;
	
	float cellSpacing;
	float heightmapScale;

	string heightmapFilename;

	void loadHeightmap();
	float average(UINT i, UINT j);
	bool inBounds(UINT i, UINT j);
	void smooth();
	void createGrid();
	void createVertexBuffer();

public:

	VertexBehaviorHeightmap(ID3D11Device* device,
							int numRows,
							int numColumns,
							float cellSpacing,
							float heightmapScale,
							string heightmapFilename);

	~VertexBehaviorHeightmap();

	virtual void constructVertexBuffer();

	virtual ID3D11Buffer*	getVertexBuffer();
	virtual vector<Vertex>	getVertices();
	float					getHeight(float x, float y);

};

#endif