#ifndef MANAGEMENT_SHADER_H
#define MANAGEMENT_SHADER_H

#include <d3d11.h>
#include <d3dcompiler.h>
#include <string>

enum ShaderId
{
	SHADER_ID_VS_DEFAULT,
	SHADER_ID_PS_DEFAULT
};

enum InputLayoutId
{
	INPUT_LAYOUT_ID_VS_DEFAULT
};

class ManagementShader
{
public:
	ManagementShader();
	~ManagementShader();

	void setShader(ID3D11DeviceContext* devcon, ShaderId shaderId);
	void setInputLayout(ID3D11DeviceContext* devcon, InputLayoutId inputLayoutId);

	HRESULT init(ID3D11Device* device);
private:
	HRESULT initShaders(ID3D11Device* device);
	HRESULT initVertexShader(ID3D11Device* device, std::wstring filePath, std::wstring fileName, ID3D11VertexShader** shader, ID3DBlob** blob);
	HRESULT initPixelShader(ID3D11Device* device, std::wstring filePath, std::wstring fileName, ID3D11PixelShader** shader, ID3DBlob** blob);

	HRESULT initInputLayouts(ID3D11Device* device);
	HRESULT initVSDefaultInputLayout(ID3D11Device* device);

	std::wstring decideFilePath();

	ID3D11VertexShader* vsDefault_;
	ID3D11PixelShader* psDefault_;

	ID3DBlob* vsDefaultBlob_;
	ID3DBlob* psDefaultBlob_;

	ID3D11InputLayout* vsDefaultIL_;
};

#endif //MANAGEMENT_SHADER_H