#include "CameraController.h"

#include "camera.h"
#include "XInputFetcher.h"
#include "mathHelper.h"
#include "DebugGUI.h"
#include "HeightMap.h"
#include "PivotPoint.h"
#include <AntTweakBar.h>

const int CameraController::s_vantagePoints[VantagePoints_CNT] = {
	10,
	50,
	100,
	200,
	350
};

CameraController::CameraController( Camera* p_camera, XInputFetcher* p_xinput )
{
	m_camera = p_camera;
	m_xinput = p_xinput;

	/*m_sensitivity[Sticks_LEFT]  = 64.0f;
	m_sensitivity[Sticks_RIGHT] = 2.0f;*/
	m_sensitivity = 2.0f;

	m_curVantagePoint = VantagePoints_FAR;
	m_curVantage = (float)s_vantagePoints[m_curVantagePoint];

	//m_pivot	= DirectX::XMFLOAT3( 0.0f, 10.0f, -10.0f );
	m_position		= DirectX::XMFLOAT3( 0.0f, 10.0f, -10.0f );
	m_forward		= DirectX::XMFLOAT3( 0.0f, 0.0f, 1.0f );
	m_right			= DirectX::XMFLOAT3( 1.0f, 0.0f, 0.0f );
	
	m_look			= DirectX::XMFLOAT3( 0.0f, 0.0f, 1.0f );

	m_up			= DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f );
	m_north			= XMFLOAT3( 0.0f, 0.0f, 1.0f );
	m_east			= XMFLOAT3( 1.0f, 0.0f, 0.0f );

	m_pivotAngleX = 0.0f;
	m_pivotAngleY = 0.3f;

	DebugGUI* dGui = DebugGUI::getInstance();
	dGui->addVar( "CameraDebug", DebugGUI::Types_VEC3, DebugGUI::Permissions_READ_ONLY, "pivot point", &m_position.x );
	dGui->addVar( "CameraDebug", DebugGUI::Types_VEC3, DebugGUI::Permissions_READ_ONLY, "position", &m_position.x );
	dGui->addVar( "CameraDebug", DebugGUI::Types_FLOAT, DebugGUI::Permissions_READ_WRITE, "x angle", &m_pivotAngleX, "step=0.1" );
	dGui->addVar( "CameraDebug", DebugGUI::Types_FLOAT, DebugGUI::Permissions_READ_WRITE, "y angle", &m_pivotAngleY, "step=0.1" );
	dGui->addVar( "CameraDebug", DebugGUI::Types_VEC3, DebugGUI::Permissions_READ_ONLY, "look", &m_look.x, "opened=true" );
	dGui->addVar( "CameraDebug", DebugGUI::Types_VEC3, DebugGUI::Permissions_READ_ONLY, "right", &m_right.x, "opened=true" );
	dGui->addVar( "CameraDebug", DebugGUI::Types_VEC3, DebugGUI::Permissions_READ_ONLY, "forward", &m_forward.x, "opened=true" );
	dGui->addVar( "CameraDebug", DebugGUI::Types_VEC3, DebugGUI::Permissions_READ_ONLY, "up", &m_up.x );
	dGui->addVar( "CameraDebug", DebugGUI::Types_VEC3, DebugGUI::Permissions_READ_ONLY, "north", &m_north.x );
	dGui->addVar( "CameraDebug", DebugGUI::Types_VEC3, DebugGUI::Permissions_READ_ONLY, "east", &m_east.x );

	dGui->setPosition( "CameraDebug", 420, 0 );
	dGui->setSize( "CameraDebug", 300, 480 );

}

CameraController::~CameraController()
{

}

DirectX::XMFLOAT3 CameraController::getPosition() const
{
	return m_position;
}

void CameraController::update( float p_dt, const XMFLOAT3& p_pivotPos )
{
	handleZoom( p_dt );

	float thumbRX = getThumbRX( p_dt );
	float thumbRY = getThumbRY( p_dt );

	if( m_curVantagePoint != VantagePoints_ONTOP ) {
		updateAngles( thumbRX, thumbRY );
		updateVecsAndMats();
		updateCam( p_pivotPos );
	} else {
		m_position.x = 0.0f;
		m_position.y = (float)s_vantagePoints[VantagePoints_ONTOP];
		m_position.z = 0.0f;

		m_look.x = 0.0f;
		m_look.y = -1.0f;
		m_look.z = 0.0f;

		m_right.x = 1.0f;
		m_right.y = 0.0f;
		m_right.z = 0.0f;
	}

	m_camera->rebuildView( m_position, m_right, m_look, m_up );
}

const XMFLOAT3& CameraController::getForward() const { return m_forward; }
const XMFLOAT3& CameraController::getRight() const { return m_right; }

void CameraController::handleZoom( float p_dt )
{
	float smoothZoomTime = 0.2f;
	float zoomFac = 100.0f;
	float vibTime = 0.2f;
	
	if( m_vibbedTime > vibTime ) {
		m_xinput->vibrate( 0, 0 );
	} else {
		m_vibbedTime += p_dt;
	}

	InputHelper::KeyStates dUp = m_xinput->getBtnState(
		InputHelper::Xbox360Digitals_DPAD_UP );
	InputHelper::KeyStates dDown = m_xinput->getBtnState(
		InputHelper::Xbox360Digitals_DPAD_DOWN );

	if( dUp == InputHelper::KeyStates_KEY_PRESSED ) {
		m_zoomPressedTime = 0.0f;
	} else if( dUp == InputHelper::KeyStates_KEY_DOWN ) {
		m_zoomPressedTime += p_dt;
		if( m_zoomPressedTime > smoothZoomTime ) {
			m_curVantage -= p_dt * zoomFac;
		}
	} else if( dUp == InputHelper::KeyStates_KEY_RELEASED ) {
		if( m_zoomPressedTime < smoothZoomTime ) {
			zoomIn();
		}
	} 
	
	if( dDown == InputHelper::KeyStates_KEY_PRESSED ) {
		m_zoomPressedTime = 0.0f;
	} else if( dDown == InputHelper::KeyStates_KEY_DOWN ) {
		m_zoomPressedTime += p_dt;
		if( m_zoomPressedTime > smoothZoomTime ) {
			m_curVantage += p_dt * zoomFac;
		}
	} else if( dDown == InputHelper::KeyStates_KEY_RELEASED ) {
		if( m_zoomPressedTime < smoothZoomTime ) {
			zoomOut();
		}
	}

	// Zoom must be in correct range
	m_curVantagePoint = vantagePointFromVantage( m_curVantage );
	m_curVantage =  inBoundVantage( m_curVantage );
}

void CameraController::zoomIn()
{
	m_curVantagePoint--;
	m_curVantagePoint = inBoundVantagePoint( m_curVantagePoint );
	m_curVantage = (float)s_vantagePoints[m_curVantagePoint];
}

void CameraController::zoomOut()
{
	m_curVantagePoint++;
	m_curVantagePoint = inBoundVantagePoint( m_curVantagePoint );
	m_curVantage = (float)s_vantagePoints[m_curVantagePoint];
}

int CameraController::vantagePointFromVantage( float p_vantage )
{
	int vantage = (int)p_vantage;
	for( int i=VantagePoints_LAST; i>0; i-- ) {
		if( vantage >= s_vantagePoints[i] ) {
			return i;
		}
	}
	return VantagePoints_FIRST;
}

int CameraController::inBoundVantagePoint( int p_vantagePoint )
{
	if( p_vantagePoint < VantagePoints_FIRST ) {
		p_vantagePoint = VantagePoints_FIRST;
		vibrate();
	} else if( p_vantagePoint > VantagePoints_LAST ) {
		p_vantagePoint = VantagePoints_LAST;
		vibrate();
	}
	return p_vantagePoint;
}

float CameraController::inBoundVantage( float p_vantage )
{
	if( p_vantage < s_vantagePoints[VantagePoints_FIRST] ) {
		p_vantage = (float)s_vantagePoints[VantagePoints_FIRST];
		vibrate();
	} else if( p_vantage > s_vantagePoints[VantagePoints_LAST] ) {
		p_vantage = (float)s_vantagePoints[VantagePoints_LAST];
		vibrate();
	}
	return p_vantage;
}

void CameraController::vibrate()
{
	unsigned short motorSpeed = USHRT_MAX;
	m_vibbedTime = 0.0f;
	m_xinput->vibrate( motorSpeed, motorSpeed );
}

float CameraController::getThumbRX( float p_dt )
{
	double thumbRX	= m_xinput->getCalibratedAnalogQuad(
		InputHelper::Xbox360Analogs_THUMB_RX_NEGATIVE );
	thumbRX *= m_sensitivity * p_dt;
	return (float)thumbRX;

}

float CameraController::getThumbRY( float p_dt )
{
	double thumbRY	= m_xinput->getCalibratedAnalogQuad(
		InputHelper::Xbox360Analogs_THUMB_RY_NEGATIVE );
	thumbRY *= m_sensitivity * p_dt  *-1;
	return (float)thumbRY;
}

void CameraController::updateAngles( float p_x, float p_y )
{
	m_pivotAngleX += p_x;
	m_pivotAngleY += p_y;

	float maxPivotAngleYFactor = 0.5;

	if( m_pivotAngleY > XM_PI*maxPivotAngleYFactor ) {
		m_pivotAngleY = XM_PI*maxPivotAngleYFactor;
	} else if( m_pivotAngleY < 0.0f ) {
		m_pivotAngleY = 0.0f;
	}
}

void CameraController::updateVecsAndMats()
{
	// Set up rotation in xz-plane (turning).
	XMFLOAT3 down( 0, -1, 0 );
	XMVECTOR upRotVec = XMLoadFloat3( &m_up );
	XMMATRIX upRotMat = XMMatrixRotationAxis( upRotVec, m_pivotAngleX );
	XMStoreFloat4x4( &m_upRot, upRotMat );

	XMVECTOR northVec = XMLoadFloat3( &m_north );
	XMVECTOR forwardVec = XMVector3Transform( northVec, upRotMat );
	XMStoreFloat3( &m_forward, forwardVec );

	XMVECTOR eastVec = XMLoadFloat3( &m_east );
	XMVECTOR rightVec = XMVector3Transform( eastVec, upRotMat );
	XMStoreFloat3( &m_right, rightVec );

	XMMATRIX rightRotMat = XMMatrixRotationAxis( rightVec, m_pivotAngleY );
	XMStoreFloat4x4( &m_rightRot, rightRotMat );
}

void CameraController::updateCam( const XMFLOAT3& p_pivotPos )
{
	// Set up rotation in xz-plane (turning).
	XMMATRIX upRotMat = XMLoadFloat4x4( &m_upRot );

	// Load right for z,y-plane rotation (pitch)
	XMMATRIX rightRotMat = XMLoadFloat4x4( &m_rightRot );

	// Rotate 
	//float distToVantage = s_vantagePoints[m_currentVantagePoint];

	m_look.x = m_forward.x;
	m_look.y = m_forward.y;
	m_look.z = m_forward.z;

	XMVECTOR lookVec = XMLoadFloat3( &m_look );
	lookVec = XMVector3Transform( lookVec, rightRotMat );
	XMStoreFloat3( &m_look, lookVec );

	XMFLOAT3 finalPos = m_look;
	finalPos.x *= m_curVantage;
	finalPos.y *= m_curVantage;
	finalPos.z *= m_curVantage;

	m_position.x = p_pivotPos.x - finalPos.x;
	m_position.y = - finalPos.y;
	m_position.z = p_pivotPos.z - finalPos.z;
	
	// Last zoom level has a static height
	if( m_curVantagePoint < VantagePoints_FAR ) {
		m_position.y += p_pivotPos.y;
	}
}