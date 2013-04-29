#ifndef	DebugGUI_h
#define	DebugGUI_h

#include <sstream>
#include <string>
#include <vector>

#include <windows.h>
#include <windowsx.h>
#include <d3d11.h>
#include <AntTweakBar.h>

#include "ManagementD3D.h"
#include "utility.h"

using namespace std;

// Pre def
class ManagementD3D;

class DebugGUI
{
//=========================================================================
// Enums
//=========================================================================
public:
	enum Types
	{
		Types_BOOL		= TW_TYPE_BOOLCPP,
		Types_INT		= TW_TYPE_INT32,
		Types_FLOAT		= TW_TYPE_FLOAT,
		Types_COLOR		= TW_TYPE_COLOR4F,
		Types_VEC3		= TW_TYPE_DIR3F
	};
	enum Permissions	{ Permissions_READ_ONLY, Permissions_READ_WRITE };
	enum Results			{ Results_FAILED, Results_SUCCESS };

//=========================================================================
// Functions
//=========================================================================
public:
	TwBar* barFromString( string p_barName ); // Should be private

	static DebugGUI* getInstance();
	void init( ManagementD3D* p_deviceHandler, HWND p_handle );
	Results addVar( string p_barName, Types p_type, Permissions p_permissions,
		string p_name, void *p_var, string p_options );

	void setSize( string p_barName, int p_x, int p_y );
	void setPosition( string p_barName, int p_x, int p_y);

	/** Returns zero on fail and nonzero on success as per TwEventWin */ 
	int updateMsgProc( HWND wnd, UINT msg, WPARAM wParam, LPARAM lParam );
	void draw();
	void terminate();

private:
	DebugGUI();
	DebugGUI(DebugGUI const&);			// Don't Implement
	void operator=(DebugGUI const&);		// Don't implement

	string stringFromParams( string p_barName, string p_varName,
		string p_paramName, int p_arg);
	string stringFromParams( string p_barName, string p_varName,
		string p_paramName, int p_arg1, int p_arg2);
	string stringFromParams( string p_barName, string p_varName,
		string p_paramName, vector<int> p_args);

//=========================================================================
// Variables
//=========================================================================
private:	
	ManagementD3D* m_deviceHandler;
};

#endif	//DebugGUI_h