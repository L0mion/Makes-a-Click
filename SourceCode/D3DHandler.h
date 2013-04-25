#ifndef D3DHANDLER_H
#define D3DHANDLER_H

#include <D3D11.h>
#include <D3DX11.h>
#include <D3DX10.h>

#include <Windows.h>
#include <WindowsX.h>

#include "Utility.h"

class D3DHandler
{
private:
	
	IDXGISwapChain *swapChain;
	ID3D11Device *device;
	ID3D11DeviceContext *devcon;
	ID3D11RenderTargetView *backBuffer;
	ID3D11DepthStencilView *zBuffer;
	ID3D11RasterizerState *rasterizerState;
	
	char* featureLevelToString(D3D_FEATURE_LEVEL featureLevel);

	void createSwapChainAndDevice(HWND hWnd);
	void createDepthBuffer();
	void createBackBuffer();
	void createViewport();
	void createRasterizerState();

public:
	D3DHandler();
	~D3DHandler();

	void initialize(HWND hWnd);

	ID3D11Device* getDevice() const;
	ID3D11DeviceContext* getDeviceContext() const;
	void present();
	void clearRenderTargetsAndZBuffer();
};

#endif