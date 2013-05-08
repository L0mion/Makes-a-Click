#ifndef XKILL_RENDERER_D3DDEBUG_H
#define XKILL_RENDERER_D3DDEBUG_H

#pragma comment( lib , "dxgi.lib" )

#include <d3d11.h>

#if defined(DEBUG) || defined(_DEBUG)
#include <DXGI.h> 
#include <Initguid.h> 
#include <DXGIDebug.h>
#include <D3D11SDKLayers.h>
#endif //DEBUG || _DEBUG

//#include "d3dInterface.h"

//! Wraps d3d-debug COM-objects that allow detailed printing of live objects at run-time.
/*!
If DEBUG- or _DEBUG-flags are set, D3DDebug will be empty, and have no functionality whatsoever.
Warning: these members will be reported as live objects, and may influence other COM-objects to do the same. Use with caution.
\ingroup xkill-renderer
\sa DEBUG
\sa _DEBUG
*/
class ManagementDebug
{
public:
  //! Sets object to default state and nulls all members.
	ManagementDebug();
	//! Clears memory and resets to default state.
	~ManagementDebug();

	//! Clears memory and resets to default state.
	void reset();

	//! Initializes all members. Must be called before usage of report-functionality.
	/*!
	\param device DirectX Device pointer.
	\return Any eror encountered.
	*/
	HRESULT init(ID3D11Device* device);

	//! Prints all live COM-objects.
	/*!
	\return Any error encountered.
	*/
	HRESULT reportLiveDeviceObjects();
	//! Prints all live objects.
	/*!
	\return Any error encountered.
	*/
	HRESULT reportLiveObjects();
private:
	#if defined(DEBUG) || defined(_DEBUG)
	IDXGIFactory1*	dxgiFactory_;	//!< Will not be created if DEBUG- or _DEBUG-flags are not set.
	ID3D11Debug*	d3d11Debug_;		//!< Will not be created if DEBUG- or _DEBUG-flags are not set.
	IDXGIDebug*		dxgiDebug_;		//!< Will not be created if DEBUG- or _DEBUG-flags are not set.
	IDXGIInfoQueue* dxgiInfoQueue_;	//!< Will not be created if DEBUG- or _DEBUG-flags are not set.
	#endif //DEBUG || _DEBUG
};

#endif //XKILL_RENDERER_D3DDEBUG_H