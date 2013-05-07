#ifndef MANAGEMENT_SHADER_H
#define MANAGEMENT_SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

struct ShaderSet
{
	ID3D11VertexShader* m_vs;
	ID3D11PixelShader*	m_ps;

	ID3DBlob* m_vsBlob;
	ID3DBlob* m_psBlob;

	ID3D11InputLayout* m_inputLayout;

	ShaderSet();
	void setNull();
	void safeRelease();
	void set( ID3D11DeviceContext* p_devcon );
};

class ManagementShader
{
public:
	ManagementShader();
	~ManagementShader();
	
	enum ShaderIds
	{
		ShaderIds_NONE,
		ShaderIds_DEFAULT,
		ShaderIds_SPRITE,
		ShaderIds_HEIGHTMAP,
		ShaderIds_CNT
	};

	void setShader(ID3D11DeviceContext* devcon, ShaderIds shaderId);
	/*void setInputLayout(ID3D11DeviceContext* devcon, ShaderIds inputLayoutId);*/

	HRESULT init(ID3D11Device* device);
private:
	HRESULT initShaders(ID3D11Device* device);
	HRESULT initVertexShader(ID3D11Device* device, std::wstring filePath, std::wstring fileName, ID3D11VertexShader** shader, ID3DBlob** blob);
	HRESULT initPixelShader(ID3D11Device* device, std::wstring filePath, std::wstring fileName, ID3D11PixelShader** shader, ID3DBlob** blob);

	HRESULT initInputLayouts(ID3D11Device* device);
	HRESULT initVSDefaultInputLayout(ID3D11Device* device);
	HRESULT initVSSpriteInputLayout(ID3D11Device* device);
	HRESULT initVSHeightmapInputLayout(ID3D11Device* device);

	std::wstring decideFilePath();

	ShaderSet m_shaders[ShaderIds_CNT];
};

#endif //MANAGEMENT_SHADER_H