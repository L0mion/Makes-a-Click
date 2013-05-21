#include "DigitalSmoothControl.h"
#include "XInputFetcher.h"

DigitalSmoothControl::DigitalSmoothControl(
	XInputFetcher* p_xinput,
	InputHelper::Xbox360Digitals p_raise,
	InputHelper::Xbox360Digitals p_lower,
	vector<int> p_points,
	float p_smoothTime, float p_smoothSpeed, float p_vibrationDuration )
{
	m_xinput = p_xinput;
	m_next = p_raise;
	m_prev = p_lower;
	m_points = p_points;
	m_smoothTime = p_smoothTime;
	m_smoothSpeed = p_smoothSpeed;
	m_vibrationDuration = p_vibrationDuration;

	m_pressedTime = 0.0f;
	m_vibbedTime = 0.0f;
	m_curPoint = p_points[0];
	m_curSmooth = (float)p_points[0];
}

DigitalSmoothControl::~DigitalSmoothControl()
{

}

void DigitalSmoothControl::update( const float p_dt )
{
	if( m_vibbedTime > m_vibrationDuration ) {
		m_xinput->vibrate( 0, 0 );
	} else {
		m_vibbedTime += p_dt;
	}

	InputHelper::KeyStates ksNext = m_xinput->getBtnState( m_next);
	InputHelper::KeyStates ksPrev = m_xinput->getBtnState( m_prev );

	if( ksNext == InputHelper::KeyStates_KEY_PRESSED ) {
		m_pressedTime = 0.0f;
	} else if( ksNext == InputHelper::KeyStates_KEY_DOWN ) {
		m_pressedTime += p_dt;
		if( m_pressedTime > m_smoothTime ) {
			m_curSmooth += p_dt * m_smoothSpeed;
		}
	} else if( ksNext == InputHelper::KeyStates_KEY_RELEASED ) {
		if( m_pressedTime < m_smoothTime ) {
			nextPoint();
		}
	} 

	if( ksPrev == InputHelper::KeyStates_KEY_PRESSED ) {
		m_pressedTime = 0.0f;
	} else if( ksPrev == InputHelper::KeyStates_KEY_DOWN ) {
		m_pressedTime += p_dt;
		if( m_pressedTime > m_smoothTime ) {
			m_curSmooth -= p_dt * m_smoothSpeed;
		}
	} else if( ksPrev == InputHelper::KeyStates_KEY_RELEASED ) {
		if( m_pressedTime < m_smoothTime ) {
			prevPoint();
		}
	}

	// Make sure not out of bounds
	m_curPoint = pointFromSmooth( m_curSmooth );
	m_curSmooth =  inBoundSmooth( m_curSmooth );
}

int DigitalSmoothControl::getCurrentPoint() const { return m_curPoint; }
void DigitalSmoothControl::setCurrentPoint( const int p_point )
{
	m_curPoint = p_point;
	m_curSmooth = (float)m_points[p_point];
}

float DigitalSmoothControl::getCurrentSmooth() const { return m_curSmooth; }
void DigitalSmoothControl::setCurrentSmooth( const float p_smooth )
{
	m_curSmooth = p_smooth;
	m_curPoint = pointFromSmooth( p_smooth );
}

void DigitalSmoothControl::nextPoint()
{
	m_curPoint++;
	m_curPoint = inBoundPoint( m_curPoint );
	m_curSmooth = (float)m_points[m_curPoint];
}

void DigitalSmoothControl::prevPoint()
{
	m_curPoint--;
	m_curPoint = inBoundPoint( m_curPoint );
	m_curSmooth = (float)m_points[m_curPoint];
}

int DigitalSmoothControl::pointFromSmooth( float p_smooth )
{
	int vantage = (int)p_smooth;
	for( unsigned int i=m_points.size()-1; i>0; i-- ) {
		if( vantage >= m_points[i] ) {
			return i;
		}
	}
	return 0;
}

int DigitalSmoothControl::inBoundPoint( int p_point )
{
	if( p_point < 0 ) {
		p_point = 0;
		vibrate();
	} else if( p_point >= (int)m_points.size() ) {
		p_point = (int)m_points.size()-1;
		vibrate();
	}
	return p_point;
}

float DigitalSmoothControl::inBoundSmooth( float p_smooth )
{
	if( p_smooth < m_points[0] ) {
		p_smooth = (float)m_points[0];
		vibrate();
	} else if( p_smooth > m_points[m_points.size()-1] ) {
		p_smooth = (float)m_points[m_points.size()-1];
		vibrate();
	}
	return p_smooth;
}

void DigitalSmoothControl::vibrate()
{
	unsigned short motorSpeed = USHRT_MAX;
	m_vibbedTime = 0.0f;
	m_xinput->vibrate( motorSpeed, motorSpeed );
}
