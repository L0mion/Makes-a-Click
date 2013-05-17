#ifndef TEXT_H
#define TEXT_H

#include <string>
#include <DirectXMath.h>

struct Text
{
	std::wstring		m_text;
	DirectX::XMFLOAT2	m_position;
	DirectX::XMFLOAT4	m_color;

	Text()
	{
		m_text		= L"Unknown";
		m_position	= DirectX::XMFLOAT2(0.0f, 0.0f);
		m_color		= DirectX::XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
	}
	Text(std::wstring p_text, DirectX::XMFLOAT2 p_position, DirectX::XMFLOAT4 p_color)
	{
		m_text		= p_text;
		m_position	= p_position;
		m_color		= p_color;
	}
};

#endif //TEXT_H