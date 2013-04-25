#include "managementD3D.h"
#include "utility.h"

ManagementD3D::ManagementD3D()
{
	swapChain_		= NULL;
	device_			= NULL;
	devcon_			= NULL;
	uavBackBuffer_	= NULL;
}
ManagementD3D::~ManagementD3D()
{
	SAFE_RELEASE(swapChain_);
	SAFE_RELEASE(device_);
	SAFE_RELEASE(devcon_);
	SAFE_RELEASE(uavBackBuffer_);
}

void ManagementD3D::present()
{
	if(swapChain_)
		swapChain_->Present(0, 0);
}
void ManagementD3D::csSetUAV(UAVId uavId, unsigned int startSlot)
{
	ID3D11UnorderedAccessView* uav = NULL;

	switch(uavId)
	{
	case UAV_ID_BACK_BUFFER:
		uav = uavBackBuffer_;
		break;
	}

	devcon_->CSSetUnorderedAccessViews(startSlot, 1, &uav, NULL);
}


ID3D11Device* ManagementD3D::getDevice() const
{
	return device_;
}
ID3D11DeviceContext* ManagementD3D::getDeviceContext() const
{
	return devcon_;
}

HRESULT ManagementD3D::init(HWND windowHandle)
{
	HRESULT hr = S_OK;
	hr = initDeviceAndSwapChain(windowHandle);
	if(SUCCEEDED(hr))
		hr = initBackBuffer();
	if(SUCCEEDED(hr))
		initViewport();

	return hr;
}

HRESULT ManagementD3D::initDeviceAndSwapChain(HWND windowHandle)
{
	HRESULT hr = S_OK;

	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(scd));

	scd.BufferCount							= 1;
	scd.BufferDesc.Format					= DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width					= SCREEN_WIDTH;
	scd.BufferDesc.Height					= SCREEN_HEIGHT;
	scd.BufferUsage							= DXGI_USAGE_RENDER_TARGET_OUTPUT | DXGI_USAGE_UNORDERED_ACCESS;
	scd.OutputWindow						= windowHandle;
	scd.BufferDesc.RefreshRate.Numerator	= 60;
	scd.BufferDesc.RefreshRate.Denominator	= 1;
	scd.SampleDesc.Count					= 1;
	scd.SampleDesc.Quality					= 0;
	scd.Windowed							= true;
	scd.Flags								= DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT numFeatureLevels = 3;
	D3D_FEATURE_LEVEL initiatedFeatureLevel;
	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0,
										 D3D_FEATURE_LEVEL_10_1,
										 D3D_FEATURE_LEVEL_10_0};
	UINT numDriverTypes = 2;
	D3D_DRIVER_TYPE driverTypes[] = {D3D_DRIVER_TYPE_HARDWARE,
									 D3D_DRIVER_TYPE_REFERENCE};

	UINT createDeviceFlags = 0;
#if defined( DEBUG ) || defined( _DEBUG )
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	unsigned int index = 0;
	bool deviceCreated = false;
	while(index < numDriverTypes && !deviceCreated)
	{
		hr = D3D11CreateDeviceAndSwapChain(NULL,
										   driverTypes[index],
										   NULL,
										   createDeviceFlags,
										   featureLevels,
										   numFeatureLevels,
										   D3D11_SDK_VERSION,
										   &scd,
										   &swapChain_,
										   &device_,
										   &initiatedFeatureLevel,
										   &devcon_);
		if(SUCCEEDED(hr))
		{
			deviceCreated = true;
		}
		index++;
	}

	return hr;
}
HRESULT ManagementD3D::initBackBuffer()
{
	HRESULT hr = S_OK;

	ID3D11Texture2D* texBackBuffer;
	swapChain_->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texBackBuffer);
	hr = device_->CreateUnorderedAccessView(texBackBuffer, NULL, &uavBackBuffer_);
	SAFE_RELEASE(texBackBuffer);

	return hr;
}
void ManagementD3D::initViewport()
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(viewport));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width	  = SCREEN_WIDTH;
	viewport.Height	  = SCREEN_HEIGHT;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	devcon_->RSSetViewports(1, &viewport);
}