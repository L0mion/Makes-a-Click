#pragma once

#include <DirectXMath.h>
using namespace DirectX;

class DigitalSmoothControl;
class HeightMap;
class XInputFetcher;
struct EntityBufferInfo;

class PivotPoint
{
public:
	PivotPoint( XInputFetcher* p_xinput, HeightMap* p_heightmap, EntityBufferInfo* p_avatar );
	virtual ~PivotPoint();

	void update( const float p_dt, const XMFLOAT3& p_forward,
		const XMFLOAT3& p_right );
	void setSize( const float p_size );
	float getSize() const;
	void setPosition( const DirectX::XMFLOAT3& p_pos );
	XMFLOAT3 getPosition() const;
	void setSpeed( const float p_speed );
	float getSpeed() const;

private:
	void movePivot( const XMFLOAT3& p_forward, const float p_x,
		const XMFLOAT3& p_right, const float p_y );
	float getThumbLX( const float p_dt );
	float getThumbLY( const float p_dt );

public:
	EntityBufferInfo* m_avatar;

private:
	XInputFetcher* m_xinput;
	HeightMap* m_heightmap;
	DigitalSmoothControl* m_sizeControl;

	float m_sensitivity;

	//XMFLOAT3 m_position;
	//float m_size;
	float m_speed;
};