#include "XInputFetcher.h"
#include <math.h>
#include "DebugGUI.h"
#include "mathHelper.h"

int XInputFetcher::s_btnMaskMap[InputHelper::Xbox360Digitals_CNT] = 
{
	XINPUT_GAMEPAD_DPAD_UP,
	XINPUT_GAMEPAD_DPAD_DOWN,
	XINPUT_GAMEPAD_DPAD_LEFT, 
	XINPUT_GAMEPAD_DPAD_RIGHT,
	XINPUT_GAMEPAD_START,
	XINPUT_GAMEPAD_BACK,
	XINPUT_GAMEPAD_LEFT_THUMB,
	XINPUT_GAMEPAD_RIGHT_THUMB,
	XINPUT_GAMEPAD_LEFT_SHOULDER,
	XINPUT_GAMEPAD_RIGHT_SHOULDER,
	XINPUT_GAMEPAD_A,
	XINPUT_GAMEPAD_B,
	XINPUT_GAMEPAD_X,
	XINPUT_GAMEPAD_Y
};

string XInputFetcher::s_btnNames[InputHelper::Xbox360Digitals_CNT] = 
{
	"DPAD_UP",
	"DPAD_DOWN",
	"DPAD_LEFT", 
	"DPAD_RIGHT",
	"START",
	"BACK",
	"LEFT_THUMB",
	"RIGHT_THUMB",
	"LEFT_SHOULDER",
	"RIGHT_SHOULDER",
	"A",
	"B",
	"X",
	"Y"
};

string XInputFetcher::s_analogNames[InputHelper::Xbox360Analogs_CNT] = 
{
	"THUMB_LX_POSITIVE",
	"THUMB_LX_NEGATIVE",
	"THUMB_LY_POSITIVE",
	"THUMB_LY_NEGATIVE",
	"THUMB_RX_POSITIVE",
	"THUMB_RX_NEGATIVE",
	"THUMB_RY_POSITIVE",
	"THUMB_RY_NEGATIVE",
	"TRIGGER_L",
	"TRIGGER_R"
};

XInputFetcher::XInputFetcher()
{
	m_epsilon = 0.0;
	m_sensitivity = 1.0;

	clearBuffers();

	// Add input to DebugGUI
	DebugGUI* dbgGui = DebugGUI::getInstance();

	for( int btnIdx=0; btnIdx<InputHelper::Xbox360Digitals_CNT; btnIdx++ )
	{
		dbgGui->addVar( "Controller Buttons", DebugGUI::Types_INT,
			DebugGUI::Permissions_READ_ONLY, s_btnNames[btnIdx],
			&m_btns[btnIdx], "" );
	}
	for( int axisIdx=0; axisIdx<InputHelper::Xbox360Analogs_CNT; axisIdx++ )
	{
		dbgGui->addVar( "Controller Axes", DebugGUI::Types_INT,
			DebugGUI::Permissions_READ_ONLY, s_analogNames[axisIdx],
			&m_rawAnalogs[axisIdx], "" );
	}
}

XInputFetcher::~XInputFetcher()
{
	vibrate(0.0f, 0.0f);
}

void XInputFetcher::update()
{
	//XINPUT_STATE newState;
	ZeroMemory( &m_currentState, sizeof(XINPUT_STATE) );
	// clearBuffers();
	unsigned long result = XInputGetState( 0, &m_currentState );
	if( result == ERROR_SUCCESS )
	{
		for( int i=0; i<InputHelper::Xbox360Digitals_CNT; i++)
		{
			bool pressed = m_currentState.Gamepad.wButtons & s_btnMaskMap[i]?true:false;
			m_btns[i] = InputHelper::calcState( m_btns[i], pressed );
		}
		//m_currentState = newState;

		m_rawAnalogs[InputHelper::Xbox360Analogs_THUMB_LX_POSITIVE] = m_currentState.Gamepad.sThumbLX;
		m_rawAnalogs[InputHelper::Xbox360Analogs_THUMB_LY_POSITIVE] = m_currentState.Gamepad.sThumbLY;
		m_rawAnalogs[InputHelper::Xbox360Analogs_THUMB_RX_POSITIVE] = m_currentState.Gamepad.sThumbRX;
		m_rawAnalogs[InputHelper::Xbox360Analogs_THUMB_RY_POSITIVE] = m_currentState.Gamepad.sThumbRY;
		m_rawAnalogs[InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE] = m_currentState.Gamepad.sThumbLX;
		m_rawAnalogs[InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE] = m_currentState.Gamepad.sThumbLY;
		m_rawAnalogs[InputHelper::Xbox360Analogs_THUMB_RX_NEGATIVE] = m_currentState.Gamepad.sThumbRX;
		m_rawAnalogs[InputHelper::Xbox360Analogs_THUMB_RY_NEGATIVE] = m_currentState.Gamepad.sThumbRY;
		m_rawAnalogs[InputHelper::Xbox360Analogs_TRIGGER_L] = (short)m_currentState.Gamepad.bLeftTrigger;
		m_rawAnalogs[InputHelper::Xbox360Analogs_TRIGGER_R] = (short)m_currentState.Gamepad.bRightTrigger;
	}
	else
	{
		clearBuffers();
	}
}

InputHelper::KeyStates XInputFetcher::getBtnState( int p_btn )
{
	if( 0 <= p_btn && p_btn < InputHelper::Xbox360Digitals_CNT )
		return m_btns[p_btn];
	else
		return InputHelper::KeyStates_NON_EXISTING;
}

short XInputFetcher::getRawAnalog( int p_analog )
{
	if( 0 <= p_analog && p_analog < InputHelper::Xbox360Analogs_CNT )
		return m_rawAnalogs[p_analog];
	else
		return -1;
}

void XInputFetcher::setControllerSensitivity( const double p_sensitivity )
{
	m_sensitivity = p_sensitivity;
}

const double XInputFetcher::getControllerSensitivity() const
{
	return m_sensitivity;
}

double XInputFetcher::getCalibratedAnalogQuad( int p_analog )
{
	double val = getCalibratedAnalog( p_analog );
	return MathHelper::signedQuad( val );
}

double XInputFetcher::getCalibratedAnalog( int p_analog )
{
	// Std-val. Whats returned if the analog isn't found.
	double val = (float)-1;

	// Make sure that p_analog is in range
	if( 0 <= p_analog && p_analog < InputHelper::Xbox360Analogs_CNT )
	{
		// Offset the input by the offset calculated by calibrate()
		int calibratedInput = getRawAnalog( p_analog ) - m_analogOffsets[p_analog] ;
		short clampedInput = calibratedInput;

		if( calibratedInput > SHRT_MAX ) {
			clampedInput = SHRT_MAX;
		} else if( calibratedInput < SHRT_MIN ) {
			clampedInput = SHRT_MIN; 
		}

		// Triggers have a precision of 0-255 instead of 0-65k as the thumb sticks.
		// Take that into account when calculating 0.0 - 1.0 
		if(p_analog == InputHelper::Xbox360Analogs_TRIGGER_L ||
			p_analog == InputHelper::Xbox360Analogs_TRIGGER_R) {
			val =  (double)clampedInput / 255; //BYTE_MAX
		} else {
			val = (double)clampedInput / SHRT_MAX;
		}

		// Dead zone adjustment
		if( abs(val) < m_epsilon ) {
			val = 0.0;
		}
	}

	return val;
}

void XInputFetcher::calibrate( double p_epsilon )
{
	m_epsilon = p_epsilon;

	// Calibrate _bad_ (those that are reeeeeally bad) gamepads
	update();

	for( int i=0; i<InputHelper::Xbox360Analogs_CNT; i++)
	{
		m_analogOffsets[i] = getRawAnalog(i);
	}
}

void XInputFetcher::vibrate( unsigned short p_leftMotor, unsigned short p_rightMotor ) const
{
	XINPUT_VIBRATION vibration;
	vibration.wLeftMotorSpeed = (WORD)p_leftMotor;
	vibration.wRightMotorSpeed = (WORD)p_rightMotor;
	XInputSetState(0, &vibration);
}

void XInputFetcher::clearBuffers()
{
	ZeroMemory( &m_currentState, sizeof(XINPUT_STATE) );
	ZeroMemory( &m_rawAnalogs, sizeof(m_rawAnalogs) );
	ZeroMemory( &m_btns, sizeof(m_btns) );

}