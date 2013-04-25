#ifndef CAMERA_H
#define CAMERA_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include <math.h>

#include "Utility.h"

class Camera
{
private:
	D3DXVECTOR3 position;
	D3DXVECTOR3 right;
	D3DXVECTOR3 up;
	D3DXVECTOR3 look;

	D3DXMATRIX view;
	D3DXMATRIX projection;
public:
	Camera();
	~Camera();

	D3DXVECTOR3 getPosition() const;
	D3DXVECTOR3	getRight() const;
	D3DXVECTOR3 getLook() const;
	D3DXVECTOR3 getUp() const;
	D3DXMATRIX getView() const;
	D3DXMATRIX getProjection() const;

	void setLens(float fov, float aspect, float zn, float zf);

	void strafe(float velocity);
	void walk(float velocity);
	void verticalWalk(float velocity);
	void setHeight(float height);

	void pitch(float angle);
	void rotateY(float angle);
	
	void rebuildView();
};

#endif