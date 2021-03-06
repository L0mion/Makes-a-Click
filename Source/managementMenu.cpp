#include "managementMenu.h"
#include "managementWrite.h"
#include "sprite.h"
#include "text.h"
#include "utility.h"
#include "XInputFetcher.h"

ManagementMenu::ManagementMenu(XInputFetcher* p_xinput)
{
	m_xinput = p_xinput;

	m_textColor				= DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_managementSprite		= NULL;
	m_managementWrite		= NULL;
	m_activeTool			= ToolIds_SAND;
	m_activeProperty		= ToolPropertyIds_PROPERTY_0;
	m_sandProperty			= ToolPropertyIds_PROPERTY_0;
	m_objectProperty		= ToolPropertyIds_PROPERTY_0;
	m_textureProperty		= ToolPropertyIds_PROPERTY_0;
	m_tempSelectedProperty  = ToolPropertyIds_NONE;
	m_tempSelectedTool		= ToolIds_NONE;
	m_textState				= TextStates_LOWER_CASE;

	m_cursorIndex		= 0;

	m_analogActive		= 0.5;
}
ManagementMenu::~ManagementMenu()
{
	SAFE_DELETE(m_managementSprite);
	SAFE_DELETE(m_managementWrite);
}

void ManagementMenu::useTextMenu()
{
	double analogX	= m_xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LX_POSITIVE);
	double analogY	= m_xinput->getCalibratedAnalog(InputHelper::Xbox360Analogs_THUMB_LY_NEGATIVE);

	m_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_TEXT_MENU);

	textMenuSwitchHighlighter(analogX, analogY);
	textMenuSwitchCase();
	textMenuMoveCursor();
	textMenuCalcCursorPos();
	textMenuInputText(analogX, analogY);

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
	else if(insideSector2(analogX, analogY))
		toolsMenuSector2();
	else
		toolsMenuSectorNone();

	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
		setSelectedTool();

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
	case ToolIds_TEXTURE_BRUSH:
		useTextureBrushPropertiesMenu(analogX, analogY);
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

	switch(m_activeTool)
	{
	case ToolIds_SAND:
		m_activeProperty = m_sandProperty;
		break;
	case ToolIds_OBJECT:
		m_activeProperty = m_objectProperty;
		break;
	case ToolIds_TEXTURE_BRUSH:
		m_activeProperty = m_textureProperty;
		break;
	default:
		m_activeProperty = ToolPropertyIds_PROPERTY_0;
		break;
	}

	setToolIcon();
}
void ManagementMenu::setSelectedProperty()
{
	if(m_tempSelectedProperty != ToolPropertyIds_NONE)
		m_activeProperty = m_tempSelectedProperty;

	switch(m_activeTool)
	{
	case ToolIds_SAND:
		m_sandProperty = m_activeProperty;
		break;
	case ToolIds_OBJECT:
		m_objectProperty = m_activeProperty;
		break;
	case ToolIds_TEXTURE_BRUSH:
		m_textureProperty = m_activeProperty;
		break;
	}

	setToolPropertyIcon();
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

	initManagementWrite(p_device, p_devcon);

	return hr;
}
HRESULT ManagementMenu::initManagementSprite(ID3D11Device* p_device)
{
	HRESULT hr = S_OK;
	m_managementSprite = new ManagementSprite();
	m_managementSprite->init(p_device);
	return hr;
}
void ManagementMenu::initManagementWrite(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon)
{
	m_managementWrite = new ManagementWrite();
	m_managementWrite->init(p_device, p_devcon);
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

void ManagementMenu::setToolIcon()
{
	if(m_activeTool == ToolIds_SAND)
	{
		Sprite* sprite = m_managementSprite->getSprite(ManagementSprite::SpriteIds_TOOL_ICON);
		sprite->setTextureId(TextureIds::TextureIds_SANB_BUCKET);
	}
	else if(m_activeTool == ToolIds_OBJECT)
	{
		Sprite* sprite = m_managementSprite->getSprite(ManagementSprite::SpriteIds_TOOL_ICON);
		sprite->setTextureId(TextureIds::TextureIds_OBJECT);
	}
	else if(m_activeTool == ToolIds_TEXTURE_BRUSH)
	{
		Sprite* sprite = m_managementSprite->getSprite(ManagementSprite::SpriteIds_TOOL_ICON);
		sprite->setTextureId(TextureIds::TextureIds_TEXTURE_BRUSH);
	}
	setToolPropertyIcon();
}
void ManagementMenu::setToolPropertyIcon()
{
	Sprite* sprite = m_managementSprite->getSprite(ManagementSprite::SpriteIds_TOOL_PROPERTY_ICON);
	if(m_activeTool == ToolIds_SAND)
	{
		if(m_activeProperty == ToolPropertyIds_PROPERTY_0)
			sprite->setTextureId(TextureIds::TextureIds_ADD_REMOVE);
		else if(m_activeProperty == ToolPropertyIds_PROPERTY_1)
			sprite->setTextureId(TextureIds::TextureIds_SMOOTH);
	}
	else if(m_activeTool == ToolIds_OBJECT)
	{
		if(m_activeProperty == ToolPropertyIds_PROPERTY_0)
			sprite->setTextureId(TextureIds::TextureIds_PREVIEW_SHRUBS);
		else if(m_activeProperty == ToolPropertyIds_PROPERTY_1)
			sprite->setTextureId(TextureIds::TextureIds_PREVIEW_HEMPS);
		else if(m_activeProperty == ToolPropertyIds_PROPERTY_2)
			sprite->setTextureId(TextureIds::TextureIds_PREVIEW_HESCO);
		else if(m_activeProperty == ToolPropertyIds_PROPERTY_3)
			sprite->setTextureId(TextureIds::TextureIds_PREVIEW_MONEY);
		else if(m_activeProperty == ToolPropertyIds_PROPERTY_4)
			sprite->setTextureId(TextureIds::TextureIds_PREVIEW_PALM);
		else if(m_activeProperty == ToolPropertyIds_PROPERTY_5)
			sprite->setTextureId(TextureIds::TextureIds_PREVIEW_PLASTIC_BARREL);
		else if(m_activeProperty == ToolPropertyIds_PROPERTY_6)
			sprite->setTextureId(TextureIds::TextureIds_PREVIEW_SHRUB1);
		else if(m_activeProperty == ToolPropertyIds_PROPERTY_7)
			sprite->setTextureId(TextureIds::TextureIds_PREVIEW_SHRUB2);
	}
	else if(m_activeTool == ToolIds_TEXTURE_BRUSH)
	{
		if(m_activeProperty == ToolPropertyIds_PROPERTY_0)
			sprite->setTextureId(TextureIds::TextureIds_STONE);
		else if(m_activeProperty == ToolPropertyIds_PROPERTY_1)
			sprite->setTextureId(TextureIds::TextureIds_GRASS);
		else if(m_activeProperty == ToolPropertyIds_PROPERTY_2)
			sprite->setTextureId(TextureIds::TextureIds_GRAVEL);
	}
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
void ManagementMenu::textMenuMoveCursor()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_DPAD_LEFT) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_cursorIndex > 0)
			m_cursorIndex--;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_DPAD_RIGHT) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_cursorIndex < (int)m_textMenuOut.size())
			m_cursorIndex++;
	}
}
void ManagementMenu::textMenuCalcCursorPos()
{
	Sprite* cursor = m_managementSprite->getSprite(ManagementSprite::SpriteIds_TEXT_CURSOR);

	std::wstring text;
	for(unsigned int i=0; i<(unsigned int)m_cursorIndex; i++)
	{
		text.push_back(m_textMenuOut.at(i));
	}

	int numTrailingSpaces = m_managementWrite->findNumTrailingSpaces(text);

	DirectX::XMFLOAT2 origin = calcTextMenuOutPos(m_textMenuOut);
	DirectX::XMFLOAT2 stringSize = m_managementWrite->measureString(text);
	float posX = origin.x+ stringSize.x + numTrailingSpaces * ManagementWrite::SPACE_WIDTH;
	posX = posX / SCREEN_WIDTH;
	posX = (posX-0.5f) * 2;
	float posY = cursor->getPosition().y;

	cursor->setPosition(posX, posY);
}
void ManagementMenu::textMenuInputText(double p_analogX, double p_analogY)
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_SHOULDER_PRESS_L) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textMenuOut.length() > 0)
		{
			m_textMenuOut.erase(m_cursorIndex-1, 1);
			m_cursorIndex--;
		}
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_SHOULDER_PRESS_R) == InputHelper::KeyStates_KEY_PRESSED)
	{
		m_textMenuOut.insert(m_cursorIndex, L" ");
		m_cursorIndex++;
	}
	
	int maxNumChars = 65;
	if((int)m_textMenuOut.size() < maxNumChars)
	{
		if(insideSector0(p_analogX, p_analogY))
			textMenuSector0();
		if(insideSector1(p_analogX, p_analogY))
			textMenuSector1();
		if(insideSector2(p_analogX, p_analogY))
			textMenuSector2();
		if(insideSector3(p_analogX, p_analogY))
			textMenuSector3();
		if(insideSector4(p_analogX, p_analogY))
			textMenuSector4();
		if(insideSector5(p_analogX, p_analogY))
			textMenuSector5();
		if(insideSector6(p_analogX, p_analogY))
			textMenuSector6();
		if(insideSector7(p_analogX, p_analogY))
			textMenuSector7();
		else
			textMenuSectorNone();
	}
}
void ManagementMenu::textMenuSector0()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"a");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"A");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"1");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"b");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"B");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"2");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"c");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"C");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"3");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"d");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"D");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"4");

		m_cursorIndex++;
	}
}
void ManagementMenu::textMenuSector1()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"e");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"E");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"5");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"f");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"F");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"6");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"g");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"G");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"7");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"h");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"H");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"8");

		m_cursorIndex++;
	}
}
void ManagementMenu::textMenuSector2()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"i");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"I");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"9");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"j");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"J");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"0");
		
		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"k");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"K");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"*");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"l");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"L");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"+");

		m_cursorIndex++;
	}
}
void ManagementMenu::textMenuSector3()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"m");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"M");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"-");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"n");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"N");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"@");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"o");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"O");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"$");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"p");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"P");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L",");

		m_cursorIndex++;
	}
}
void ManagementMenu::textMenuSector4()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"q");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"Q");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"\'");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"r");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"R");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"\"");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"s");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"S");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"~");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"t");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"T");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"|");

		m_cursorIndex++;
	}
}
void ManagementMenu::textMenuSector5()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"u");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"U");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"=");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"v");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"V");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"<");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"w");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"W");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L">");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"x");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"X");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"[");

		m_cursorIndex++;
	}
}
void ManagementMenu::textMenuSector6()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"y");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"Y");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"]");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"z");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"Z");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"{");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L",");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"?");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"}");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L".");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"!");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"(");

		m_cursorIndex++;
	}
}
void ManagementMenu::textMenuSector7()
{
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_X) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L":");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L";");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L")");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_Y) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"a");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"A");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"/");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_B) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"a");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"A");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"&");

		m_cursorIndex++;
	}
	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
	{
		if(m_textState == TextStates_LOWER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"o");
		if(m_textState == TextStates_UPPER_CASE)
			m_textMenuOut.insert(m_cursorIndex, L"O");
		if(m_textState == TextStates_NUMBERS)
			m_textMenuOut.insert(m_cursorIndex, L"%");

		m_cursorIndex++;
	}
}
void ManagementMenu::textMenuSectorNone()
{
}

void ManagementMenu::toolsMenuSector0()
{
	m_tempSelectedTool = ToolIds_SAND;
	//m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;
	
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
	//m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;
	
	std::wstring text = L"Object Tool";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::toolsMenuSector2()
{
	m_tempSelectedTool = ToolIds_TEXTURE_BRUSH;
	//m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;
	
	std::wstring text = L"Texture Brush";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
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

	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
		setSelectedProperty();

	moveSpriteToSector(p_analogStickX, p_analogStickY, ManagementSprite::SpriteIds_CIRCLE_HIGHLIGHT);
}
void ManagementMenu::sandPropertiesSector0()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;

	std::wstring text = L"Add/Remove";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::sandPropertiesSector1()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_1;

	std::wstring text = L"Smooth";
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
	m_tempSelectedTool = ToolIds_NONE;
	m_tempSelectedProperty = ToolPropertyIds_NONE;
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
	if(insideSector4(p_analogStickX, p_analogStickY))
		objectPropertiesSector4();
	if(insideSector5(p_analogStickX, p_analogStickY))
		objectPropertiesSector5();
	if(insideSector6(p_analogStickX, p_analogStickY))
		objectPropertiesSector6();
	if(insideSector7(p_analogStickX, p_analogStickY))
		objectPropertiesSector7();

	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
		setSelectedProperty();

	moveSpriteToSector(p_analogStickX, p_analogStickY, ManagementSprite::SpriteIds_CIRCLE_HIGHLIGHT);
}
void ManagementMenu::objectPropertiesSector0()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;

	std::wstring text = L"Flower";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSector1()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_1;

	std::wstring text = L"Hemp";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSector2()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_2;

	std::wstring text = L"Hesco Box";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSector3()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_3;

	std::wstring text = L"Money";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSector4()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_4;

	std::wstring text = L"Palm";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSector5()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_5;

	std::wstring text = L"Plastic Barrel";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSector6()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_6;

	std::wstring text = L"Shrub 1";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSector7()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_7;

	std::wstring text = L"Shrub 2";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::objectPropertiesSectorNone()
{
	m_tempSelectedTool = ToolIds_NONE;
	m_tempSelectedProperty = ToolPropertyIds_NONE;
}

void ManagementMenu::useTextureBrushPropertiesMenu(double p_analogStickX, double p_analogStickY)
{
	m_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_TEXTURE_BRUSH_PROPERTIES_MENU);

	if(insideSector0(p_analogStickX, p_analogStickY))
		textureBrushPropertiesSector0();
	if(insideSector1(p_analogStickX, p_analogStickY))
		textureBrushPropertiesSector1();
	if(insideSector2(p_analogStickX, p_analogStickY))
		textureBrushPropertiesSector2();

	if(m_xinput->getBtnState(InputHelper::Xbox360Digitals_BTN_A) == InputHelper::KeyStates_KEY_PRESSED)
		setSelectedProperty();

	moveSpriteToSector(p_analogStickX, p_analogStickY, ManagementSprite::SpriteIds_CIRCLE_HIGHLIGHT);
}
void ManagementMenu::textureBrushPropertiesSector0()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;

	std::wstring text = L"Stone";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::textureBrushPropertiesSector1()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_1;

	std::wstring text = L"Grass";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::textureBrushPropertiesSector2()
{
	m_tempSelectedProperty = ToolPropertyIds_PROPERTY_2;

	std::wstring text = L"Gravel";
	DirectX::XMFLOAT2 textPos = calcTextPosForCenter(text);

	m_textStrings.push_back(
		new Text(text,
		textPos,
		m_textColor));
}
void ManagementMenu::textureBrushPropertiesSector3()
{
}
void ManagementMenu::textureBrushPropertiesSector4()
{
}
void ManagementMenu::textureBrushPropertiesSector5()
{
}
void ManagementMenu::textureBrushPropertiesSector6()
{
}
void ManagementMenu::textureBrushPropertiesSector7()
{
}
void ManagementMenu::textureBrushPropertiesSectorNone()
{
}

DirectX::XMFLOAT2 ManagementMenu::calcTextPosForCenter(std::wstring p_text)
{
	DirectX::XMFLOAT2 pos = DirectX::XMFLOAT2(SCREEN_WIDTH/2.0f, SCREEN_HEIGHT/2.0f);
	DirectX::XMFLOAT2 stringSize = m_managementWrite->measureString(p_text);

	pos.x -= stringSize.x/2.0f;
	pos.y -= stringSize.y/2.0f;

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

	DirectX::XMFLOAT2 stringSize = m_managementWrite->measureString(p_text);

	int numTrailingSpaces = m_managementWrite->findNumTrailingSpaces(p_text);
	stringSize.x += numTrailingSpaces * ManagementWrite::SPACE_WIDTH; 

	pos.x = pos.x * SCREEN_WIDTH - stringSize.x/2.0f;
	pos.y = pos.y * SCREEN_HEIGHT - stringSize.y/2.0f;

	return pos;
}