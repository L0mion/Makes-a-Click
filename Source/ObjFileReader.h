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

class ManagementD3D;
struct EntityBufferInfo;
struct ObjectMold;


class ObjFileReader
{
public:
	ObjFileReader();
	~ObjFileReader();

	//vector<Model> readFile(string pFolder, string pFileName, bool p_startAtZero);
	EntityBufferInfo* ebiFromFilename(string pFolder, string pFileName,
		bool p_startAtZero, ManagementD3D* p_d3d);
	ObjectMold* omFromFilename(string pFolder, string pFileName,
		bool p_startAtZero, ManagementD3D* p_d3d);
	int readFile( string p_folder, string p_fileName, bool p_startAtZero, ManagementD3D* p_d3d );

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

	/// \return -1 if the requested mold doesn't exist 
	int findMold( string p_filePath );

private:
	//Model currModel;
	//vector<Model> models;

	vector<Vertex_PNT> m_vertices;
	vector<int> m_indices;
	vector<ObjectMold*> m_molds;

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