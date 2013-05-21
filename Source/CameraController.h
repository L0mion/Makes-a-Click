#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera;
class HeightMap;
class PivotPoint;
class XInputFetcher;

class CameraController
{
public:
	enum VantagePoints
	{
		VantagePoints_FIRST,

		VantagePoints_NEAR = VantagePoints_FIRST,
		VantagePoints_MEDIUM,
		VantagePoints_FAR,
		VantagePoints_FARFAR,
		VantagePoints_ONTOP,

		VantagePoints_LAST = VantagePoints_ONTOP,
		VantagePoints_CNT
	};

	CameraController( Camera* p_camera, XInputFetcher* p_xinput );
	~CameraController();

	DirectX::XMFLOAT3 getPosition() const;
	//DirectX::XMFLOAT3 getPivotPosition() const;
	void update( float p_dt, const XMFLOAT3& p_pivotPos );

	const XMFLOAT3& getForward() const;
	const XMFLOAT3& getRight() const;

private:
	void handleZoom( float p_dt );
	void zoomIn();
	void zoomOut();
	int vantagePointFromVantage( float p_vantage );
	int inBoundVantagePoint( int p_vantagePoint );
	float inBoundVantage( float p_vantage );
	void vibrate();

	float getThumbRX( float p_dt );
	float getThumbRY( float p_dt );

	void updateAngles( float p_x, float p_y );
	void updateVecsAndMats();
	void updateCam( const XMFLOAT3& p_pivotPos );

private:
	Camera* m_camera;
	XInputFetcher* m_xinput;

	// Zoom control
	static const int s_vantagePoints[VantagePoints_CNT];
	int m_curVantagePoint;
	float m_curVantage;
	float m_zoomPressedTime;
	float m_vibbedTime;

	// Overall
	XMFLOAT3 m_position;
	XMFLOAT3 m_forward;
	XMFLOAT3 m_right;

	XMFLOAT4X4 m_upRot;
	XMFLOAT4X4 m_rightRot;

	// Camera
	XMFLOAT3 m_look;
	
	// Utility
	XMFLOAT3 m_up;
	XMFLOAT3 m_north;
	XMFLOAT3 m_east;

	double m_sensitivity/*[Sticks_CNT]*/;

	float m_pivotAngleX;
	float m_pivotAngleY;
};