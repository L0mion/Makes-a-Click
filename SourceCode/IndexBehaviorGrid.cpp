#include "IndexBehaviorGrid.h"


IndexBehaviorGrid::IndexBehaviorGrid(ID3D11Device *device, int numRows, int numColumns)
{
	this->device = device;
	this->numRows = numRows;
	this->numColumns = numColumns;

	this->numFaces = (numRows-1)*(numColumns-1)*2;
}

IndexBehaviorGrid::~IndexBehaviorGrid()
{
	device->Release();
	indexBuffer->Release();
}

void IndexBehaviorGrid::createIndices()
{
	indices.resize(numFaces*3);

	int k=0;
	for(int i=0; i<(numRows-1); i++)
	{
		for(int j=0; j<(numColumns-1); j++)
		{
			indices[k]   = i*numColumns+j;
			indices[k+1] = i*numColumns+j+1;
			indices[k+2] = (i+1)*numColumns+j;
			
			indices[k+3] = (i+1)*numColumns+j;
			indices[k+4] = i*numColumns+j+1;
			indices[k+5] = (i+1)*numColumns+j+1;
			
			k += 6;
		}
	}
}

void IndexBehaviorGrid::createIndexBuffer()
{
	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(DWORD) * numFaces * 3;
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	device->CreateBuffer(&ibd, &iinitData, &indexBuffer);
}

void IndexBehaviorGrid::constructIndexBuffer()
{
	createIndices();
	createIndexBuffer();
}

ID3D11Buffer* IndexBehaviorGrid::getIndexBuffer()
{
	return indexBuffer;
}

int IndexBehaviorGrid::getIndexCount()
{
	return indices.size();
}