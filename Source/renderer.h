#ifndef RENDERER_H
#define RENDERER_H

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

class ManagementD3D;
class ManagementShader;
class ManagementCB;
class ManagementTex;
class ManagementSprite;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void render();
	void update(DirectX::XMFLOAT4X4 finalMatrix);

	HRESULT init(HWND windowHandle);

	ManagementD3D* getD3DManagement();

private:
	HRESULT initManagementD3D(HWND windowHandle);
	HRESULT initManagementShader(ID3D11Device* device);
	HRESULT initManagementCB(ID3D11Device* device);
	HRESULT initManagementTex(ID3D11Device* device);
	HRESULT initManagementSprite(ID3D11Device* device);

	ManagementD3D*	  managementD3D_;
	ManagementShader* managementShader_;
	ManagementCB*	  managementCB_;
	ManagementTex*	  managementTex_;
	ManagementSprite* managementSprite_;

	/*TEMP*/
	void renderSprite();

	ID3D11Buffer* vertexBuffer_;
	ID3D11Buffer* indexBuffer_;

	void createVertices();
	void createIndices();

};

#endif //RENDERER_H