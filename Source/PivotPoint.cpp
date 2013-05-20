#include "PivotPoint.h"

PivotPoint::PivotPoint( XInputFetcher* p_xinput )
{
	m_xinput = p_xinput;

	m_position = XMFLOAT3( .0f, .0f, .0f );
	m_size = 0.0f;
	m_speed = 0.0f;
}

PivotPoint::~PivotPoint()
{

}

void PivotPoint::update( float p_dt )
{

}
