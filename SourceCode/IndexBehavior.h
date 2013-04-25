#ifndef INDEXBEHAVIOR_H
#define INDEXBEHAVIOR_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>


class IndexBehavior
{
public:
	virtual void constructIndexBuffer() = 0;
	virtual ID3D11Buffer* getIndexBuffer() = 0;
	virtual int getIndexCount() = 0;
};

#endif