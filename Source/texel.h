#ifndef TEXEL_H
#define TEXEL_H

struct Texel
{
	byte m_red;
	byte m_green;
	byte m_blue;
	byte m_alpha;

	Texel()
	{
		m_red	= 0;
		m_green = 0;
		m_blue	= 0;
		m_alpha = 0;
	}
	Texel(byte p_red, byte p_green, byte p_blue, byte p_alpha)
	{
		m_red	= p_red;
		m_green = p_green;
		m_blue	= p_blue;
		m_alpha = p_alpha;
	}
};

#endif // TEXEL_H