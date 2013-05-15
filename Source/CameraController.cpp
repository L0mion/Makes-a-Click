#include "CameraController.h"

#include "camera.h"
#include "XInputFetcher.h"
#include "mathHelper.h"
#include "DebugGUI.h"
#include "HeightMap.h"

const int CameraController::s_vantagePoints[VantagePoints_CNT] = {
	5,
	10,
	20,
	100
};

CameraController::CameraController( Camera* p_camera,
	XInputFetcher* p_xinput, HeightMap* p_heightmap )
{
	m_camera = p_camera;
	m_xinput = p_xinput;
	m_heightmap = p_heightmap;

	m_sensitivity[Sticks_LEFT]  = 64.0f;
	m_sensitivity[Sticks_RIGHT] = 2.0f;
	m_currentVantagePoint = VantagePoints_MEDIUM;

	m_pivotPoint	= DirectX::XMFLOAT3( 0.0f, 10.0f, -10.0f );
	m_position		= DirectX::XMFLOAT3( 0.0f, 10.0f, -10.0f );
	m_right			= DirectX::XMFLOAT3( 1.0f, 0.0f, 0.0f );
	m_look			= DirectX::XMFLOAT3( 0.0f, 0.0f, 1.0f );
	m_up			= DirectX::XMFLOAT3( 0.0f, 1.0f, 0.0f );

	m_pivotAngleX = 0.01f;
	m_pivotAngleY = 0.01f;

	DebugGUI* dGui = DebugGUI::getInstance();

	dGui->addVar( "Camera debug", DebugGUI::Types_FLOAT, DebugGUI::Permissions_READ_WRITE, "x angle", &m_pivotAngleX );
	dGui->addVar( "Camera debug", DebugGUI::Types_FLOAT, DebugGUI::Permissions_READ_WRITE, "y angle", &m_pivotAngleY );
	dGui->addVar( "Camera debug", DebugGUI::Types_VEC3, DebugGUI::Permissions_READ_ONLY, "look", &m_look.x );
	dGui->addVar( "Camera debug", DebugGUI::Types_VEC3, DebugGUI::Permissions_READ_ONLY, "right", &m_right.x );
	dGui->addVar( "Camera debug", DebugGUI::Types_VEC3, DebugGUI::Permissions_READ_ONLY, "up", &m_up.x );
}

CameraController::~CameraController()
{

}

void CameraController::update( float p_dt )
{
	double thumbLY	= m_xinput->getCalibratedAnalogQuad(
		InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE );
	double thumbLX	= m_xinput->getCalibratedAnalogQuad(
		InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE );
	double thumbRX	= m_xinput->getCalibratedAnalogQuad(
		InputHelper::Xbox360Analogs_THUMB_RX_NEGATIVE );
	double thumbRY	= m_xinput->getCalibratedAnalogQuad(
		InputHelper::Xbox360Analogs_THUMB_RY_NEGATIVE );

	InputHelper::KeyStates dUp = m_xinput->getBtnState(
		InputHelper::Xbox360Digitals_DPAD_UP );
	InputHelper::KeyStates dDown = m_xinput->getBtnState(
		InputHelper::Xbox360Digitals_DPAD_DOWN );

	if( dUp == InputHelper::KeyStates_KEY_PRESSED ) {
		zoomIn();
	} else if( dDown == InputHelper::KeyStates_KEY_PRESSED ) {
		zoomOut();
	}

	thumbLX *= m_sensitivity[Sticks_LEFT]  * p_dt;
	thumbLY *= m_sensitivity[Sticks_LEFT]  * p_dt;
	thumbRX *= m_sensitivity[Sticks_RIGHT] * p_dt;
	thumbRY *= m_sensitivity[Sticks_RIGHT] * p_dt * -1;

	movePivot( thumbLX, thumbLY );
	updateAngles( thumbRX, thumbRY );
	
	moveCam();
	updateLookAndRight();

	m_camera->rebuildView( m_position, m_right, m_look, m_up );
}

DirectX::XMFLOAT3 CameraController::getPosition() const
{
	return m_position;
}
DirectX::XMFLOAT3 CameraController::getPivotPosition() const
{
	return m_pivotPoint;
}

void CameraController::movePivot( float p_x, float p_y )
{
	m_pivotPoint.x += m_look.x * p_y;
	m_pivotPoint.z += m_look.z * p_y;

	m_pivotPoint.x += m_right.x * p_x;
	m_pivotPoint.z += m_right.z * p_x;

	m_pivotPoint.y = m_heightmap->getHeight( m_pivotPoint.x, m_pivotPoint.z );
}

void CameraController::updateAngles( float p_x, float p_y )
{
	m_pivotAngleX += p_x;
	m_pivotAngleY += p_y;

	if( m_pivotAngleY > XM_PI*0.3f ) {
		m_pivotAngleY = XM_PI*0.3f;
	} else if( m_pivotAngleY < 0.0f ) {
		m_pivotAngleY = 0.0f;
	}
}

void CameraController::moveCam()
{
	// Set up rotation matrix for x,z-plane rotation (turning)
	XMFLOAT3 yRot = XMFLOAT3( 0.0f, 1.0f, 0.0f );
	XMVECTOR yRotVec = XMLoadFloat3( &yRot );
	XMMATRIX yRotMat = XMMatrixRotationAxis( yRotVec, m_pivotAngleX );

	// Load right for z,y-plane rotation (pitch)
	XMVECTOR rightRotVec = XMLoadFloat3( &m_right );
	XMMATRIX rightRotMat = XMMatrixRotationAxis( rightRotVec, m_pivotAngleY );

	// Rotate 
	float distToVantage = s_vantagePoints[m_currentVantagePoint];
	XMFLOAT3 finalPos = XMFLOAT3( distToVantage , 0.0f, 0.0f );
	XMVECTOR finalPosVec = XMLoadFloat3( &finalPos );
	finalPosVec = XMVector3Transform( finalPosVec, yRotMat );
	finalPosVec = XMVector3Transform( finalPosVec, rightRotMat );

	XMStoreFloat3( &finalPos, finalPosVec );
	m_position.x = m_pivotPoint.x + finalPos.x;
	m_position.y = finalPos.y;
	m_position.z = m_pivotPoint.z + finalPos.z;
	
	// Last zoom level has a static height
	if( m_currentVantagePoint != VantagePoints_FARFAR ) {
		m_position.y += m_pivotPoint.y;
	}
}


void CameraController::updateLookAndRight()
{
	m_look.x = m_pivotPoint.x - m_position.x;
	m_look.y = -m_position.y;
	m_look.z = m_pivotPoint.z - m_position.z;

	// Last zoom level has a static height
	if( m_currentVantagePoint != VantagePoints_FARFAR ) {
		m_look.y += m_pivotPoint.y;
	}

	XMVECTOR lookVec = XMLoadFloat3( &m_look );
	lookVec = XMVector3Normalize( lookVec );

	XMVECTOR upVec = XMLoadFloat3( &m_up );
	XMVECTOR rightVec;
	rightVec = XMVector3Cross( upVec, lookVec );
	rightVec = XMVector3Normalize( rightVec );

	XMStoreFloat3( &m_look, lookVec );
	XMStoreFloat3( &m_right, rightVec );

}

void CameraController::zoomIn()
{
	m_currentVantagePoint--;
	if( m_currentVantagePoint < VantagePoints_FIRST ) {
		m_currentVantagePoint = VantagePoints_FIRST;
	}
}

void CameraController::zoomOut()
{
	m_currentVantagePoint++;
	if( m_currentVantagePoint > VantagePoints_LAST ) {
		m_currentVantagePoint = VantagePoints_LAST;
	}
}

void CameraController::strafeCam( float p_distance )
{
	m_position.x += m_right.x * p_distance;
	m_position.y += m_right.y * p_distance;
	m_position.z += m_right.z * p_distance;
}
void CameraController::walk( float p_distance )
{
	m_position.x += m_look.x * p_distance;
	m_position.y += m_look.y * p_distance;
	m_position.z += m_look.z * p_distance;
}
void CameraController::walkDeluxe( float p_distance )
{
	m_position.x += m_look.x * p_distance;
	m_position.z += m_look.z * p_distance;
}
void CameraController::verticalWalk( float p_distance )
{
	m_position.y += p_distance;
}

void CameraController::pitchCam( float p_angle )
{
	DirectX::XMMATRIX rotationMatrix;
	DirectX::XMVECTOR xmRight	= DirectX::XMLoadFloat3(&m_right);
	DirectX::XMVECTOR xmLook	= DirectX::XMLoadFloat3(&m_look);
	DirectX::XMVECTOR xmUp		= DirectX::XMLoadFloat3(&m_up);
	rotationMatrix = DirectX::XMMatrixRotationAxis(xmRight, p_angle);

	xmLook	= DirectX::XMVector3TransformNormal(xmLook, rotationMatrix);
	xmUp	= DirectX::XMVector3TransformNormal(xmUp, rotationMatrix);

	DirectX::XMStoreFloat3(&m_look, xmLook);
	DirectX::XMStoreFloat3(&m_up, xmUp);
}

void CameraController::yawCam( float p_angle )
{
	DirectX::XMVECTOR yAxis = DirectX::XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
	DirectX::XMMATRIX rotationMatrix;

	DirectX::XMVECTOR xmRight	= DirectX::XMLoadFloat3(&m_right);
	DirectX::XMVECTOR xmLook	= DirectX::XMLoadFloat3(&m_look);
	DirectX::XMVECTOR xmUp		= DirectX::XMLoadFloat3(&m_up);

	rotationMatrix = DirectX::XMMatrixRotationAxis(yAxis, p_angle);
	xmRight	= DirectX::XMVector3TransformNormal(xmRight, rotationMatrix);
	xmLook	= DirectX::XMVector3TransformNormal(xmLook, rotationMatrix);
	xmUp	= DirectX::XMVector3TransformNormal(xmUp, rotationMatrix);

	DirectX::XMStoreFloat3(&m_right, xmRight);
	DirectX::XMStoreFloat3(&m_look, xmLook);
	DirectX::XMStoreFloat3(&m_up, xmUp);
}