#include "D3DHandler.h"


D3DHandler::D3DHandler()
{

}

D3DHandler::~D3DHandler()
{
	swapChain->Release();
	backBuffer->Release();
	device->Release();
	devcon->Release();
	
	zBuffer->Release();
	rasterizerState->Release();
}

char* D3DHandler::featureLevelToString(D3D_FEATURE_LEVEL featureLevel)
{
	if(featureLevel == D3D_FEATURE_LEVEL_11_0)
		return "11.0";
	if(featureLevel == D3D_FEATURE_LEVEL_10_1)
		return "10.1";
	if(featureLevel == D3D_FEATURE_LEVEL_10_0)
		return "10.0";

	return "Unknown";
}

void D3DHandler::createSwapChainAndDevice(HWND hWnd)
{
	DXGI_SWAP_CHAIN_DESC scd;
	ZeroMemory(&scd, sizeof(DXGI_SWAP_CHAIN_DESC));

	scd.BufferCount = 1;
	scd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	scd.BufferDesc.Width = SCREEN_WIDTH;
	scd.BufferDesc.Height = SCREEN_HEIGHT;
	scd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	scd.OutputWindow = hWnd;
	scd.SampleDesc.Count = 4;
	scd.Windowed = true;
	scd.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;

	UINT numFeatureLevels = 3;
	D3D_FEATURE_LEVEL initiatedFeatureLevel;
	D3D_FEATURE_LEVEL featureLevels[] = {D3D_FEATURE_LEVEL_11_0,
										 D3D_FEATURE_LEVEL_10_1,
										 D3D_FEATURE_LEVEL_10_0,};

	UINT numDriverTypes = 2;
	D3D_DRIVER_TYPE driverTypes[] = {	D3D_DRIVER_TYPE_HARDWARE,
										D3D_DRIVER_TYPE_REFERENCE};

	int index = 0;
	bool deviceCreated = false;
	while(index < numDriverTypes && !deviceCreated)
	{	
		HRESULT hr;
		D3D_DRIVER_TYPE driverType = driverTypes[index];
		hr = D3D11CreateDeviceAndSwapChain(	NULL,
											driverType, //D3D_DRIVER_TYPE_HARDWARE,
											NULL,
											NULL, //D3D11_CREATE_DEVICE_DEBUG,	//Enables shader debugging with PIX
											featureLevels,
											numFeatureLevels,
											D3D11_SDK_VERSION, 
											&scd,
											&swapChain,
											&device,
											&initiatedFeatureLevel,
											&devcon);
		if(SUCCEEDED(hr))
		{
			char title[256];
			sprintf_s(	title,
						sizeof(title),
						"Wonderland | Direct3D 11.0 device initiated with Direct3D %s feature level",
						featureLevelToString(initiatedFeatureLevel));
			
			//convert title to a unicode string
			int a = lstrlenA(title);
			BSTR unicodestr = SysAllocStringLen(NULL, a);
			::MultiByteToWideChar(CP_ACP, 0, title, a, unicodestr, a);

			SetWindowText(hWnd, unicodestr);

			::SysFreeString(unicodestr);
			deviceCreated = true;
		}

		index++;
	}
}

void D3DHandler::createDepthBuffer()
{
	D3D11_TEXTURE2D_DESC texd;
	ZeroMemory(&texd, sizeof(texd));

	texd.Width = SCREEN_WIDTH;
	texd.Height = SCREEN_HEIGHT;
	texd.ArraySize = 1;
	texd.MipLevels = 1;
	texd.SampleDesc.Count = 4;
	texd.Format = DXGI_FORMAT_D32_FLOAT;
	texd.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	ID3D11Texture2D *depthBuffer;
	device->CreateTexture2D(&texd, NULL, &depthBuffer);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	ZeroMemory(&dsvd, sizeof(dsvd));
	dsvd.Format = DXGI_FORMAT_D32_FLOAT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2DMS;

	device->CreateDepthStencilView(depthBuffer, &dsvd, &zBuffer);
	depthBuffer->Release();
}

void D3DHandler::createBackBuffer()
{
	ID3D11Texture2D *texBackBuffer;
	swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&texBackBuffer);

	device->CreateRenderTargetView(texBackBuffer, NULL, &backBuffer);
	texBackBuffer->Release();

	devcon->OMSetRenderTargets(1, &backBuffer, zBuffer);
}

void D3DHandler::createViewport()
{
	D3D11_VIEWPORT viewport;
	ZeroMemory(&viewport, sizeof(D3D11_VIEWPORT));

	viewport.TopLeftX = 0;
	viewport.TopLeftY = 0;
	viewport.Width = SCREEN_WIDTH;
	viewport.Height = SCREEN_HEIGHT;
	viewport.MinDepth = 0;
	viewport.MaxDepth = 1;

	devcon->RSSetViewports(1, &viewport);
}

void D3DHandler::createRasterizerState()
{
	D3D11_RASTERIZER_DESC rsd;
	rsd.CullMode = D3D11_CULL_BACK;
	rsd.FillMode = D3D11_FILL_SOLID;
	rsd.FrontCounterClockwise = false;
	rsd.DepthBias = false;
	rsd.DepthBiasClamp = 0;
	rsd.SlopeScaledDepthBias = 0;
	rsd.DepthClipEnable = true;
	rsd.ScissorEnable = false;
	rsd.MultisampleEnable = false;
	rsd.AntialiasedLineEnable = true;

	device->CreateRasterizerState(&rsd, &rasterizerState);
	devcon->RSSetState(rasterizerState);
}

void D3DHandler::initialize(HWND hWnd)
{
	createSwapChainAndDevice(hWnd);
	createDepthBuffer();
	createBackBuffer();
	createViewport();
	createRasterizerState();
}

ID3D11Device* D3DHandler::getDevice() const
{
	return device;
}

ID3D11DeviceContext* D3DHandler::getDeviceContext() const
{
	return devcon;
}

void D3DHandler::present()
{
	swapChain->Present(0, 0);
}

void D3DHandler::clearRenderTargetsAndZBuffer()
{
	devcon->ClearRenderTargetView(backBuffer, BLACK);
	devcon->ClearDepthStencilView(zBuffer, D3D11_CLEAR_DEPTH, 1.0f, 0);
}