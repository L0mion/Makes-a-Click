#ifndef GAMEOBJECT_H
#define GAMEOBJECT_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include <fstream>
#include <vector>

#include "DrawBehaviorIndexed.h"
#include "VertexBehaviorHeightmap.h"
#include "IndexBehaviorGrid.h"

#include "Utility.h"
#include "Vertex.h"

#include "CBuffer.h"

class GameObject
{
private:
	ID3D11Device				*device;
	ID3D11DeviceContext			*devcon;

	ID3D11ShaderResourceView	*texture;
	LPCTSTR textureFilename; 

	DrawBehavior	*drawBehavior;
	VertexBehavior	*vertexBehavior;
	IndexBehavior	*indexBehavior;

	D3DXMATRIX worldMatrix;
	D3DXMATRIX translationMatrix;
	D3DXMATRIX scalingMatrix;
	D3DXMATRIX rotationMatrix;
	D3DXMATRIX textureScaleMatrix;

	D3DXVECTOR3 position;
	D3DXVECTOR3 scale;
	D3DXVECTOR3 rotation;

	void createTexture();
	void initialize();

public:
	GameObject(	ID3D11Device *device,
				ID3D11DeviceContext *devcon,
				LPCTSTR textureFilename,
				DrawBehavior *drawBehavior,
				VertexBehavior *vertexBehavior,
				IndexBehavior *indexBehavior);

	~GameObject();

	void draw();

	DrawBehavior* getDrawBehavior() const;
	VertexBehavior* getVertexBehavior() const;
	IndexBehavior* getIndexBehavior() const;
	
	void setTranslation(D3DXVECTOR3 translation);
	void setScale(D3DXVECTOR3 scale);
	void setRotation(D3DXVECTOR3 rotation);
	void setTextureScale(D3DXVECTOR3 textureScale);

	D3DXMATRIX getWorldMatrix();
	D3DXMATRIX getTranslationMatrix() const;
	D3DXMATRIX getScalingMatrix() const;
	D3DXMATRIX getRotationMatrix() const;
	D3DXMATRIX getTextureScaleMatrix() const;

	D3DXVECTOR3 getPosition();
	D3DXVECTOR3 getScale();
	D3DXVECTOR3 getRotation();
};


#endif