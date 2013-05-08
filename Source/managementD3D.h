#ifndef MANAGEMENT_D3D_H
#define MANAGEMENT_D3D_H

#include <Windows.h>
#include <d3d11.h>

enum UAVId
{
	UAV_ID_BACK_BUFFER
};

class ManagementD3D
{
public:
	ManagementD3D();
	~ManagementD3D();

	void present();
	
	void setBackBuffer();
	void setBackBufferNoDepth();
	void clearBackBuffer();


	ID3D11Device* getDevice() const;
	ID3D11DeviceContext* getDeviceContext() const;
	ID3D11RenderTargetView* getRTVBackBuffer();
	ID3D11DepthStencilView* getDSVDepthBuffer();

	HRESULT init(HWND windowHandle);
private:
	HRESULT initDeviceAndSwapChain(HWND windowHandle);
	HRESULT initBackBuffer();
	HRESULT initDepthBuffer();
	void initViewport();
	

	IDXGISwapChain* swapChain_;
	ID3D11Device* device_;
	ID3D11DeviceContext* devcon_;
	ID3D11UnorderedAccessView* uavBackBuffer_;
	ID3D11RenderTargetView* rtvBackBuffer_;
	ID3D11DepthStencilView* dsvDepthBuffer_;
};

#endif // MANAGEMENT_D3D_H