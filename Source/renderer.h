#ifndef RENDERER_H
#define RENDERER_H

#include <Windows.h>
#include <d3d11.h>

class ManagementD3D;
class ManagementShader;

class Renderer
{
public:
	Renderer();
	~Renderer();

	void render();
	void update();

	HRESULT init(HWND windowHandle);
private:
	HRESULT initManagementD3D(HWND windowHandle);
	HRESULT initManagementShader(ID3D11Device* device);

	ManagementD3D*	  managementD3D_;
	ManagementShader* managementShader_;
};

#endif //RENDERER_H