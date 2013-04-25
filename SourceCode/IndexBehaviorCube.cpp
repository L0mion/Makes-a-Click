#include "IndexBehaviorCube.h"

IndexBehaviorCube::IndexBehaviorCube(ID3D11Device *device)
{
	this->device = device;
}

IndexBehaviorCube::~IndexBehaviorCube()
{
	device->Release();
	indexBuffer->Release();
}

void IndexBehaviorCube::createIndices()
{
	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(7);

	indices.push_back(8);
	indices.push_back(9);
	indices.push_back(10);

	indices.push_back(10);
	indices.push_back(9);
	indices.push_back(11);

	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(14);

	indices.push_back(14);
	indices.push_back(13);
	indices.push_back(15);

	indices.push_back(16);
	indices.push_back(17);
	indices.push_back(18);

	indices.push_back(18);
	indices.push_back(17);
	indices.push_back(19);

	indices.push_back(20);
	indices.push_back(21);
	indices.push_back(22);

	indices.push_back(22);
	indices.push_back(21);
	indices.push_back(23);
}

void IndexBehaviorCube::createIndexBuffer()
{
	int numFaces = 6*2;

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

void IndexBehaviorCube::constructIndexBuffer()
{
	createIndices();
	createIndexBuffer();
}

ID3D11Buffer* IndexBehaviorCube::getIndexBuffer()
{
	return indexBuffer;
}

int IndexBehaviorCube::getIndexCount()
{
	return indices.size();
}