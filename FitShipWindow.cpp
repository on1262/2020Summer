#include "FitShipWindow.h"
#include "LanguageManager.h"
#include "util.h"

game::FitShipWindow* game::FitShipWindow::_window = nullptr;

game::FitShipPanel * game::FitShipPanel::create(bool isEquipped, Data * weaponData)
{
	//���뱳��
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
	//����DataLoader
	auto pGameDL = DataLoader::getDataLoader(DataLoaderType::gameDL);
	auto pSaveDL = DataLoader::getDataLoader(DataLoaderType::saveDL);
	//����language
	auto lang = LanguageManager::getInstance();
	//����������ͼ��
	std::string weaponType = weaponData->findPairByKey("weaponType")->value;
	auto weaponIcon = cocos2d::Sprite::create(
		pGameDL->findDataByLabel(weaponType)->findPairByKey("texturePath")->value
	);
	weaponIcon->setRotation(90.0f);
	weaponIcon->setPosition(cocos2d::Vec2(50,41));
	panel->addChild(weaponIcon);
	//��weaponData�еõ������������
	auto attrs = util::getFixedAttr(weaponData);
	//�����ݼ��ص���ǩ��
	std::string str = lang->getStringForKey(weaponType) + "\n";
	for (auto j = attrs.begin(); j != attrs.end(); j++) {
		str += j->attrStr + ":" + util::shortenNum(j->attrValue) + " ";
	}
	auto leftLabel = cocos2d::Label::create(str, "fonts//msyh.ttf", 20);
	leftLabel->setAnchorPoint(cocos2d::Vec2(0, 0.5));
	leftLabel->setPosition(160, 40);
	panel->addChild(leftLabel);
	//�������Ͻǵ�״̬��ǩ
	panel->isEquipped = isEquipped;
	if (isEquipped == true) panel->status = cocos2d::Label::create(lang->getStringForKey("statusEquipped"), "fonts//msyh.ttf", 20);
	else panel->status = cocos2d::Label::create(lang->getStringForKey("statusUnEquipped"), "fonts//msyh.ttf", 20);
	panel->status->setAnchorPoint(cocos2d::Vec2::ZERO);
	panel->status->setPosition(368, 50);
	panel->addChild(panel->status);
	//�������Ͻǵİ�ť
	cocos2d::ui::Button* button = cocos2d::ui::Button::create();
	panel->button = button;
	if (isEquipped == true) button->setTitleText(lang->getStringForKey("unEquipButton"));
	else button->setTitleText(lang->getStringForKey("equipButton"));
	button->setTitleColor(cocos2d::Color3B(14, 184, 255));
	button->setTitleFontName("fonts//msyh.ttf");
	button->setTitleFontSize(24);
	button->setAnchorPoint(cocos2d::Vec2(0.5,0));
	button->setPosition(cocos2d::Vec2(502,50));
	//���ð�ť�Ļص�����
	button->addClickEventListener(CC_CALLBACK_0(FitShipPanel::buttonClickCallback, panel));
	panel->addChild(button);
	return panel;
}

void game::FitShipPanel::buttonClickCallback()
{
	if (this->isEquipped == true) { //���ж�°�ť
		//��д״̬
		cocos2d::log("INFO: FitShipPanel:buttonClickCallback(), unEquip");
		this->isEquipped = false;
		this->button->setTitleText(LanguageManager::getInstance()->getStringForKey("equipButton"));
		this->status->setString(LanguageManager::getInstance()->getStringForKey("statusUnEquipped"));
		//ж��
		FitShipWindow::getInstance()->unEquipWeapon(this, this->weaponData);
	}
	else { //���װ����ť
		cocos2d::log("INFO: FitShipPanel:buttonClickCallback(), equip");
		if (FitShipWindow::getInstance()->equipWeapon(this,this->weaponData) == true) {
			//����ܹ�װ��, ���д״̬
			this->isEquipped = true;
			this->button->setTitleText(LanguageManager::getInstance()->getStringForKey("unEquipButton"));
			this->status->setString(LanguageManager::getInstance()->getStringForKey("statusEquipped"));
		}
		else {
			//�������װ����ʲô������
			return;
		}
	}
}



game::FitShipWindow * game::FitShipWindow::create(Data* shipData)
{
	cocos2d::log("INFO: FitShipWindow creating...Part 0");
	//���ɽ���
	FitShipWindow *window = new (std::nothrow) FitShipWindow;
	if (window && window->initWithFile("UI//UI-station-ship.png"))
	{
		window->autorelease();
	}
	else {
		CC_SAFE_DELETE(window);
		return nullptr;
	}
	_window = window; //����ʵ��
	_window->shipData = shipData;
	//�趨����λ��
	_window->setAnchorPoint(cocos2d::Vec2::ZERO);
	_window->setPosition(252, 768 - 649);
	//���ع�����
	cocos2d::log("INFO: FitShipWindow creating...Part 1");
	_window->weaponView = cocos2d::ui::ScrollView::create();
	_window->weaponView->setContentSize(cocos2d::Size(831-265,620-333));
	_window->weaponView->setInnerContainerSize(cocos2d::Size(831 - 265, 2000));
	_window->weaponView->setAnchorPoint(cocos2d::Vec2::ZERO);
	_window->weaponView->setPosition(cocos2d::Vec2(12,584-557));
	_window->addChild(_window->weaponView);
	cocos2d::log("INFO: FitShipWindow creating...Part 2");
	/*���ع������ڵ�����*/
	//����DataLoader
	auto pGameDL = DataLoader::getDataLoader(DataLoaderType::gameDL);
	auto pSaveDL = DataLoader::getDataLoader(DataLoaderType::saveDL);
	//���طɴ�ӵ�е�����
	int portID = 1;
	pair* p = shipData->findPairByKey("port_" + std::to_string(portID));
	while (p != nullptr) {
		auto panel = FitShipPanel::create(true,
			pSaveDL->findDataByLabel(p->value));
		_window->weaponView->addChild(panel);
		//����
		portID++;
		p = shipData->findPairByKey("port_" + std::to_string(portID));
	}
	cocos2d::log("INFO: FitShipWindow creating...Part 3");
	//����storage�е�����
	auto storageWeaponStrs = pSaveDL->findDataByLabel("storage")->findPairsByKey("weapon");
	for (auto j = storageWeaponStrs.begin(); j != storageWeaponStrs.end(); j++) {
		_window->weaponView->addChild(
			FitShipPanel::create(false,pSaveDL->findDataByLabel(*j))
		);
	}
	cocos2d::log("INFO: FitShipWindow creating...Part 4");
	//���������б�
	_window->sortPanel();
	//ˢ�½���
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
	//���ҷɴ��Ƿ��п��õĶ˿�
	std::string weaponType = wData->findPairByKey("weaponType")->value;
	std::string fighterType = _window->shipData->findPairByKey("fighterType")->value;
	int i = 1;
	do {
		pair* p = _window->shipData->findPairByKey("port_" + std::to_string(i));
		if (p == nullptr) break; //��ֹ
		else {
			if (p->value == "empty" || p->value == "EMPTY") {
				//���Ҷ˿�����
				std::string portType = DataLoader::getDataLoader(DataLoaderType::gameDL)
					->findDataByLabel(fighterType)
					->findPairByKey("port_" + std::to_string(i))
					->value;
				if (portType == weaponType) { //����˿�������ȷ,�Ҷ˿��ǿյ�
					//��������ӵ��˿�
					p->value = wData->m_label;
					//��������storage���Ƴ�
					DataLoader::getDataLoader(DataLoaderType::saveDL)
						->findDataByLabel("storage")
						->deletePair("weapon", wData->m_label, false);
					//ˢ��
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
	//���empty��ȡ��ʵ��
	pair* p = nullptr;
	//ɨ��˿�
	int portID = 1;
	while ((p = _window->shipData->findPairByKey("port_" + std::to_string(portID))),
		(p != nullptr)) {
		if (p->value == wData->m_label) {
			//ɾ���˿ڵ�����
			p->value = "empty";
			//����������ֿ���
			DataLoader::getDataLoader(DataLoaderType::saveDL)
				->findDataByLabel("storage")
				->addPair("weapon", wData->m_label);
			//ˢ��
			_window->flush();
			return true;
		}
		portID++;
	}
	return false;
}


void game::FitShipWindow::sortPanel()
{
	//���ִ��view��������
	auto vec = this->weaponView->getChildren();
	int viewHeight = this->weaponView->getInnerContainerSize().height;
	int height = 0;
	//��װ������������
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
	//�����ͷɴ���۽���ˢ��(ֱ������һ���·ɴ�ʵ��)
	std::string str = lang->getStringForKey(_window->shipData->findPairByKey("fighterName")->value);
	//ɨ��ӿ�
	int portID = 1;
	pair* p;
	auto shipGameData = 
		DataLoader::getDataLoader(DataLoaderType::gameDL)
		->findDataByLabel(
			_window->shipData->findPairByKey("fighterType")->value
		);
	while ((p = _window->shipData->findPairByKey("port_" + std::to_string(portID))), (p != nullptr)) {
		//�ҵ��ӿڶ�Ӧ������
		std::string portType = shipGameData->findPairByKey("port_" + std::to_string(portID))->value;
		str += "\n-" + lang->getStringForKey(portType);
		if (p->value == "empty" || p->value == "EMPTY") {
			str += ("-" + lang->getStringForKey("emptyPort"));
		}
		else {
			//�ҵ�����ӿڶ�Ӧ������
			Data* wData = DataLoader::getDataLoader(DataLoaderType::saveDL)
				->findDataByLabel(p->value);

			auto attrs = util::getFixedAttr(wData);
			//���������������
			str += ("-" + lang->getStringForKey(wData->findPairByKey("weaponType")->value));
			for (auto i = attrs.begin(); i != attrs.end(); i++) {
				if (str.length() >= 50) { //�����������̫��
					str += "...";
					break;
				}
				str += (" " + i->attrStr + ":" + util::shortenNum(i->attrValue));
			}
			
		}
		cocos2d::log(("INFO: FitShipWindow flush(): add port Info, portID=" + std::to_string(portID)).c_str());
		portID++;
	}
	//���ñ�ǩ
	_window->shipLabel->setString(str);

}
