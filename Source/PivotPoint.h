#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class XInputFetcher;

class PivotPoint
{
public:
	PivotPoint( XInputFetcher* p_xinput );
	~PivotPoint();

	//XMFLOAT3 getPosition();
	//float getSize();
	//float getSpeed();

	void update( float p_dt ); 

public:
	XInputFetcher* m_xinput;

	XMFLOAT3 m_position;
	float m_size;
	float m_speed;

private:

};