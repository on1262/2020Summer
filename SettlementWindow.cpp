#include "SettlementWindow.h"
#include "LanguageManager.h"
#include "util.h"

game::SettlementGemPanel * game::SettlementGemPanel::create(Data * gd, std::string ip)
{
	SettlementGemPanel *btn = new (std::nothrow) SettlementGemPanel;
	if ((btn && btn->init("UI//UI-settlement-left.png", "", "")) == false) return nullptr;
	btn->autorelease();
	//������������
	std::string type = LanguageManager::getInstance()->getStringForKey(gd->findPairByKey("gemType")->value);
	btn->label = cocos2d::Label::create(type + "," + gd->findPairByKey("gemValue")->value
		+ "\n" + LanguageManager::getInstance()->getStringForKey("sellPrice")
		+ gd->findPairByKey("gemPrice")->value
		, "fonts//msyh.ttf", 18);
	btn->label->setColor(cocos2d::Color3B(110, 198, 235));//����ɫ
	btn->label->setVisible(false); //�տ�ʼ������
	btn->setTitleLabel(btn->label);
	//���icon
	auto icon = cocos2d::ui::Button::create(ip, "", "");
	icon->setPosition(cocos2d::Vec2(40, 80));
	icon->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	btn->addChild(icon);
	btn->setAnchorPoint(cocos2d::Vec2::ZERO);
	//��ӵ���ش�
	btn->addClickEventListener(CC_CALLBACK_0(buttonCallback, btn));
	return btn;
}

void game::SettlementGemPanel::buttonCallback()
{
	this->label->setVisible(true); //��ʾ��Ϣ
}

game::SettlementWeaponPanel * game::SettlementWeaponPanel::create(Data * wd, std::string ip)
{
	SettlementWeaponPanel *btn = new (std::nothrow) SettlementWeaponPanel;
	if ((btn && btn->init("UI//UI-settlement-right.png", "", "")) == false) return nullptr;
	btn->autorelease();
	//��������������
	std::string type = LanguageManager::getInstance()->getStringForKey(wd->findPairByKey("weaponType")->value);
	//��������
	auto attrVec = wd->findAttrs();
	std::string labelStr;
	for (auto i = attrVec.begin(); i != attrVec.end(); i++) {
		if (labelStr.empty() == false) labelStr += "\n"; //�ʼ������
		labelStr += ((*i).attrStr + ":" + util::shortenNum((*i).attrValue) + "(" + util::shortenNum((*i).attrCoef) + ")");
	}
	btn->label = cocos2d::Label::create(
		labelStr, "fonts//msyh.ttf", 18);
	btn->label->setColor(cocos2d::Color3B(110, 198, 235));//����ɫ
	btn->label->setVisible(false); //�տ�ʼ������
	btn->setTitleLabel(btn->label);
	//���icon
	auto icon = cocos2d::ui::Button::create(ip,"", "");
	icon->setPosition(cocos2d::Vec2(40, 80));
	icon->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	btn->addChild(icon);
	btn->setAnchorPoint(cocos2d::Vec2::ZERO);
	//��ӵ���ش�
	btn->addClickEventListener(CC_CALLBACK_0(buttonCallback, btn));
	return btn;
}

void game::SettlementWeaponPanel::buttonCallback()
{
	this->label->setVisible(true); //��ʾ��Ϣ
}

game::SettlementWindow * game::SettlementWindow::create(initData init)
{
	cocos2d::log("INFO:initing SettlementWindow...Part 0");
	//��ʼ������
	SettlementWindow *sw = new (std::nothrow) SettlementWindow();
	if ((sw && sw->initWithFile("UI//UI-combat-settlement.png")) == false) return nullptr;
	sw->autorelease();
	//��������λ��
	sw->setPosition(241, 768 - 662);
	sw->setAnchorPoint(cocos2d::Vec2::ZERO);
	//��ʼ���ɻ�ͼ��
	if (init.player == nullptr) {
		cocos2d::log("Warning: init.player = nullptr");
	}
	init.player->setOpacity(150); //����͸����
	sw->addChild(init.player);//�ı丸�ڵ�
	init.player->setPosition(124, 768 - 319);
	//��ʼ���²�ı�ǩ
	float rdiff = init.levelData->valueConvert<float>(init.levelData->findPairByKey("relativeDifficulty")->value);
	float tdiff = init.levelData->valueConvert<float>(init.levelData->findPairByKey("timeDifficulty")->value);
	float difficulty = rdiff * tdiff;
	//TODO: ����ļ��㹫ʽ��Ҫ�Ľ�
	float score = difficulty * (5 * init.destroyEnemyCount + 10 * init.getGemCount + init.getMachinePartCount);
	auto lang = LanguageManager::getInstance();
	auto leftLabel = cocos2d::Label::create(
		lang->getStringForKey("levelDesignedDifficulty") + util::shortenNum(difficulty) + "\n"
		+ lang->getStringForKey("evaluation") + util::shortenNum(score) + "\n"
		+ lang->getStringForKey("destroyEnemyCount") + std::to_string(init.destroyEnemyCount) + "\n"
		+ lang->getStringForKey("getGemCount") + std::to_string(init.getGemCount) + "\n"
		+ lang->getStringForKey("getWeaponCount") + std::to_string(init.getWeaponCount),
		"fonts//msyh.ttf", 18
	);
	leftLabel->setAnchorPoint(cocos2d::Vec2(0, 1));
	leftLabel->setPosition(14, 768 - 441);
	sw->addChild(leftLabel);
	cocos2d::log("INFO:initing SettlementWindow...Part 1");
	//��ʼ����ť1
	auto checkAllLabel = cocos2d::Label::create(lang->getStringForKey("checkAll"), "fonts//msyh.ttf", 24);
	auto checkAllButton = cocos2d::ui::Button::create();
	checkAllLabel->setColor(cocos2d::Color3B(14, 184, 255));
	checkAllButton->setTitleLabel(checkAllLabel); //���ñ�ǩ
	checkAllButton->setAnchorPoint(cocos2d::Vec2::ZERO);
	checkAllButton->setPosition(cocos2d::Vec2(14, 768 - 671));
	checkAllButton->addClickEventListener(CC_CALLBACK_0(game::SettlementWindow::checkAllCallback, sw)); //��Ӽ���
	sw->addChild(checkAllButton);
	//��ʼ����ť2
	auto returnLabel = cocos2d::Label::create(lang->getStringForKey("returnToStation"), "fonts//msyh.ttf", 24);
	auto returnButton = cocos2d::ui::Button::create();
	returnLabel->setColor(cocos2d::Color3B(14, 184, 255));
	returnButton->setTitleLabel(returnLabel); //���ñ�ǩ
	returnButton->setAnchorPoint(cocos2d::Vec2::ZERO);
	returnButton->setPosition(cocos2d::Vec2(14, 768 - 718));
	returnButton->addClickEventListener(CC_CALLBACK_0(game::SettlementWindow::returnCallback, sw)); //��Ӽ���
	sw->addChild(returnButton);
	cocos2d::log("INFO:initing SettlementWindow...Part 2");
	//��ʼ����ʯ��
	sw->leftView = cocos2d::ui::ScrollView::create();
	sw->leftView->setAnchorPoint(cocos2d::Vec2::ZERO);
	sw->leftView->setContentSize(cocos2d::Size(224, 535));
	sw->leftView->setInnerContainerSize(cocos2d::Size(224, 2000));
	sw->leftView->setPosition(cocos2d::Vec2(246, 768 - 745));
	sw->addChild(sw->leftView);
	//��ʼ��
	sw->data = init;
	//��ӱ�ʯ����ʯ����
	sw->addGemToLeftView();
	//��ʼ��������
	sw->rightView = cocos2d::ui::ScrollView::create();
	sw->rightView->setAnchorPoint(cocos2d::Vec2::ZERO);
	sw->rightView->setContentSize(cocos2d::Size(723 - 475, 745 - 210));
	sw->rightView->setInnerContainerSize(cocos2d::Size(723 - 475, 2000));
	sw->rightView->setPosition(cocos2d::Vec2(475, 768 - 745));
	sw->addChild(sw->rightView);
	cocos2d::log("INFO:initing SettlementWindow...Part 3");
	//�������
	sw->addWeaponToRightView();
	//����������������
	sw->sortPanel(sw->rightView);
	sw->sortPanel(sw->leftView);
	cocos2d::log("INFO:initing SettlementWindow...Done! ");
	return sw;
}

void game::SettlementWindow::addGemToLeftView()
{
	//�ؿ��Ѷ�, ��ʯ���Ը�ֵ���ڹؿ��Ѷ����¸�����
	float levelDifficulty = data.levelData->valueConvert<float>(data.levelData->findPairByKey("relativeDifficulty")->value);
	for (auto i = data.vec.begin(); i != data.vec.end(); i++) {
		if (i->_dropType == dropType::gemType) { //�����gem
			Data* pData = new Data();
			float value;
			//����һ��pData
			pData->m_label = "gem_" + std::to_string(data.saveDL->getAvailableDataLabel("gem_"));
			cocos2d::log(("gemType:" + i->itemStr).c_str());
			if (i->itemStr == "gem_blue") { //linear����ǿ����ؿ��Ѷȳ�����
				value = levelDifficulty * util::getRandom(0.5, 2);
				pData->addPair("gemType", "linear");
			}
			else if (i->itemStr == "gem_purple") {
				value = util::getRandom(0.5, 9.5);
				pData->addPair("gemType", "advanced");
			}
			pData->addPair("gemValue", util::shortenNum(value));
			pData->addPair("gemPrice", std::to_string((int)(10 * value)));
			//���뵽�浵��
			data.saveDL->loaderVector->push_back(pData);
			//ʵ���������뵽scrollView��
			this->leftView->addChild(
				SettlementGemPanel::create(pData, data.gameDL->findDataByLabel(i->itemStr)->findPairByKey("texturePath")->value)
			);
			//���뵽storage��
			data.saveDL->findDataByLabel("storage")->addPair("gem", pData->m_label);
		}
	}
}

void game::SettlementWindow::addWeaponToRightView()
{
	//������ֵ���Źؿ��Ѷȸ���
	float levelDifficulty = data.levelData->valueConvert<float>(data.levelData->findPairByKey("relativeDifficulty")->value);
	for (auto i = data.vec.begin(); i != data.vec.end(); i++) {
		if (i->_dropType == dropType::weaponType) { //�����������
			//����һ��data
			Data* pData = new Data();
			pData->m_label = "weapon_" + std::to_string(data.saveDL->getAvailableDataLabel("weapon_"));
			pData->addPair("weaponType", data.gameDL->findDataByLabel(i->itemStr)->findPairByKey("weaponType")->value);
			auto attrVec = data.gameDL->findDataByLabel(i->itemStr)->findAttrs();
			for (auto j = attrVec.begin(); j != attrVec.end(); j++) { //�������¼��
				pData->addPair("attribute", j->attrName);
				pData->addPair("attr_" + j->attrName, util::shortenNum((util::getRandom(0,0.1*levelDifficulty) + 1) * j->attrValue));
				pData->addPair("coef_" + j->attrName, util::shortenNum(j->attrValue));
			}
			//��ӵ�save_1.txt
			data.saveDL->loaderVector->push_back(pData);
			//��ӵ�storage��
			data.saveDL->findDataByLabel("storage")->addPair("weapon", pData->m_label);
			//ʵ��������ӵ�scrollView��
			this->rightView->addChild(
				SettlementWeaponPanel::create(pData, data.gameDL->findDataByLabel(i->itemStr)->findPairByKey("texturePath")->value)
				);
		}
	}
}

void game::SettlementWindow::sortPanel(cocos2d::ui::ScrollView * sv)
{
	auto vec = sv->getChildren();
	int height = 0;
	int scrollViewHeight = sv->getInnerContainerSize().height;
	for (auto i = vec.begin(); i != vec.end(); i++) {
		height += (*i)->getContentSize().height; //�Զ������߶�
		(*i)->setPosition(0, scrollViewHeight - height);
	}
}

void game::SettlementWindow::checkAllCallback()
{
	cocos2d::log("check all call back");
}

void game::SettlementWindow::returnCallback()
{
	cocos2d::log("return call back");
}
