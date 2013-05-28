#include "ObjFileReader.h"
#include "vertex.h"
#include "EntityBufferInfo.h"
#include "managementD3D.h"
#include "ObjectMold.h"

ObjFileReader::ObjFileReader()
{
	//numVertices = 0;
	numIndices = 0;

	//TEST: split function
	//string a = "a b c  d ///   def/ghi   ";
	//deque<string> test1 = split(a, ' ');
	//deque<string> test2 = split(a, '/');
	//deque<string> test3 = split(a, 'd');
	//deque<string> test4 = split(a, 'm');

	m_maxSize = 1.0f;
}

ObjFileReader::~ObjFileReader()
{
	for( unsigned int i=0; i<m_molds.size(); i++ ) {
		SAFE_DELETE( m_molds[i] );
	}
}

/*vector<Model> ObjFileReader::readFile(string pFolder, string pFileName, bool p_startAtZero)
{
	// Clean from earlier uses of this object
	currModel.clear();
	models.clear();
	numIndices = 0;
	readNorm.clear();
	readPos.clear();
	readTexCoord.clear();
	materials.clear();
	
	// Create the object
	fileName = pFileName;
	folder = pFolder;
	startAtZero = p_startAtZero;

	string objFilePath = folder + fileName;
	fstream objFile(objFilePath);

	if(objFile)
	{
		deque<string> lines;
		string tmp;
		while(!objFile.eof())
		{
			getline(objFile,tmp);
			lines.push_back(tmp);
		}

		int count = 0;
		string line;
		// Words at a line is all characters that are grouped together and
		// separated from other words by ' '.
		vector<string> lineWords;
		//string prefix; // first word on a line
		for(unsigned int i=0; i<lines.size(); i++)
		{
			count++;

			line = lines[i];
			if(line.size() > 0) // protect from empty line
			{

				lineWords = split(line,' ');
				lineWords.erase(lineWords.begin());

				// Roughly sorted in number of occurences in .obj-files
				if(line[0] == 'v')
				{
					if(line[1] == 't') // vt = tex coords
						readTextureUV(lineWords);
					else if (line[1] == 'n') // vn = normals
						readNormals(lineWords);
					else	// v = pos coords
						readVertices(lineWords);
				}
				else if(line[0] == 'f')
				{
					readFaces(lineWords);
				}
				else if(line[0] == 'g')
				{
					createModel();
					int korv = 0;
				}
				else if(line[0] == 'm') // mtllib = file containing material definitions
				{
					readMtlFile(lineWords);
				}
				else if(line[0] == 'u') // usemtl = which material to use
				{
					for(unsigned int i=0; i<materials.size(); i++)
					{
						if(materials[i].mtlName == lineWords.front())
							currModel.addMaterial(materials[i]);
					}
				}

				//lineWords.clear(); //clear the deque from old entries
			}
		}
		createModel();
	}
	return models;
}*/

EntityBufferInfo* ObjFileReader::ebiFromFilename( string p_folder,
	string p_fileName, bool p_startAtZero , ManagementD3D* p_d3d )
{
	ObjectMold* mold = omFromFilename( p_folder, p_fileName, p_startAtZero,
		p_d3d ); 

	EntityBufferInfo* bufferInfo = new EntityBufferInfo();
	bufferInfo->setVertexBuffer( sizeof(Vertex_PNT), mold->m_vertices.size(),
		&mold->m_vertices[0], p_d3d );
	bufferInfo->setIndexBuffer( mold->m_indices.size(), &mold->m_indices[0], p_d3d );
	bufferInfo->m_textureId = mold->m_textureId;

	return bufferInfo;
}


ObjectMold* ObjFileReader::omFromFilename( string p_folder,
	string p_fileName, bool p_startAtZero, ManagementD3D* p_d3d )
{
	string objName = p_folder+p_fileName;
	int moldIdx = findMold( objName );
	if( moldIdx == -1 ) {
		moldIdx = readFile( p_folder, p_fileName, p_startAtZero, p_d3d );
	}

	return m_molds[moldIdx]; 
}

int ObjFileReader::readFile( string p_folder, string p_fileName,
	bool p_startAtZero, ManagementD3D* p_d3d )
{
	// Create the object
	fileName = p_fileName;
	folder = p_folder;
	string objFilePath = folder + fileName;

	int moldIdx = findMold( objFilePath );

	// Read file if the file hasn't been read
	if( moldIdx == -1 ) {
		fstream objFile(objFilePath);

		startAtZero = p_startAtZero;

		// Clean from earlier uses of this object
		/*currModel.clear();
		models.clear();*/
		numIndices = 0;
		readNorm.clear();
		readPos.clear();
		readTexCoord.clear();
		m_vertices.clear();
		m_indices.clear();
		m_maxSize = 0.0f;
		/*materials.clear();*/
		if(objFile)
		{
			deque<string> lines;
			string tmp;
			while(!objFile.eof())
			{
				getline(objFile,tmp);
				lines.push_back(tmp);
			}

			int count = 0;
			string line;
			// Words at a line is all characters that are grouped together and
			// separated from other words by ' '.
			vector<string> lineWords;
			//string prefix; // first word on a line
			for(unsigned int i=0; i<lines.size(); i++)
			{
				count++;

				line = lines[i];
				if(line.size() > 0) // protect from empty line
				{

					lineWords = split(line,' ');
					lineWords.erase(lineWords.begin());

					// Roughly sorted in number of occurences in .obj-files
					if(line[0] == 'v')
					{
						if(line[1] == 't') // vt = tex coords
							readTextureUV(lineWords);
						else if (line[1] == 'n') // vn = normals
							readNormals(lineWords);
						else	// v = pos coords
							readVertices(lineWords);
					}
					else if(line[0] == 'f')
					{
						readFaces(lineWords);
					}
					else if(line[0] == 'g')
					{
						/*createModel();*/
						int korv = 0;
					}
					else if(line[0] == 'm') // mtllib = file containing material definitions
					{
						//readMtlFile(lineWords);
					}
					else if(line[0] == 'u') // usemtl = which material to use
					{
						/*for(unsigned int i=0; i<materials.size(); i++)
						{
						if(materials[i].mtlName == lineWords.front())
						currModel.addMaterial(materials[i]);
						}*/
					}

					//lineWords.clear(); //clear the deque from old entries
				}
			}
			/*createModel();*/
			for( unsigned int i=0; i<m_vertices.size(); i++ ) {
				m_vertices[i].position[Coords::X] /= m_maxSize;
				m_vertices[i].position[Coords::Y] /= m_maxSize;
				m_vertices[i].position[Coords::Z] /= m_maxSize;
			}

		} else {
			throw exception("file not found in obj reader");
		}
		/*return models;*/
		ObjectMold* mold = new ObjectMold();
		mold->m_filePath = objFilePath;
		mold->m_vertices = m_vertices;
		mold->m_indices = m_indices;
		mold->m_textureId = TextureIds::TextureIds_PLACEHOLDER;

		m_molds.push_back( mold );
		moldIdx = m_molds.size()-1;
	}
	return moldIdx;
}

/*void ObjFileReader::readMtlFile(vector<string> p_lineWords)
{
	//Read .mtl-file;
	string mtlFileName = p_lineWords[0];

	string line;
	string prefix;
	fstream mtlFile(folder + mtlFileName);

	if(mtlFile.good())
	{
		Material material;
		string mtlName;
		while(mtlFile.eof() == false)
		{
			string temp = "";
			stringstream lineStreamMtl;
			getline(mtlFile, line);
			lineStreamMtl << line;
			lineStreamMtl >> prefix;

			if(prefix == "newmtl")
			{
				if(material.mtlName != "")
				{
					materials.push_back(material);

					material.mtlName ="";
					material.texturePath = "";
					material.normalMapPath = "";
					material.specularMapPath = "";
				}
				lineStreamMtl >> material.mtlName;
			}
			// Diffuse and ambient are seen as the same component
			if(prefix == "map_Kd" || prefix == "map_Ka")
			{
				lineStreamMtl >> temp;
				material.texturePath = folder + temp;
			}
			// Specular map and specular highlight map are seen as the same
			else if(prefix == "map_Ks" || prefix == "map_Ns")
			{
				lineStreamMtl >> temp;
				material.specularMapPath = folder + temp;
			}
			// Bump maps are in this case seen as normal maps 
			else if(prefix == "map_bump" || prefix == "bump")
			{
				lineStreamMtl >> temp;
				material.normalMapPath = folder + temp;
			}
		}
		materials.push_back(material);
	}
}*/

void ObjFileReader::readNormals(vector<string> p_lineWords)
{
	array<float,3> normal;
	//_stream >> normal[Vertex::X] >> normal[Vertex::Y] >> normal[Vertex::Z];
	normal[Coords::X] = (float)atof(p_lineWords[0].c_str());
	normal[Coords::Y] = (float)atof(p_lineWords[1].c_str());
	normal[Coords::Z] = (float)atof(p_lineWords[2].c_str());
	readNorm.push_back(normal);
}

void ObjFileReader::readVertices(vector<string> p_lineWords)
{
	array<float,3> pos;
	//_stream >> pos[Vertex::X] >> pos[Vertex::Y] >> pos[Vertex::Z];
	pos[Coords::X] = (float)atof(p_lineWords[0].c_str());
	pos[Coords::Y] = (float)atof(p_lineWords[1].c_str());
	pos[Coords::Z] = (float)atof(p_lineWords[2].c_str());
	readPos.push_back(pos);

	if( pos[Coords::X] > m_maxSize ){
		m_maxSize = pos[Coords::X];
	} else if( pos[Coords::Y] > m_maxSize ){
		m_maxSize = pos[Coords::Y];
	} else if( pos[Coords::Z] > m_maxSize ){
		m_maxSize = pos[Coords::Z];
	}

}

void ObjFileReader::readTextureUV(vector<string> p_lineWords)
{
	array<float,2> uv;
	//_stream >> uv[Vertex::U] >> uv[Vertex::V];
	uv[Coords::U] = (float)atof(p_lineWords[0].c_str());
	uv[Coords::V] = (float)atof(p_lineWords[1].c_str());
	readTexCoord.push_back(uv);
}

void ObjFileReader::readFaces(vector<string> p_lineWords)
{
	if(p_lineWords.size() == 3) // 3 vertices = triangle
	{
		createFace(p_lineWords);
	}
	else if(p_lineWords.size() == 4) // 4 vertices = quad
	{
		createFace(triFromQuad(p_lineWords,1));
		createFace(triFromQuad(p_lineWords,2));
	}
}

void ObjFileReader::createFace(vector<string> p_lineWords)
{
	//char tmp; // Used to "eat" '/'

	// Init as 1 to protect from empty string stream. (0 as effective index)
	int indexPos = 1;
	int texPos = 1;
	int normPos = 1;

	vector<string> elements;
	for(int i=0; i<3; i++)
	{
		elements = split(p_lineWords[i], '/');
		Vertex_PNT vertex;
		indexPos	= atoi(elements[0].c_str());
		texPos		= atoi(elements[1].c_str());
		normPos		= atoi(elements[2].c_str());
	//	_stream >> indexPos >> tmp >> texPos >> tmp >> normPos;

		vertex.position[Coords::X] = readPos[indexPos-1][Coords::X];
		vertex.position[Coords::Y] = readPos[indexPos-1][Coords::Y];
		vertex.position[Coords::Z] = readPos[indexPos-1][Coords::Z];

		if(readTexCoord.size() > 0) // If the model has uv coords
		{
			vertex.texCoord[Coords::U] = readTexCoord[texPos-1][Coords::U];
			vertex.texCoord[Coords::V] = readTexCoord[texPos-1][Coords::V];
		}
		else
		{	// Else, Use hardcoded
			vertex.texCoord[Coords::U] = 0.3f*i;
			vertex.texCoord[Coords::V] = 0.3f*i;
		}
		
		if(!startAtZero)
		{	// Works for bth logo
			vertex.normal[Coords::X] = readNorm[normPos-1][Coords::X];
			vertex.normal[Coords::Y] = readNorm[normPos-1][Coords::Y];
			vertex.normal[Coords::Z] = readNorm[normPos-1][Coords::Z];
		}
		else
		{	// for teapot
			vertex.normal[Coords::X] = readNorm[normPos][Coords::X];
			vertex.normal[Coords::Y] = readNorm[normPos][Coords::Y];
			vertex.normal[Coords::Z] = readNorm[normPos][Coords::Z];
		}

		/*currModel.addVertex(vertex);
		currModel.addIndex(numIndices);*/
		m_vertices.push_back(vertex);
		m_indices.push_back(numIndices);

		numIndices++;
	}
}

/*void ObjFileReader::createModel()
{

	// If a model exists, push it back 
	if(currModel.getNumIndices() > 0)
	{
		currModel.setUseBlendMap(false); //Obj-files have no support for blendmaps
		currModel.name = fileName; // + " " + models.size();
		models.push_back(currModel);
	}

	// Start from phresh
	currModel.clear();
	numIndices = 0;
}*/

vector<string> ObjFileReader::split(string p_str, char p_delim)
{
	vector<string> strings;
	string currStr;

	for(unsigned int i=0; i<p_str.length(); i++)
	{
		if(p_str[i] != p_delim)
		{
			currStr.push_back(p_str[i]);
		}
		else
		{
			if(currStr.size() > 0) // Protect from trailing delimiters
				strings.push_back(currStr);
			currStr.clear();
		}
	}

	if(currStr.size() > 0) // Protect from trailing delimiters
		strings.push_back(currStr);

	return strings;
}

vector<string> ObjFileReader::triFromQuad(vector<string> p_lineWords, int p_triNum)
{
	vector<string> newLineWords;
	if(p_triNum == 1)
	{
		newLineWords.push_back(p_lineWords[0]);
		newLineWords.push_back(p_lineWords[1]);
		newLineWords.push_back(p_lineWords[2]);
	}
	else if(p_triNum == 2)
	{
		
		newLineWords.push_back(p_lineWords[0]);
		newLineWords.push_back(p_lineWords[2]);
		newLineWords.push_back(p_lineWords[3]);
	}
	return newLineWords;
}

int ObjFileReader::findMold( string p_filePath )
{
	for( unsigned int i=0; i<m_molds.size(); i++ ) {
		if( m_molds[i]->m_filePath == p_filePath ) {
			return i;
		}
	}

	return -1;
}
