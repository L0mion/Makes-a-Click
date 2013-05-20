#include "managementMenu.h"
#include "managementWrite.h"
#include "sprite.h"
#include "text.h"
#include "utility.h"
#include "XInputFetcher.h"

ManagementMenu::ManagementMenu(XInputFetcher* p_xinput)
{
	m_xinput = p_xinput;

	m_textColor			= DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_managementSprite	= NULL;
	m_activeTool		= ToolIds_SAND;
	m_tempSelectedTool	= ToolIds_NONE;
	m_textState			= TextStates_LOWER_CASE;
	m_analogActive		= 0.5;
}
ManagementMenu::~ManagementMenu()
{
	SAFE_DELETE(m_managementSprite);
}

void ManagementMenu::useTextMenu()
{
	double analogX	= m_xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LX_POSITIVE);
	double analogY	= m_xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE);

	m_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_TEXT_MENU);

	textMenuSwitchHighlighter(analogX, analogY);
	textMenuSwitchCase();

	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_SHOULDER_PRESS_L) == InputHelper::KeyStates_KEY_PRESSED)
		m_textMenuOut.pop_back();
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_SHOULDER_PRESS_R) == InputHelper::KeyStates_KEY_PRESSED)
		m_textMenuOut.push_back(' ');

	if(insideSector0(analogX, analogY))
		textMenuSector0();
	if(insideSector1(analogX, analogY))
		textMenuSector1();
	if(insideSector2(analogX, analogY))
		textMenuSector2();
	if(insideSector3(analogX, analogY))
		textMenuSector3();
	if(insideSector4(analogX, analogY))
		textMenuSector4();
	if(insideSector5(analogX, analogY))
		textMenuSector5();
	if(insideSector6(analogX, analogY))
		textMenuSector6();
	if(insideSector7(analogX, analogY))
		textMenuSector7();
	else
		textMenuSectorNone();

	DirectX::XMFLOAT2 pos = calcTextMenuOutPos(m_textMenuOut);
	m_textStrings.push_back(
		new Text(m_textMenuOut,
		pos,
		m_textColor));

	moveSpriteToSector(analogX, analogY, ManagementSprite::SpriteIds_TEXT_HIGHLIGHTER);
}
void ManagementMenu::useToolsMenu()
{
	double analogX	= m_xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE);
	double analogY	= m_xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE);

	m_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_TOOLS_MENU);

	if(insideSector0(analogX, analogY))
		toolsMenuSector0();
	else if(insideSector1(analogX, analogY))
		toolsMenuSector1();
	else
		toolsMenuSectorNone();

	moveSpriteToSector(analogX, analogY, ManagementSprite::SpriteIds_CIRCLE_HIGHLIGHT);
}
void ManagementMenu::useToolPropertiesMenu()
{
	double analogX	= m_xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LX_NEGATIVE);
	double analogY	= m_xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE);

	switch(m_activeTool)
	{
	case ToolIds_SAND:
		useSandPropertiesMenu(analogX, analogY);
		break;
	case ToolIds_OBJECT:
		useObjectPropertiesMenu(analogX, analogY);
		break;
	}
}
void ManagementMenu::useNoMenu()
{
	m_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_NONE);
}

void ManagementMenu::setSelectedTool()
{
	if(m_tempSelectedTool != ToolIds_NONE)
		m_activeTool = m_tempSelectedTool;
}
void ManagementMenu::setSelectedProperty()
{
	if(m_tempSelectedProperty != ToolPropertyIds_NONE)
		m_activeProperty = m_tempSelectedProperty;
}

ManagementMenu::ToolIds			ManagementMenu::getActiveTool()
{
	return m_activeTool;
}
ManagementMenu::ToolPropertyIds ManagementMenu::getActiveProperty()
{
	return m_activeProperty;
}

ManagementSprite*	ManagementMenu::getManagementSprite()
{
	return m_managementSprite;
}
std::vector<Text*>	ManagementMenu::getTextStrings()
{
	return m_textStrings;
}

void ManagementMenu::clearTextStrings()
{
	for(unsigned int i=0; i<m_textStrings.size(); i++)
		SAFE_DELETE(m_textStrings[i]);
	m_textStrings.clear();
}

HRESULT ManagementMenu::init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon)
{
	HRESULT hr = S_OK;
	hr = initManagementSprite(p_device);
	return hr;
}
HRESULT ManagementMenu::initManagementSprite(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;
	m_managementSprite = new ManagementSprite();
	m_managementSprite->init(p_device);
	return hr;
}

void ManagementMenu::moveSpriteToSector(double p_analogStickX, double p_analogStickY, ManagementSprite::SpriteIds p_spriteId)
{
	if(insideSector0(p_analogStickX, p_analogStickY))	
		setSpriteSector(ManagementSprite::SectorIds_SECTOR_0, p_spriteId);

	else if(insideSector1(p_analogStickX, p_analogStickY))
		setSpriteSector(ManagementSprite::SectorIds_SECTOR_1, p_spriteId);
	
	else if(insideSector2(p_analogStickX, p_analogStickY))
		setSpriteSector(ManagementSprite::SectorIds_SECTOR_2, p_spriteId);
	
	else if(insideSector3(p_analogStickX, p_analogStickY))
		setSpriteSector(ManagementSprite::SectorIds_SECTOR_3, p_spriteId);

	else if(insideSector4(p_analogStickX, p_analogStickY))
		setSpriteSector(ManagementSprite::SectorIds_SECTOR_4, p_spriteId);

	else if(insideSector5(p_analogStickX, p_analogStickY))
		setSpriteSector(ManagementSprite::SectorIds_SECTOR_5, p_spriteId);

	else if(insideSector6(p_analogStickX, p_analogStickY))
		setSpriteSector(ManagementSprite::SectorIds_SECTOR_6, p_spriteId);

	else if(insideSector7(p_analogStickX, p_analogStickY))
		setSpriteSector(ManagementSprite::SectorIds_SECTOR_7, p_spriteId);
		
	else
		setSpriteSector(ManagementSprite::SectorIds_CENTER, p_spriteId);
}
void ManagementMenu::setSpriteSector(ManagementSprite::SectorIds sectorId, ManagementSprite::SpriteIds p_spriteId)
{
	DirectX::XMFLOAT2 pos = m_managementSprite->getSectorCoords(sectorId);
	Sprite* highlighter = m_managementSprite->getSprite(p_spriteId);
	highlighter->setPosition(pos.x, pos.y);
}

bool ManagementMenu::insideSector0(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if((p_analogX < m_analogActive && p_analogX > -m_analogActive) && p_analogY > m_analogActive)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector1(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX > m_analogActive && p_analogY > m_analogActive)
		isInside = true;
	
	return isInside;
}
bool ManagementMenu::insideSector2(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX > m_analogActive && (p_analogY > -m_analogActive && p_analogY < m_analogActive))
		isInside = true;
	
	return isInside;
}
bool ManagementMenu::insideSector3(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX > m_analogActive && p_analogY < -m_analogActive)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector4(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if((p_analogX < m_analogActive && p_analogX > -m_analogActive) && p_analogY < -m_analogActive)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector5(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX < -m_analogActive && p_analogY < -m_analogActive)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector6(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX < -m_analogActive &&( p_analogY < m_analogActive && p_analogY > -m_analogActive))
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector7(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX < -m_analogActive && p_analogY > m_analogActive)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSectorNone(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX > -m_analogActive && p_analogX < m_analogActive &&
	   p_analogY > -m_analogActive && p_analogY < m_analogActive)
	{
		isInside = true;
	}
	
	return isInside;
}

void ManagementMenu::textMenuSwitchHighlighter(double p_analogX, double p_analogY)
{
	if(insideSectorNone(p_analogX, p_analogY))
	{
		Sprite* textHighlighter = m_managementSprite->getSprite(ManagementSprite::SpriteIds_TEXT_HIGHLIGHTER);
		textHighlighter->setTextureId(TextureIds::TextureIds_TEXT_HIGHLIGHTER);
	}
	else
	{
		Sprite* textHighlighter = m_managementSprite->getSprite(ManagementSprite::SpriteIds_TEXT_HIGHLIGHTER);
		textHighlighter->setTextureId(TextureIds::TextureIds_TEXT_HIGHLIGHTER_BUTTONS);
	}
}
void ManagementMenu::textMenuSwitchCase()
{
	if(m_xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_TRIGGER_L) > m_analogActive)
	{
		Sprite* text = m_managementSprite->getSprite(ManagementSprite::SpriteIds_TEXT);
		text->setTextureId(TextureIds::TextureIds_TEXT_CAPS);
		m_textState = TextStates_UPPER_CASE;
	}
	else if(m_xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_TRIGGER_R) > m_analogActive)
	{
		Sprite* text = m_managementSprite->getSprite(ManagementSprite::SpriteIds_TEXT);
		text->setTextureId(TextureIds::TextureIds_TEXT_NUMBERS);
		m_textState = TextStates_NUMBERS;
	}
	else
	{
		Sprite* text = m_managementSprite->getSprite(ManagementSprite::SpriteIds_TEXT);
		text->setTextureId(TextureIds::TextureIds_TEXT);
		m_textState = TextStates_LOWER_CASE;
	}
}
void ManagementMenu::textMenuSector0()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('a');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('A');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('1');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('b');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('B');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('2');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('c');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('C');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('3');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('d');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('D');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('4');
	}
}
void ManagementMenu::textMenuSector1()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('e');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('E');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('5');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('f');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('F');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('6');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('g');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('G');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('7');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('h');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('H');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('8');
	}
}
void ManagementMenu::textMenuSector2()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('i');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('I');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('9');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('j');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('J');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('0');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('k');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('K');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('*');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('l');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('L');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('+');
	}
}
void ManagementMenu::textMenuSector3()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('m');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('M');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('-');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('n');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('N');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('@');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('o');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('O');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('$');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('p');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('P');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back(',');
	}
}
void ManagementMenu::textMenuSector4()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('q');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('Q');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('\'');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('r');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('R');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('\"');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('s');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('S');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('~');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('t');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('T');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('|');
	}
}
void ManagementMenu::textMenuSector5()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('u');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('U');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('=');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('v');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('V');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('<');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('w');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('W');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('>');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('x');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('X');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('[');
	}
}
void ManagementMenu::textMenuSector6()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('y');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('Y');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back(']');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('z');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('Z');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('{');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('.');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('?');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('}');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back(',');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('!');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('(');
	}
}
void ManagementMenu::textMenuSector7()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back(':');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back(';');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back(')');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('a');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('A');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('/');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('a');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('A');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('&');
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.push_back('o');
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.push_back('O');
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.push_back('%');
	}
}
void ManagementMenu::textMenuSectorNone()
{
}

void ManagementMenu::toolsMenuSector0()
{
	m_tempSelectedTool = ToolIds_SAND;
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;
	
	std::wstring text = L"Sand Tool";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::toolsMenuSector1()
{
	m_tempSelectedTool = ToolIds_OBJECT;
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;
	
	std::wstring text = L"Object Tool";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::toolsMenuSector2()
{
}
void ManagementMenu::toolsMenuSector3()
{
}
void ManagementMenu::toolsMenuSector4()
{
}
void ManagementMenu::toolsMenuSector5()
{
}
void ManagementMenu::toolsMenuSector6()
{
}
void ManagementMenu::toolsMenuSector7()
{
}
void ManagementMenu::toolsMenuSectorNone()
{
	m_tempSelectedTool = ToolIds_NONE;
	m_tempSelectedProperty = ToolPropertyIds_NONE;
}

void ManagementMenu::useSandPropertiesMenu(double p_analogStickX, double p_analogStickY)
{
	m_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_SAND_PROPERTIES_MENU);

	if(insideSector0(p_analogStickX, p_analogStickY))
		sandPropertiesSector0();
	if(insideSector1(p_analogStickX, p_analogStickY))
		sandPropertiesSector1();
	if(insideSector2(p_analogStickX, p_analogStickY))
		sandPropertiesSector2();
	if(insideSector3(p_analogStickX, p_analogStickY))
		sandPropertiesSector3();

	moveSpriteToSector(p_analogStickX, p_analogStickY, ManagementSprite::SpriteIds_CIRCLE_HIGHLIGHT);
}
void ManagementMenu::sandPropertiesSector0()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;

	std::wstring text = L"Sand Property 0";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::sandPropertiesSector1()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_1;

	std::wstring text = L"Sand Property 1";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::sandPropertiesSector2()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_2;

	std::wstring text = L"Sand Property 2";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::sandPropertiesSector3()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_3;

	std::wstring text = L"Sand Property 3";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::sandPropertiesSector4()
{
}
void ManagementMenu::sandPropertiesSector5()
{
}
void ManagementMenu::sandPropertiesSector6()
{
}
void ManagementMenu::sandPropertiesSector7()
{
}
void ManagementMenu::sandPropertiesSectorNone()
{
}

void ManagementMenu::useObjectPropertiesMenu(double p_analogStickX, double p_analogStickY)
{
	m_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_OBJECT_PROPERTIES_MENU);

	if(insideSector0(p_analogStickX, p_analogStickY))
		objectPropertiesSector0();
	if(insideSector1(p_analogStickX, p_analogStickY))
		objectPropertiesSector1();
	if(insideSector2(p_analogStickX, p_analogStickY))
		objectPropertiesSector2();
	if(insideSector3(p_analogStickX, p_analogStickY))
		objectPropertiesSector3();

	moveSpriteToSector(p_analogStickX, p_analogStickY, ManagementSprite::SpriteIds_CIRCLE_HIGHLIGHT);
}
void ManagementMenu::objectPropertiesSector0()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;

	std::wstring text = L"Object Property 0";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSector1()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_1;

	std::wstring text = L"Object Property 1";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSector2()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_2;

	std::wstring text = L"Object Property 2";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSector3()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_3;

	std::wstring text = L"Object Property 3";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSector4()
{
}
void ManagementMenu::objectPropertiesSector5()
{
}
void ManagementMenu::objectPropertiesSector6()
{
}
void ManagementMenu::objectPropertiesSector7()
{
}
void ManagementMenu::objectPropertiesSectorNone()
{
}

DirectX::XMFLOAT2 ManagementMenu::calcTextPosForCenter(std::wstring p_text)
{
	DirectX::XMFLOAT2 pos = DirectX::XMFLOAT2(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f);
	int charWidth		= 5;
	int halfCharHeight	= 8;

	pos.x -= p_text.length() * charWidth;
	pos.y -= halfCharHeight;

	return pos;
}
DirectX::XMFLOAT2 ManagementMenu::calcTextMenuOutPos(std::wstring p_text)
{
	Sprite* outBackground = m_managementSprite->getSprite(ManagementSprite::SpriteIds_TEXT_OUTPUT_BACKGROUND);
	DirectX::XMFLOAT2 pos;
	pos.x = outBackground->getPosition().x;
	pos.y = outBackground->getPosition().y * -1;

	//Transform from [-1 .. 1] to [0 .. 1]
	pos.x = (pos.x + 1.0f) /2.0f;
	pos.y = (pos.y + 1.0f) /2.0f;

	float charWidth = 4.45f;
	float charHalfHeight = 8.0f;

	pos.x = pos.x * SCREEN_WIDTH - p_text.length() * charWidth;
	pos.y = pos.y * SCREEN_HEIGHT - charHalfHeight;

	return pos;
}