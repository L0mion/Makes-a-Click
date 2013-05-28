#ifndef TEXEL_H
#define TEXEL_H

struct Texel
{
	float m_red;
	float m_green;
	float m_blue;
	float m_alpha;

	Texel()
	{
		m_red	= 0;
		m_green = 0;
		m_blue	= 0;
		m_alpha = 0;
	}
	Texel(float p_red, float p_green, float p_blue, float p_alpha)
	{
		m_red	= p_red;
		m_green = p_green;
		m_blue	= p_blue;
		m_alpha = p_alpha;
	}
};

#endif // TEXEL_H