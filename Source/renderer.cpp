#include "renderer.h"
#include "managementD3D.h"
#include "managementShader.h"
#include "utility.h"

Renderer::Renderer()
{
	managementD3D_	  = NULL;
	managementShader_ = NULL;
}
Renderer::~Renderer()
{
	SAFE_DELETE(managementD3D_);
	SAFE_DELETE(managementShader_);
}

void Renderer::render()
{
	FLOAT colorBlack[4] = {0, 0, 0, 0};
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	devcon->ClearRenderTargetView(managementD3D_->getRTVBackBuffer(), colorBlack);
	devcon->ClearDepthStencilView(managementD3D_->getDSVDepthBuffer(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	managementD3D_->present();
}
void Renderer::update()
{
}

HRESULT Renderer::init(HWND windowHandle)
{
	HRESULT hr = S_OK;

	hr = initManagementD3D(windowHandle);
	if(SUCCEEDED(hr))
		hr = initManagementShader(managementD3D_->getDevice());

	return hr;
}
HRESULT Renderer::initManagementD3D(HWND windowHandle)
{
	HRESULT hr = S_OK;
	managementD3D_ = new ManagementD3D();
	hr = managementD3D_->init(windowHandle);
	return hr;
}
HRESULT Renderer::initManagementShader(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	managementShader_ = new ManagementShader();
	hr = managementShader_->init(device);
	return hr;
}