#include "Terrain.h"

Terrain::Terrain(	ID3D11Device *device,
					ID3D11DeviceContext *devcon,
					LPCTSTR textureFilename,
					DrawBehavior *drawBehavior,
					VertexBehavior *vertexBehavior,
					IndexBehavior *indexBehavior) :	GameObject(	device,
																devcon,
																textureFilename,
																drawBehavior,
																vertexBehavior,
																indexBehavior)
{

}


float Terrain::getHeight(float x, float z)
{
	VertexBehaviorHeightmap *temp = (VertexBehaviorHeightmap*)getVertexBehavior();
	return temp->getHeight(x, z);
}