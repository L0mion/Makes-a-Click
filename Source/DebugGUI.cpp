#include "DebugGUI.h"

//=========================================================================
// Public functions
//=========================================================================
DebugGUI* DebugGUI::getInstance()
{
	// Instantiated on first use. Guaranteed to be destroyed.
	static DebugGUI instance;
	return &instance;
}

void DebugGUI::init( ManagementD3D* p_deviceHandler, HWND p_handle )
{
	m_deviceHandler = p_deviceHandler;
	TwInit(TW_DIRECT3D11, m_deviceHandler->getDevice());
	//TwWindowSize(m_deviceHandler->getWindowWidth(), m_deviceHandler->getWindowHeight());
	TwWindowSize( SCREEN_WIDTH, SCREEN_HEIGHT );
}

DebugGUI::Results DebugGUI::addVar( string p_barName, Types p_type,
	Permissions p_permissions, string p_name, void *p_var, string p_options )
{
	TwBar* bar = barFromString(p_barName);

	Results result = Results_FAILED;
	if(p_permissions == Permissions_READ_ONLY)
	{
		if(TwAddVarRO(bar, p_name.c_str(), (TwType)p_type, p_var, p_options.c_str()))
			result = Results_SUCCESS;
	}
	else if(p_permissions == Permissions_READ_WRITE)
	{
		if(TwAddVarRW(bar, p_name.c_str(), (TwType)p_type, p_var, p_options.c_str()))
			result = Results_SUCCESS;
	}

	// Set value column width to optimal
	TwSetParam(bar, NULL, "valueswidth", TW_PARAM_CSTRING, 1, "fit");

	return result;
}

void DebugGUI::setSize( string p_barName, int p_x, int p_y )
{
	int result;
	result = TwDefine(stringFromParams( p_barName, "", "size", p_x, p_y ).c_str());
}

void DebugGUI::setPosition( string p_barName, int p_x, int p_y )
{
	int result;
	result = TwDefine(stringFromParams( p_barName, "", "position", p_x, p_y ).c_str());
}


void DebugGUI::hideAllBars()
{
	/*bool moreBars = true;
	while( moreBars ) {*/

	int numBars = TwGetBarCount();

	for( int i=0; i<numBars; i++ )
	{
		TwBar* bar = TwGetBarByIndex(i);
		if( bar != NULL ){
			string barName( TwGetBarName(bar) );

			// if the bar name contains space, add '.
			if( barName.find( " ", 1 ) != barName.npos ) {
				barName.insert( 0,"'" );
				barName.append( "'" );
			}
			hideBar(barName);
		}
	}
}


void DebugGUI::hideBar( string p_barName )
{
	stringstream ss;
	ss << p_barName;
	ss << " iconified=true";

	int result;
	result = TwDefine( ss.str().c_str() );
}

void DebugGUI::draw()
{
	TwDraw();
}

int DebugGUI::updateMsgProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam )
{
	return TwEventWin(wnd, msg, wParam, lParam);
}

void DebugGUI::terminate()
{
	TwTerminate();
}


//=========================================================================
// Private functions
//=========================================================================
DebugGUI::DebugGUI()
{

}

TwBar* DebugGUI::barFromString( string p_barName )
{
	TwBar* bar = TwGetBarByName(p_barName.c_str());

	if(bar == NULL)
	{
		bar = TwNewBar(p_barName.c_str());
		//TwDefine("DebugGUI size='380 720' position='900 0'");
	}
	return bar;
}

string DebugGUI::stringFromParams( string p_barName, string p_varName,
								  string p_paramName, int p_arg)
{
	vector<int> args;
	args.push_back(p_arg);
	return stringFromParams(p_barName, p_varName, p_paramName, args);
}

string DebugGUI::stringFromParams( string p_barName, string p_varName,
								  string p_paramName, int p_arg1, int p_arg2)
{
	vector<int> args;
	args.push_back(p_arg1);
	args.push_back(p_arg2);
	return stringFromParams(p_barName, p_varName, p_paramName, args);
}

string DebugGUI::stringFromParams( string p_barName, string p_varName,
								  string p_paramName, vector<int> p_args )
{
	barFromString(p_barName);

	stringstream ss;
	ss << p_barName;
	if(p_varName.length() > 0)
	{
		ss<<"/"<<p_varName;
	}
	ss<<" "<<p_paramName<<"=";

	// Create space separated string list of args surrounded by '.
	ss<<"'"<<p_args[0];
	for( int i=1; i<(int)p_args.size(); i++ )
	{
		ss<<" "<<p_args[i];
	}
	ss<<"'";

	string result = ss.str();
	return ss.str();
}