#pragma once
#include "InputHelper.h"
#include <vector>

class XInputFetcher;

using namespace std;

class DigitalSmoothControl
{
public:
	DigitalSmoothControl(
		XInputFetcher* p_xinput,
		InputHelper::Xbox360Digitals p_raise,
		InputHelper::Xbox360Digitals p_lower,
		vector<int> p_points,
		float p_smoothTime, float p_smoothSpeed, float p_vibrationDuration );
	~DigitalSmoothControl();

	void update( const float p_dt );

	int getCurrentPoint() const;
	void setCurrentPoint( const int p_point );
	float getCurrentSmooth() const;
	void setCurrentSmooth( const float p_smooth );

private:
	void nextPoint();
	void prevPoint();
	int pointFromSmooth( float p_smooth );
	int inBoundPoint( int p_point );
	float inBoundSmooth( float p_smooth );
	void vibrate();

private:
	XInputFetcher* m_xinput;
	InputHelper::Xbox360Digitals m_next;
	InputHelper::Xbox360Digitals m_prev;
	vector<int> m_points;
	float m_smoothTime;
	float m_smoothSpeed;
	float m_vibrationDuration;

	float m_pressedTime;
	float m_vibbedTime;
	int m_curPoint;
	float m_curSmooth;

};