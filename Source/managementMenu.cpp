#include "managementMenu.h"
#include "managementWrite.h"
#include "sprite.h"
#include "utility.h"

ManagementMenu::ManagementMenu()
{
	m_managementSprite = NULL;
	m_managementWrite = NULL;

	m_activeTool		= ToolIds_SAND;
	m_tempSelectedTool	= ToolIds_NONE;
}
ManagementMenu::~ManagementMenu()
{
	SAFE_DELETE(m_managementSprite);
	SAFE_DELETE(m_managementWrite);
}

void ManagementMenu::useToolsMenu(double p_analogStickX, double p_analogStickY)
{
	m_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_TOOLS_MENU);

	m_managementWrite->renderText();

	if(insideSector0(p_analogStickX, p_analogStickY))
	{
		m_tempSelectedTool = ToolIds_SAND;
		m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;
	}
	else if(insideSector1(p_analogStickX, p_analogStickY))
	{
		m_tempSelectedTool = ToolIds_OBJECT;
		m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;
	}
	else
	{
		m_tempSelectedTool = ToolIds_NONE;
		m_tempSelectedProperty = ToolPropertyIds_NONE;
	}

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

ManagementMenu::ToolIds ManagementMenu::getActiveTool()
{
	return m_activeTool;
}
ManagementMenu::ToolPropertyIds ManagementMenu::getActiveProperty()
{
	return m_activeProperty;
}

ManagementSprite* ManagementMenu::getManagementSprite()
{
	return m_managementSprite;
}

ManagementWrite* ManagementMenu::getManagementWrite()
{
	return m_managementWrite;
}

HRESULT ManagementMenu::init(ID3D11Device* p_device, ID3D11DeviceContext* p_devcon)
{
	HRESULT hr = S_OK;

	hr = initManagementSprite(p_device);
	if(SUCCEEDED(hr))
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

void ManagementMenu::useSandPropertiesMenu(double p_analogStickX, double p_analogStickY)
{
	m_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_SAND_PROPERTIES_MENU);

	if(insideSector0(p_analogStickX, p_analogStickY))
		m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;
	if(insideSector0(p_analogStickX, p_analogStickY))
		m_tempSelectedProperty = ToolPropertyIds_PROPERTY_1;
	if(insideSector0(p_analogStickX, p_analogStickY))
		m_tempSelectedProperty = ToolPropertyIds_PROPERTY_2;
	if(insideSector0(p_analogStickX, p_analogStickY))
		m_tempSelectedProperty = ToolPropertyIds_PROPERTY_3;

	moveHighlighter(p_analogStickX, p_analogStickY);
}
void ManagementMenu::useObjectPropertiesMenu(double p_analogStickX, double p_analogStickY)
{
	m_managementSprite->setSpriteCollection(ManagementSprite::SpriteCollectionIds_OBJECT_PROPERTIES_MENU);

	if(insideSector0(p_analogStickX, p_analogStickY))
		m_tempSelectedProperty = ToolPropertyIds_PROPERTY_0;
	if(insideSector0(p_analogStickX, p_analogStickY))
		m_tempSelectedProperty = ToolPropertyIds_PROPERTY_1;
	if(insideSector0(p_analogStickX, p_analogStickY))
		m_tempSelectedProperty = ToolPropertyIds_PROPERTY_2;
	if(insideSector0(p_analogStickX, p_analogStickY))
		m_tempSelectedProperty = ToolPropertyIds_PROPERTY_3;

	moveHighlighter(p_analogStickX, p_analogStickY);
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