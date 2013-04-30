#ifndef RENDERER_H
#define RENDERER_H

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>

class HeightMap;
class ManagementD3D;
class ManagementShader;
class ManagementCB;

struct EntityBufferInfo;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void beginRender();
	void renderHeightMap( HeightMap* p_heightMap );
	void renderCube();
	void renderEntityBufferInfo( EntityBufferInfo* p_info );
	void endRender();
	void update(DirectX::XMFLOAT4X4 finalMatrix);

	HRESULT init(HWND windowHandle);

	ManagementD3D* getD3DManagement();

private:
	HRESULT initManagementD3D(HWND windowHandle);
	HRESULT initManagementShader(ID3D11Device* device);
	HRESULT initManagementCB(ID3D11Device* device);

	ManagementD3D*	  managementD3D_;
	ManagementShader* managementShader_;
	ManagementCB*	  managementCB_;

	/*TEMP*/

	EntityBufferInfo* m_cube;

	//ID3D11Buffer* vertexBuffer_;
	//ID3D11Buffer* indexBuffer_;

};

#endif //RENDERER_H