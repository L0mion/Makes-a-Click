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
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	managementShader_->setShader(SHADER_ID_CS_PRIMARY_RAY_STAGE, devcon);
	managementD3D_->csSetUAV(UAV_ID_BACK_BUFFER, 0);
	devcon->Dispatch(25, 25, 1);

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