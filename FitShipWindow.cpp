#include "FitShipWindow.h"
#include "LanguageManager.h"
#include "util.h"

game::FitShipWindow* game::FitShipWindow::_window = nullptr;

game::FitShipPanel * game::FitShipPanel::create(bool isEquipped, Data * weaponData)
{
	//载入背景
	FitShipPanel *panel = new (std::nothrow) FitShipPanel;
	if (panel && panel->initWithFile("UI//UI-station-ship-weaponPanel.png"))
	{
		panel->autorelease();
	}
	else {
		CC_SAFE_DELETE(panel);
		return nullptr;
	}
	panel->setAnchorPoint(cocos2d::Vec2::ZERO);
	panel->weaponData = weaponData;
	//加载DataLoader
	auto pGameDL = DataLoader::getDataLoader(DataLoaderType::gameDL);
	auto pSaveDL = DataLoader::getDataLoader(DataLoaderType::saveDL);
	//加载language
	auto lang = LanguageManager::getInstance();
	//加载最左侧的图标
	std::string weaponType = weaponData->findPairByKey("weaponType")->value;
	auto weaponIcon = cocos2d::Sprite::create(
		pGameDL->findDataByLabel(weaponType)->findPairByKey("texturePath")->value
	);
	weaponIcon->setRotation(90.0f);
	weaponIcon->setPosition(cocos2d::Vec2(50,41));
	panel->addChild(weaponIcon);
	//从weaponData中得到修正后的数据
	auto attrs = util::getFixedAttr(weaponData);
	//将数据加载到标签上
	std::string str = lang->getStringForKey(weaponType) + "\n";
	for (auto j = attrs.begin(); j != attrs.end(); j++) {
		str += j->attrStr + ":" + util::shortenNum(j->attrValue) + " ";
	}
	auto leftLabel = cocos2d::Label::create(str, "fonts//msyh.ttf", 20);
	leftLabel->setAnchorPoint(cocos2d::Vec2(0, 0.5));
	leftLabel->setPosition(160, 40);
	panel->addChild(leftLabel);
	//加载右上角的状态标签
	panel->isEquipped = isEquipped;
	if (isEquipped == true) panel->status = cocos2d::Label::create(lang->getStringForKey("statusEquipped"), "fonts//msyh.ttf", 20);
	else panel->status = cocos2d::Label::create(lang->getStringForKey("statusUnEquipped"), "fonts//msyh.ttf", 20);
	panel->status->setAnchorPoint(cocos2d::Vec2::ZERO);
	panel->status->setPosition(368, 50);
	panel->addChild(panel->status);
	//加载右上角的按钮
	cocos2d::ui::Button* button = cocos2d::ui::Button::create();
	panel->button = button;
	if (isEquipped == true) button->setTitleText(lang->getStringForKey("unEquipButton"));
	else button->setTitleText(lang->getStringForKey("equipButton"));
	button->setTitleColor(cocos2d::Color3B(14, 184, 255));
	button->setTitleFontName("fonts//msyh.ttf");
	button->setTitleFontSize(24);
	button->setAnchorPoint(cocos2d::Vec2(0.5,0));
	button->setPosition(cocos2d::Vec2(502,50));
	//设置按钮的回调函数
	button->addClickEventListener(CC_CALLBACK_0(FitShipPanel::buttonClickCallback, panel));
	panel->addChild(button);
	return panel;
}

void game::FitShipPanel::buttonClickCallback()
{
	if (this->isEquipped == true) { //点击卸下按钮
		//改写状态
		cocos2d::log("INFO: FitShipPanel:buttonClickCallback(), unEquip");
		this->isEquipped = false;
		this->button->setTitleText(LanguageManager::getInstance()->getStringForKey("equipButton"));
		this->status->setString(LanguageManager::getInstance()->getStringForKey("statusUnEquipped"));
		//卸下
		FitShipWindow::getInstance()->unEquipWeapon(this, this->weaponData);
	}
	else { //点击装备按钮
		cocos2d::log("INFO: FitShipPanel:buttonClickCallback(), equip");
		if (FitShipWindow::getInstance()->equipWeapon(this,this->weaponData) == true) {
			//如果能够装备, 则改写状态
			this->isEquipped = true;
			this->button->setTitleText(LanguageManager::getInstance()->getStringForKey("unEquipButton"));
			this->status->setString(LanguageManager::getInstance()->getStringForKey("statusEquipped"));
		}
		else {
			//如果不能装备则什么都不做
			return;
		}
	}
}



game::FitShipWindow * game::FitShipWindow::create(Data* shipData)
{
	cocos2d::log("INFO: FitShipWindow creating...Part 0");
	//生成界面
	FitShipWindow *window = new (std::nothrow) FitShipWindow;
	if (window && window->initWithFile("UI//UI-station-ship.png"))
	{
		window->autorelease();
	}
	else {
		CC_SAFE_DELETE(window);
		return nullptr;
	}
	_window = window; //设置实例
	_window->shipData = shipData;
	//设定自身位置
	_window->setAnchorPoint(cocos2d::Vec2::ZERO);
	_window->setPosition(252, 768 - 649);
	//加载滚动框
	cocos2d::log("INFO: FitShipWindow creating...Part 1");
	_window->weaponView = cocos2d::ui::ScrollView::create();
	_window->weaponView->setContentSize(cocos2d::Size(831-265,620-333));
	_window->weaponView->setInnerContainerSize(cocos2d::Size(831 - 265, 2000));
	_window->weaponView->setAnchorPoint(cocos2d::Vec2::ZERO);
	_window->weaponView->setPosition(cocos2d::Vec2(12,584-557));
	_window->addChild(_window->weaponView);
	cocos2d::log("INFO: FitShipWindow creating...Part 2");
	/*加载滚动框内的武器*/
	//加载DataLoader
	auto pGameDL = DataLoader::getDataLoader(DataLoaderType::gameDL);
	auto pSaveDL = DataLoader::getDataLoader(DataLoaderType::saveDL);
	//加载飞船拥有的武器
	int portID = 1;
	pair* p = shipData->findPairByKey("port_" + std::to_string(portID));
	while (p != nullptr) {
		auto panel = FitShipPanel::create(true,
			pSaveDL->findDataByLabel(p->value));
		_window->weaponView->addChild(panel);
		//递增
		portID++;
		p = shipData->findPairByKey("port_" + std::to_string(portID));
	}
	cocos2d::log("INFO: FitShipWindow creating...Part 3");
	//加载storage中的武器
	auto storageWeaponStrs = pSaveDL->findDataByLabel("storage")->findPairsByKey("weapon");
	for (auto j = storageWeaponStrs.begin(); j != storageWeaponStrs.end(); j++) {
		_window->weaponView->addChild(
			FitShipPanel::create(false,pSaveDL->findDataByLabel(*j))
		);
	}
	cocos2d::log("INFO: FitShipWindow creating...Part 4");
	//整理武器列表
	_window->sortPanel();
	//刷新界面
	_window->pFighter = nullptr;
	_window->shipLabel = cocos2d::Label::create("NAN", "fonts//msyh.ttf", 18);
	_window->shipLabel->setAnchorPoint(cocos2d::Vec2(0, 1));
	_window->shipLabel->setPosition(cocos2d::Vec2(207,520));
	_window->addChild(_window->shipLabel);

	cocos2d::log("INFO: FitShipWindow creating...Part 5");
	_window->flush();
	cocos2d::log("INFO: FitShipWindow creating...Done! ");
	return window;
}

game::FitShipWindow * game::FitShipWindow::getInstance()
{
	return nullptr;
}

bool game::FitShipWindow::equipWeapon(FitShipPanel * panel, Data * wData)
{
	//查找飞船是否有可用的端口
	std::string weaponType = wData->findPairByKey("weaponType")->value;
	std::string fighterType = _window->shipData->findPairByKey("fighterType")->value;
	int i = 1;
	do {
		pair* p = _window->shipData->findPairByKey("port_" + std::to_string(i));
		if (p == nullptr) break; //终止
		else {
			if (p->value == "empty" || p->value == "EMPTY") {
				//查找端口类型
				std::string portType = DataLoader::getDataLoader(DataLoaderType::gameDL)
					->findDataByLabel(fighterType)
					->findPairByKey("port_" + std::to_string(i))
					->value;
				if (portType == weaponType) { //如果端口类型正确,且端口是空的
					//将武器添加到端口
					p->value = wData->m_label;
					//将武器从storage中移除
					DataLoader::getDataLoader(DataLoaderType::saveDL)
						->findDataByLabel("storage")
						->deletePair("weapon", wData->m_label, false);
					//刷新
					_window->flush();
					return true;
				}
			}
		}
		i++;
	} while (1);
	return false;
}

bool game::FitShipWindow::unEquipWeapon(FitShipPanel * panel, Data * wData)
{
	//填充empty并取出实例
	pair* p = nullptr;
	//扫描端口
	int portID = 1;
	while ((p = _window->shipData->findPairByKey("port_" + std::to_string(portID))),
		(p != nullptr)) {
		if (p->value == wData->m_label) {
			//删除端口的武器
			p->value = "empty";
			//添加武器到仓库中
			DataLoader::getDataLoader(DataLoaderType::saveDL)
				->findDataByLabel("storage")
				->addPair("weapon", wData->m_label);
			//刷新
			_window->flush();
			return true;
		}
		portID++;
	}
	return false;
}


void game::FitShipWindow::sortPanel()
{
	//对现存的view进行排列
	auto vec = this->weaponView->getChildren();
	int viewHeight = this->weaponView->getInnerContainerSize().height;
	int height = 0;
	//已装备的优先排列
	for (auto j = vec.begin(); j != vec.end(); j++) {
		auto panel = dynamic_cast<FitShipPanel*>(*j);
		if (panel->isEquipped == true) {
			height += (*j)->getContentSize().height;
			(*j)->setPosition(0, viewHeight - height);
		}
	}
	for (auto j = vec.begin(); j != vec.end(); j++) {
		auto panel = dynamic_cast<FitShipPanel*>(*j);
		if (panel->isEquipped == false) {
			height += (*j)->getContentSize().height;
			(*j)->setPosition(0, viewHeight - height);
		}
	}
}

void game::FitShipWindow::flush()
{
	auto lang = LanguageManager::getInstance();
	//对面板和飞船外观进行刷新(直接生成一个新飞船实例)
	std::string str = lang->getStringForKey(_window->shipData->findPairByKey("fighterName")->value);
	//扫描接口
	int portID = 1;
	pair* p;
	auto shipGameData = 
		DataLoader::getDataLoader(DataLoaderType::gameDL)
		->findDataByLabel(
			_window->shipData->findPairByKey("fighterType")->value
		);
	while ((p = _window->shipData->findPairByKey("port_" + std::to_string(portID))), (p != nullptr)) {
		//找到接口对应的类型
		std::string portType = shipGameData->findPairByKey("port_" + std::to_string(portID))->value;
		str += "\n-" + lang->getStringForKey(portType);
		if (p->value == "empty" || p->value == "EMPTY") {
			str += ("-" + lang->getStringForKey("emptyPort"));
		}
		else {
			//找到这个接口对应的武器
			Data* wData = DataLoader::getDataLoader(DataLoaderType::saveDL)
				->findDataByLabel(p->value);

			auto attrs = util::getFixedAttr(wData);
			//添加武器属性描述
			str += ("-" + lang->getStringForKey(wData->findPairByKey("weaponType")->value));
			for (auto i = attrs.begin(); i != attrs.end(); i++) {
				if (str.length() >= 50) { //如果属性描述太长
					str += "...";
					break;
				}
				str += (" " + i->attrStr + ":" + util::shortenNum(i->attrValue));
			}
			
		}
		cocos2d::log(("INFO: FitShipWindow flush(): add port Info, portID=" + std::to_string(portID)).c_str());
		portID++;
	}
	//设置标签
	_window->shipLabel->setString(str);

}
