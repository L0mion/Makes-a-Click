#include <vector>

#include <DirectXMath.h>

#include "renderer.h"
#include "managementD3D.h"
#include "managementShader.h"
#include "managementCB.h"
#include "utility.h"
#include "vertex.h"

Renderer::Renderer()
{
	managementD3D_	  = NULL;
	managementShader_ = NULL;
	managementCB_	  = NULL;

	vertexBuffer_ = NULL;
	indexBuffer_ = NULL;
}
Renderer::~Renderer()
{
	SAFE_DELETE(managementD3D_);
	SAFE_DELETE(managementShader_);
	SAFE_DELETE(managementCB_);

	SAFE_RELEASE(vertexBuffer_);
	SAFE_RELEASE(indexBuffer_);
}

void Renderer::render()
{
	FLOAT colorBlack[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	devcon->ClearRenderTargetView(managementD3D_->getRTVBackBuffer(), colorBlack);
	devcon->ClearDepthStencilView(managementD3D_->getDSVDepthBuffer(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	managementD3D_->setBackBuffer();

	managementShader_->setShader(devcon, SHADER_ID_VS_DEFAULT);
	managementShader_->setShader(devcon, SHADER_ID_PS_DEFAULT);
	managementShader_->setInputLayout(devcon, INPUT_LAYOUT_ID_VS_DEFAULT);

	UINT stride = sizeof(Vertex);
	UINT offset = 0;

	devcon->OMSetDepthStencilState(0, 0);

	devcon->IASetVertexBuffers(0, 1, &vertexBuffer_, &stride, &offset);
	devcon->IASetIndexBuffer(indexBuffer_, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->DrawIndexed(36, 0, 0);

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
	createVertices();
	createIndices();

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
HRESULT Renderer::initManagementCB(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	managementCB_ = new ManagementCB();
	hr = managementCB_->init(device);
	return hr;
}


/*TEMP*/

void Renderer::createVertices()
{
	std::vector<Vertex> vertices;

	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)));   
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)));
																		
	// side 2															 
	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)));    
	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)));
																		
	// side 3															 
	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f))); 
	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)));
																		
	// side 4																 
	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)));  
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)));
																		
	// side 5																	 
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)));  
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, -1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, -1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)));
							  											
	// side 6				  											
	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, -1.0f),	DirectX::XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)));  
	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, -1.0f, 1.0f),	DirectX::XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, -1.0f),	DirectX::XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)));
	vertices.push_back(Vertex(DirectX::XMFLOAT3(-1.0f, 1.0f, 1.0f),		DirectX::XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)));

	D3D11_BUFFER_DESC vbd;
	vbd.Usage = D3D11_USAGE_DYNAMIC;
	vbd.ByteWidth = sizeof(Vertex) * vertices.size();
	vbd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vbd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vbd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vinitData;
	vinitData.pSysMem = &vertices[0];
	ID3D11Device* device = managementD3D_->getDevice();
	HRESULT hr = device->CreateBuffer(&vbd, &vinitData, &vertexBuffer_);
}
void Renderer::createIndices()
{
	std::vector<DWORD> indices;

	indices.push_back(0);
	indices.push_back(1);
	indices.push_back(2);

	indices.push_back(2);
	indices.push_back(1);
	indices.push_back(3);

	indices.push_back(4);
	indices.push_back(5);
	indices.push_back(6);

	indices.push_back(6);
	indices.push_back(5);
	indices.push_back(7);

	indices.push_back(8);
	indices.push_back(9);
	indices.push_back(10);

	indices.push_back(10);
	indices.push_back(9);
	indices.push_back(11);

	indices.push_back(12);
	indices.push_back(13);
	indices.push_back(14);

	indices.push_back(14);
	indices.push_back(13);
	indices.push_back(15);

	indices.push_back(16);
	indices.push_back(17);
	indices.push_back(18);

	indices.push_back(18);
	indices.push_back(17);
	indices.push_back(19);

	indices.push_back(20);
	indices.push_back(21);
	indices.push_back(22);

	indices.push_back(22);
	indices.push_back(21);
	indices.push_back(23);

	D3D11_BUFFER_DESC ibd;
	ibd.Usage = D3D11_USAGE_DYNAMIC;
	ibd.ByteWidth = sizeof(DWORD) * indices.size();
	ibd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	ibd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	ibd.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA iinitData;
	iinitData.pSysMem = &indices[0];
	ID3D11Device* device = managementD3D_->getDevice();
	HRESULT hr = device->CreateBuffer(&ibd, &iinitData, &indexBuffer_);
}