#ifndef INDEXBEHAVIORGRID_H
#define INDEXBEHAVIORGRID_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include <vector>

#include "IndexBehavior.h"

using namespace std;

class IndexBehaviorGrid : public IndexBehavior
{
private:

	ID3D11Device *device;
	ID3D11Buffer *indexBuffer;

	int numRows;
	int numColumns;
	int numFaces;

	vector<DWORD> indices;

	void createIndices();
	void createIndexBuffer();

public:

	IndexBehaviorGrid(ID3D11Device *device, int numRows, int numColumns);
	~IndexBehaviorGrid();

	virtual void constructIndexBuffer();
	virtual ID3D11Buffer* getIndexBuffer();
	virtual int getIndexCount();
};


#endif