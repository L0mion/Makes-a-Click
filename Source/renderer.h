#ifndef RENDERER_H
#define RENDERER_H

#include <Windows.h>
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>

class HeightMap;
class ManagementD3D;
class ManagementShader;
class ManagementCB;
class ManagementTex;
class ManagementSprite;
class ManagementSS;
class ManagementBS;

struct EntityBufferInfo;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void beginRender();
	void renderHeightMap( HeightMap* p_heightMap );
	void renderSprites();
	void renderEntities();
	void renderEntityBufferInfo( EntityBufferInfo* p_info );
	void endRender();
	void update( DirectX::XMFLOAT4X4 p_finalMatrix,
		DirectX::XMFLOAT3 p_cameraPos );

	HRESULT init(HWND windowHandle);

	ManagementD3D* getD3DManagement();

	/// Takes ownership of the identity
	void addEntity( EntityBufferInfo* p_entity );

private:
	HRESULT initManagementD3D(HWND windowHandle);
	HRESULT initManagementShader(ID3D11Device* device);
	HRESULT initManagementCB(ID3D11Device* device);
	HRESULT initManagementTex(ID3D11Device* device);
	HRESULT initManagementSprite(ID3D11Device* device);
	HRESULT initManagementSS(ID3D11Device* device);
	HRESULT initManagementBS(ID3D11Device* device);

	ManagementD3D*	  managementD3D_;
	ManagementShader* managementShader_;
	ManagementCB*	  managementCB_;
	ManagementTex*	  managementTex_;
	ManagementSprite* managementSprite_;
	ManagementSS*	  managementSS_;
	ManagementBS*	  managementBS_;

	//EntityBufferInfo* m_cube;

	vector<EntityBufferInfo*> m_entities;

	//ID3D11Buffer* vertexBuffer_;
	//ID3D11Buffer* indexBuffer_;

};

#endif //RENDERER_H