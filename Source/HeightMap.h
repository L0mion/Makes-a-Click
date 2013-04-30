#ifndef HEIGHTMAP_H
#define HEIGHTMAP_H

#include <vector>
#include <fstream>
using namespace std;

//#include "D3DObject.h"
//#include "Mesh.h"
//#include "Surface.h"
#include "Vertex.h"
//#include "GameObject.h"

class HeightMap
{
private:
	//System* system;

	float cellSize;
	int nrOfCols;
	int nrOfRows;
	int nrOfFaces;
	vector<float> heightMap;

	//Heightmap specific
	void loadHeightMap(int numVertices);
	void smoothHeightMap();
	float average(int posRow, int posCol);
	bool inBounds(int i, int j);

	// Generate Model
	vector<HeightMapVertex> defineVertexBuffer(int numVertices);
	vector<int> defineIndexBuffer(int numIndices);

public:
	HeightMap(/*System* pSystem*/);
	//vector<Model> createModels(/*int pTechNr = 0, int pPassNr = 0, */int maxSize = 256);
	float getHeight(float x, float z);
};

#endif //HEIGHTMAP_H