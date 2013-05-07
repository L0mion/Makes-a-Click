#include <vector>

#include <DirectXMath.h>

#include "CubeFactory.h"
#include "DebugGUI.h"
#include "EntityBufferInfo.h"
#include "managementBS.h"
#include "managementCB.h"
#include "managementD3D.h"
#include "managementShader.h"
#include "managementTex.h"
#include "managementSprite.h"
#include "managementSS.h"
#include "sprite.h"
#include "renderer.h"
#include "utility.h"
#include "vertex.h"

Renderer::Renderer()
{
	managementD3D_	  = NULL;
	managementShader_ = NULL;
	managementCB_	  = NULL;
	managementTex_	  = NULL;
	managementSprite_ = NULL;
	managementSS_     = NULL;
	managementBS_	  = NULL;
}

Renderer::~Renderer()
{
	SAFE_DELETE(managementD3D_);
	SAFE_DELETE(managementShader_);
	SAFE_DELETE(managementCB_);
	SAFE_DELETE(managementTex_);
	SAFE_DELETE(managementSprite_);
	SAFE_DELETE(managementSS_);
	SAFE_DELETE(managementBS_);

	for( unsigned int i=0; i<m_entities.size(); i++ ) {
		SAFE_DELETE( m_entities[i] );
	}
}

void Renderer::beginRender()
{
	FLOAT colorBlack[4] = {0.0f, 0.0f, 0.0f, 0.0f};
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	managementD3D_->clearBackBuffer();
	devcon->ClearDepthStencilView(managementD3D_->getDSVDepthBuffer(), D3D11_CLEAR_DEPTH, 1.0f, 0);

	managementD3D_->setBackBuffer();

	managementShader_->setShader(devcon, ManagementShader::ShaderIds_VS_DEFAULT);
	managementShader_->setShader(devcon, ManagementShader::ShaderIds_PS_DEFAULT);
	managementShader_->setInputLayout(devcon, ManagementShader::InputLayoutIds_VS_DEFAULT);

}

void Renderer::renderHeightMap( HeightMap* p_heightMap )
{
	EntityBufferInfo* info = NULL;
	//info = p_heightMap->getEntityBufferInfo();
}

void Renderer::renderSprites()
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	managementShader_->setShader(devcon, ManagementShader::ShaderIds_VS_SPRITE);
	managementShader_->setShader(devcon, ManagementShader::ShaderIds_PS_SPRITE);
	managementShader_->setInputLayout(devcon, ManagementShader::InputLayoutIds_VS_SPRITE);

	managementBS_->setBlendState(devcon, ManagementBS::BSTypes_TRANSPARENCY);

	managementCB_->vsSetCB(devcon, ManagementCB::CBTypes_SPRITE);
	Sprite* sprite = managementSprite_->getSprite(ManagementSprite::SpriteIds_PLACEHOLDER);
	DirectX::XMFLOAT4X4 spriteTransform = sprite->getWorldMatrix();
	managementCB_->updateCBSprite(devcon, spriteTransform);

	managementTex_->psSetTexture(devcon, TextureIds::TextureIds_PLACEHOLDER, 0);
	managementSS_->setSS(devcon, ManagementSS::SSTypes_DEFAULT, 0);

	UINT stride = sizeof(SpriteVertex);
	UINT offset = 0;

	ID3D11Buffer* vertexBuffer = managementSprite_->getVertexBuffer();
	ID3D11Buffer* indexBuffer = managementSprite_->getIndexBuffer();

	devcon->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->DrawIndexed(6, 0, 0);

	managementBS_->setBlendState(devcon, ManagementBS::BSTypes_DEFAULT);
}

void Renderer::renderEntities()
{
	for( unsigned int i=0; i<m_entities.size(); i++ ) {
		renderEntityBufferInfo( m_entities[i] );
	}
}

void Renderer::renderEntityBufferInfo( EntityBufferInfo* p_info )
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();

	UINT stride = p_info->m_stride;
	UINT offset = 0;

	devcon->OMSetDepthStencilState(0, 0);

	devcon->IASetVertexBuffers(0, 1, &p_info->m_vertexBuffer, &stride, &offset);
	devcon->IASetIndexBuffer(p_info->m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	devcon->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	devcon->DrawIndexed(p_info->m_indicesCnt, 0, 0);
}

void Renderer::endRender()
{
	DebugGUI::getInstance()->draw();

	managementD3D_->present();
}

void Renderer::update(DirectX::XMFLOAT4X4 finalMatrix)
{
	ID3D11DeviceContext* devcon = managementD3D_->getDeviceContext();
	managementCB_->vsSetCB(devcon, ManagementCB::CBTypes_FRAME);
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
	if(SUCCEEDED(hr))
		hr = initManagementTex(managementD3D_->getDevice());
	if(SUCCEEDED(hr))
		hr = initManagementSprite(managementD3D_->getDevice());
	if(SUCCEEDED(hr))
		hr = initManagementSS(managementD3D_->getDevice());
	if(SUCCEEDED(hr))
		hr = initManagementBS(managementD3D_->getDevice());

	//TEMP
	EntityBufferInfo* cube = NULL;
	CubeFactory::createCube( managementD3D_, &cube );
	addEntity( cube );

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
HRESULT Renderer::initManagementTex(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	managementTex_ = new ManagementTex();
	managementTex_->init(device);
	return hr;
}
HRESULT Renderer::initManagementSprite(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	managementSprite_ = new ManagementSprite();
	managementSprite_->init(device);
	return hr;
}
HRESULT Renderer::initManagementSS(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	managementSS_ = new ManagementSS();
	managementSS_->init(device);
	return hr;
}
HRESULT Renderer::initManagementBS(ID3D11Device* device)
{
	HRESULT hr = S_OK;
	managementBS_ = new ManagementBS();
	managementBS_->init(device);
	return hr;
}

void Renderer::addEntity( EntityBufferInfo* p_entity )
{
	if ( p_entity ) {
		m_entities.push_back( p_entity );
	}
}

