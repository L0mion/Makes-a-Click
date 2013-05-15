#ifndef OBJFILEREADER_H
#define OBJFILEREADER_H

#include <fstream>
#include <sstream>
#include <vector>
#include <deque>
#include <string>
#include <array>
#include "vertex.h"

//#include "Mesh.h"
//#include "Model.h"
//#include "GameObject.h"
//#include "TransformInfo.h"

using namespace std;

struct EntityBufferInfo;

class ManagementD3D;

class ObjFileReader
{
public:
	ObjFileReader();
	~ObjFileReader();

	//vector<Model> readFile(string pFolder, string pFileName, bool p_startAtZero);
	EntityBufferInfo* readFile(string pFolder, string pFileName,
		bool p_startAtZero, ManagementD3D* p_d3d);

private:
	void readObjFile();
	//void readMtlFile(vector<string> p_lineWords);
	void readNormals(vector<string> p_lineWords);
	void readVertices(vector<string> p_lineWords);
	void readTextureUV(vector<string> p_lineWords);
	void readFaces(vector<string> p_lineWords);
	/*void createModel();*/
	void createFace(vector<string> p_lineWords);

	vector<string> triFromQuad(vector<string> p_lineWords, int p_triNum);
	vector<string> split(string p_str, char p_delim);

private:
	//Model currModel;
	//vector<Model> models;

	vector<Vertex_PNT> m_vertices;
	vector<int> m_indices;

	string fileName;
	string folder;
	bool startAtZero;

	// File parsing
	int numIndices;
	vector<array<float,3>> readNorm;
	vector<array<float,3>> readPos;
	vector<array<float,2>> readTexCoord;
	
	//vector<string> mtlNames;
	//Material currMaterial;
	//vector<Material> materials;

	//vector<Vertex> vertices;

};

#endif //OBJFILEREADER_H