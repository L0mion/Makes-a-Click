#pragma once
#include <DirectXMath.h>

using namespace DirectX;

class Camera;
class HeightMap;
class XInputFetcher;

class CameraController
{
public:
	enum Sticks
	{
		Sticks_LEFT,
		Sticks_RIGHT,
		Sticks_CNT
	};

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

	CameraController( Camera* p_camera, XInputFetcher* p_xinput,
		HeightMap* p_heightmap );
	~CameraController();

	void update( float p_dt );
	DirectX::XMFLOAT3 getPosition() const;
	DirectX::XMFLOAT3 getPivotPosition() const;

private:
	void movePivotStatically( float p_x, float p_y );
	void movePivotByRightAndLook( float p_x, float p_y );
	void updateAngles( float p_x, float p_y );
	void moveCam();
	void updateLookAndRight();

	void zoomIn();
	void zoomOut();

	void strafeCam( float p_distance );
	void walk( float p_distance );
	void walkDeluxe( float p_distance );
	void verticalWalk( float p_distance );

	void pitchCam(float angle);
	void yawCam(float angle);

private:
	Camera* m_camera;
	XInputFetcher* m_xinput;
	HeightMap* m_heightmap;

	DirectX::XMFLOAT3 m_position;
	DirectX::XMFLOAT3 m_right;
	DirectX::XMFLOAT3 m_look;
	DirectX::XMFLOAT3 m_up;
	
	double m_sensitivity[Sticks_CNT];
	
	DirectX::XMFLOAT3 m_pivotPoint;

	static const int s_vantagePoints[VantagePoints_CNT];
	int m_currentVantagePoint;
	//float m_pivotDist;

	float m_pivotAngleX;
	float m_pivotAngleY;
};