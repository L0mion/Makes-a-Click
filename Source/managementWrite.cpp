#include "managementWrite.h"

#include "text.h"
#include "utility.h"

const float ManagementWrite::SPACE_WIDTH = 5.5f;

ManagementWrite::ManagementWrite()
{
	m_spriteBatch = NULL;
	m_spriteFont = NULL;
}
ManagementWrite::~ManagementWrite()
{
	SAFE_DELETE(m_spriteBatch);
	SAFE_DELETE(m_spriteFont);
}

DirectX::XMFLOAT2 ManagementWrite::measureString(std::wstring p_text)
{
	DirectX::XMVECTOR length = m_spriteFont->MeasureString(p_text.c_str());
	DirectX::XMFLOAT2 result;
	DirectX::XMStoreFloat2(&result, length);

	return result;
}

int ManagementWrite::findNumTrailingSpaces(std::wstring p_text)
{
	int numTrailingSpaces = 0;
	if(p_text.size() > 0)
	{
		for(unsigned int i = p_text.size()-1; i>=0; i--)
		{
			if(p_text.at(i) == ' ')
				numTrailingSpaces++;
			else
				break;
		}
	}
	return numTrailingSpaces;
}

void ManagementWrite::renderText(Text* p_text)
{
	m_spriteBatch->Begin();

	DirectX::FXMVECTOR xmColor = DirectX::XMLoadFloat4(&p_text->m_color);
	m_spriteFont->DrawString(m_spriteBatch, p_text->m_text.c_str(), p_text->m_position, xmColor);

	m_spriteBatch->End();
}

void ManagementWrite::init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon)
{
	m_spriteBatch = new DirectX::SpriteBatch( p_devcon );
	m_spriteFont = new DirectX::SpriteFont( p_device, L"../../Resources/Fonts/Calibri.spritefont" );
}