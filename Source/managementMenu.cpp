#include "managementMenu.h"
#include "managementWrite.h"
#include "sprite.h"
#include "text.h"
#include "utility.h"

ManagementMenu::ManagementMenu()
{
	m_textColor			= DirectX::XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_managementSprite	= NULL;
	m_activeTool		= ToolIds_SAND;
	m_tempSelectedTool	= ToolIds_NONE;
}
ManagementMenu::~ManagementMenu()
{
	SAFE_DELETE(m_managementSprite);
}

void ManagementMenu::useToolsMenu(double p_analogStickX, double p_analogStickY)
{
	m_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_TOOLS_MENU);

	if(insideSector0(p_analogStickX, p_analogStickY))
		toolsMenuSector0();
	else if(insideSector1(p_analogStickX, p_analogStickY))
		toolsMenuSector1();
	else
		toolsMenuSectorNone();

	moveHighlighter(p_analogStickX, p_analogStickY);
}
void ManagementMenu::useToolPropertiesMenu(double p_analogStickX, double p_analogStickY)
{
	switch(m_activeTool)
	{
	case ToolIds_SAND:
		useSandPropertiesMenu(p_analogStickX, p_analogStickY);
		break;
	case ToolIds_OBJECT:
		useObjectPropertiesMenu(p_analogStickX, p_analogStickY);
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

void ManagementMenu::moveHighlighter(double p_analogStickX, double p_analogStickY)
{
	if(insideSector0(p_analogStickX, p_analogStickY))	
		setHighlighterPos(ManagementSprite::SectorIds_SECTOR_0);

	else if(insideSector1(p_analogStickX, p_analogStickY))
		setHighlighterPos(ManagementSprite::SectorIds_SECTOR_1);
	
	else if(insideSector2(p_analogStickX, p_analogStickY))
		setHighlighterPos(ManagementSprite::SectorIds_SECTOR_2);
	
	else if(insideSector3(p_analogStickX, p_analogStickY))
		setHighlighterPos(ManagementSprite::SectorIds_SECTOR_3);

	else if(insideSector4(p_analogStickX, p_analogStickY))
		setHighlighterPos(ManagementSprite::SectorIds_SECTOR_4);

	else if(insideSector5(p_analogStickX, p_analogStickY))
		setHighlighterPos(ManagementSprite::SectorIds_SECTOR_5);

	else if(insideSector6(p_analogStickX, p_analogStickY))
		setHighlighterPos(ManagementSprite::SectorIds_SECTOR_6);

	else if(insideSector7(p_analogStickX, p_analogStickY))
		setHighlighterPos(ManagementSprite::SectorIds_SECTOR_7);
		
	else
		setHighlighterPos(ManagementSprite::SectorIds_CENTER);
}
void ManagementMenu::setHighlighterPos(ManagementSprite::SectorIds sectorId)
{
	DirectX::XMFLOAT2 pos = m_managementSprite->getSectorCoords(sectorId);
	Sprite* highlighter = m_managementSprite->getSprite(ManagementSprite::SpriteIds_CIRCLE_HIGHLIGHT);
	highlighter->setPosition(pos.x, pos.y);
}

bool ManagementMenu::insideSector0(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if((p_analogX < 0.5 && p_analogX > -0.5) && p_analogY > 0.5f)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector1(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX > 0.5f && p_analogY > 0.5f)
		isInside = true;
	
	return isInside;
}
bool ManagementMenu::insideSector2(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX > 0.5f && (p_analogY > -0.5f && p_analogY < 0.5f))
		isInside = true;
	
	return isInside;
}
bool ManagementMenu::insideSector3(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX > 0.5f && p_analogY < -0.5f)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector4(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if((p_analogX < 0.5f && p_analogX > -0.5f) && p_analogY < -0.5f)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector5(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX < -0.5f && p_analogY < -0.5f)
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector6(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX < -0.5f &&( p_analogY < 0.5f && p_analogY > -0.5f))
		isInside = true;

	return isInside;
}
bool ManagementMenu::insideSector7(double p_analogX, double p_analogY)
{
	bool isInside = false;
	if(p_analogX < -0.5f && p_analogY > 0.5f)
		isInside = true;

	return isInside;
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

	moveHighlighter(p_analogStickX, p_analogStickY);
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

	moveHighlighter(p_analogStickX, p_analogStickY);
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