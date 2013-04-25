#include "GameObject.h"

GameObject::GameObject(	ID3D11Device *device,
						ID3D11DeviceContext *devcon,
						LPCTSTR textureFilename,
						DrawBehavior *drawBehavior,
						VertexBehavior *vertexBehavior,
						IndexBehavior *indexBehavior)
{
	this->device			= device;
	this->devcon			= devcon;
	this->textureFilename	= textureFilename;
	this->drawBehavior		= drawBehavior;
	this->vertexBehavior	= vertexBehavior;
	this->indexBehavior		= indexBehavior;

	D3DXMatrixIdentity(&worldMatrix);
	D3DXMatrixIdentity(&translationMatrix);
	D3DXMatrixIdentity(&scalingMatrix);
	D3DXMatrixIdentity(&rotationMatrix);
	D3DXMatrixIdentity(&textureScaleMatrix);

	position	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	scale		= D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	rotation	= D3DXVECTOR3(0.0f, 0.0f, 0.0f);

	initialize();
}

GameObject::~GameObject()
{
	device->Release();
	devcon->Release();
	texture->Release();

	if(drawBehavior != NULL)
		delete drawBehavior;
	
	if(vertexBehavior != NULL)
		delete vertexBehavior;

	if(indexBehavior != NULL)
		delete indexBehavior;
}

void GameObject::createTexture()
{
	D3DX11CreateShaderResourceViewFromFile(	device,
											textureFilename,
											NULL,
											NULL,
											&texture,
											NULL);
}

void GameObject::initialize()
{
	createTexture();
}

void GameObject::draw()
{
	devcon->PSSetShaderResources(0, 1, &texture);

	if(drawBehavior != NULL)
		drawBehavior->draw();
}

DrawBehavior* GameObject::getDrawBehavior() const
{
	return drawBehavior;
}

VertexBehavior* GameObject::getVertexBehavior() const
{
	return vertexBehavior;
}

IndexBehavior* GameObject::getIndexBehavior() const
{
	return indexBehavior;
}


void GameObject::setTranslation(D3DXVECTOR3 translation)
{
	this->position = translation;
	D3DXMatrixTranslation(&translationMatrix, translation.x, translation.y, translation.z);
}

void GameObject::setScale(D3DXVECTOR3 scale)
{
	this->scale = scale;
	D3DXMatrixScaling(&scalingMatrix, scale.x, scale.y, scale.z);
}

void GameObject::setRotation(D3DXVECTOR3 rotation)
{
	this->rotation = rotation;

	D3DXMATRIX rotateX, rotateY, rotateZ;

	D3DXMatrixRotationX(&rotateX, rotation.x);
	D3DXMatrixRotationY(&rotateY, rotation.y);
	D3DXMatrixRotationZ(&rotateZ, rotation.z);

	rotationMatrix = rotateX * rotateY * rotateZ;
}

void GameObject::setTextureScale(D3DXVECTOR3 textureScale)
{
	D3DXMatrixScaling(&textureScaleMatrix, textureScale.x, textureScale.y, textureScale.z);
}

D3DXMATRIX GameObject::getWorldMatrix()
{
	worldMatrix = rotationMatrix * scalingMatrix * translationMatrix;
	return worldMatrix;
}

D3DXMATRIX GameObject::getTranslationMatrix() const
{
	return translationMatrix;
}

D3DXMATRIX GameObject::getScalingMatrix() const
{
	return scalingMatrix; 
}

D3DXMATRIX GameObject::getRotationMatrix() const
{
	return rotationMatrix;
}

D3DXMATRIX GameObject::getTextureScaleMatrix() const
{
	return textureScaleMatrix;
}

D3DXVECTOR3 GameObject::getPosition()
{
	return position;
}

D3DXVECTOR3 GameObject::getScale()
{
	return scale;
}

D3DXVECTOR3 GameObject::getRotation()
{
	return rotation;
}