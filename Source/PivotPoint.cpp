#include "PivotPoint.h"
#include "EntityBufferInfo.h"
#include "HeightMap.h"
#include "XInputFetcher.h"

PivotPoint::PivotPoint( XInputFetcher* p_xinput, HeightMap* p_heightmap,
					   EntityBufferInfo* p_avatar )
{
	m_xinput = p_xinput;
	m_heightmap = p_heightmap;
	m_avatar = p_avatar;

	m_sensitivity = 64.0f;

	m_position = XMFLOAT3( .0f, .0f, .0f );
	m_size = 1.0f;
	m_speed = 0.0f;
}

PivotPoint::~PivotPoint()
{

}

void PivotPoint::update( const float p_dt, const XMFLOAT3& p_forward,
						const XMFLOAT3& p_right )
{
	float thumbLX = getThumbLX( p_dt );
	float thumbLY = getThumbLY( p_dt );

	float totSpeed = 0.0f;
	totSpeed += (float)m_xinput->getCalibratedAnalogQuad(
		InputHelper::Xbox360Analogs_TRIGGER_R ); 
	totSpeed -= (float)m_xinput->getCalibratedAnalogQuad(
		InputHelper::Xbox360Analogs_TRIGGER_L );

	setSpeed( totSpeed );

	movePivot( p_forward, thumbLX, p_right, thumbLY );
}

void PivotPoint::setSize( const float p_size )
{
	m_size = p_size;
	m_avatar->m_world._44 = 1.0f/p_size;
}
float PivotPoint::getSize() const { return m_size; }

void PivotPoint::setPosition( const DirectX::XMFLOAT3& p_pos )
{
	m_position = p_pos;
	m_avatar->m_world._41 = m_position.x;
	m_avatar->m_world._42 = m_position.y;
	m_avatar->m_world._43 = m_position.z;
}
DirectX::XMFLOAT3 PivotPoint::getPosition() const { return m_position; }

void PivotPoint::setSpeed( const float p_speed ) { m_speed = p_speed; }
float PivotPoint::getSpeed() const { return m_speed; }

void PivotPoint::movePivot( const XMFLOAT3& p_forward, const float p_x,
						   const XMFLOAT3& p_right, const float p_y )
{
	m_position.x += p_forward.x * p_y;
	m_position.z += p_forward.z * p_y;

	m_position.x += p_right.x * p_x;
	m_position.z += p_right.z * p_x;

	m_position.y = m_heightmap->getHeight( m_position.x, m_position.z );
	setPosition( m_position );
}

float PivotPoint::getThumbLX( const float p_dt )
{
	double thumbLX	= m_xinput->getCalibratedAnalogQuad(
		InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE );
	thumbLX *= m_sensitivity * p_dt;
	return (float)thumbLX;
}

float PivotPoint::getThumbLY( const float p_dt )
{
	double thumbLY	= m_xinput->getCalibratedAnalogQuad(
		InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE );
	thumbLY *= m_sensitivity * p_dt;
	return (float)thumbLY;
}