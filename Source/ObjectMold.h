#pragma once

#include "textureIds.h"
#include "vertex.h"
#include <string>
#include <vector>

using namespace std;
using namespace DirectX;

struct ObjectMold
{
	string m_filePath;
	TextureIds::Id m_textureId;

	vector<Vertex_PNT> m_vertices;
	vector<int> m_indices;

	ObjectMold();
	virtual ~ObjectMold();
};