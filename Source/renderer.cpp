#include <vector>

#include <DirectXMath.h>

#include "DebugGUI.h"
#include "managementCB.h"
#include "managementD3D.h"
#include "managementShader.h"
#include "renderer.h"
#include "utility.h"
#include "vertex.h"
#include "CubeFactory.h"
#include "EntityBufferInfo.h"

Renderer::Renderer()
{
	managementD3D_		= NULL;
	managementShader_	= NULL;
	managementCB_		= NULL;
	m_cube				= NULL;
}

Renderer::~Renderer()
{
	SAFE_DELETE( managementD3D_ );
	SAFE_DELETE( managementShader_ );
	SAFE_DELETE( managementCB_ );
	SAFE_DELETE( m_cube );
}

void Renderer::beginRender()
{
	FLOAT colorBlack[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	managementD3D_->clearBackBuffer();
	devcon->ClearDepthStencilView(managementD3D_->getDSVDepthBuffer(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	managementD3D_->setBackBuffer();

	managementShader_->setShader(devcon, SHADER_ID_VS_DEFAULT);
	managementShader_->setShader(devcon, SHADER_ID_PS_DEFAULT);
	managementShader_->setInputLayout(devcon, INPUT_LAYOUT_ID_VS_DEFAULT);
}

void Renderer::renderHeightMap( HeightMap* m_heightMap )
{

}

void Renderer::renderCube()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	devcon->OMSetDepthStencilState(0, 0);

	devcon->IASetVertexBuffers(0, 1, &m_cube->m_vertexBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(m_cube->m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->DrawIndexed(36, 0, 0);
}

void Renderer::endRender()
{
	DebugGUI::getInstance()->draw();

	managementD3D_->present();
}

void Renderer::update(DirectX::XMFLOAT4X4 finalMatrix)
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	managementCB_->vsSetCB(devcon, CB_TYPE_FRAME);
	managementCB_->updateCBFrame(devcon, finalMatrix);
}

HRESULT Renderer::init(HWND windowHandle)
{
	HRESULT hr = S_OK;

	hr = initManagementD3D(windowHandle);
	if(SUCCEEDED(hr))
		hr = initManagementShader(managementD3D_->getDevice());
	if(SUCCEEDED(hr))
		hr = initManagementCB(managementD3D_->getDevice());

	//TEMP
	CubeFactory::createCube( managementD3D_, &m_cube );

	return hr;
}

ManagementD3D* Renderer::getD3DManagement()
{
	return managementD3D_;
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
HRESULT Renderer::initManagementCB(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	managementCB_ = new ManagementCB();
	hr = managementCB_->init(device);
	return hr;
}