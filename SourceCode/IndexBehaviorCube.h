#ifndef INDEXBEHAVIORCUBE_H
#define INDEXBEHAVIORCUBE_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include <vector>

#include "IndexBehavior.h"

using namespace std;

class IndexBehaviorCube : public IndexBehavior
{
private:
	ID3D11Device *device;
	ID3D11Buffer *indexBuffer;

	vector<DWORD> indices;

	void createIndices();
	void createIndexBuffer();
public:
	
	IndexBehaviorCube(ID3D11Device *device);
	~IndexBehaviorCube();

	virtual void constructIndexBuffer();
	virtual ID3D11Buffer* getIndexBuffer();
	virtual int getIndexCount();
};


#endif