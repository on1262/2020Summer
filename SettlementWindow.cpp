#include "SettlementWindow.h"
#include "LanguageManager.h"
#include "util.h"

game::SettlementGemPanel * game::SettlementGemPanel::create(Data * gd, std::string ip)
{
	SettlementGemPanel *btn = new (std::nothrow) SettlementGemPanel;
	if ((btn && btn->init("UI//UI-settlement-left.png", "", "")) == false) return nullptr;
	btn->autorelease();
	//加载其他内容
	std::string type = LanguageManager::getInstance()->getStringForKey(gd->findPairByKey("gemType")->value);
	btn->label = cocos2d::Label::create(type + "," + gd->findPairByKey("gemValue")->value
		+ "\n" + LanguageManager::getInstance()->getStringForKey("sellPrice")
		+ gd->findPairByKey("gemPrice")->value
		, "fonts//msyh.ttf", 18);
	btn->label->setColor(cocos2d::Color3B(110, 198, 235));//淡蓝色
	btn->label->setVisible(false); //刚开始看不见
	btn->setTitleLabel(btn->label);
	//添加icon
	auto icon = cocos2d::ui::Button::create(ip, "", "");
	icon->setPosition(cocos2d::Vec2(40, 80));
	icon->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	btn->addChild(icon);
	btn->setAnchorPoint(cocos2d::Vec2::ZERO);
	//添加点击回传
	btn->addClickEventListener(CC_CALLBACK_0(buttonCallback, btn));
	return btn;
}

void game::SettlementGemPanel::buttonCallback()
{
	this->label->setVisible(true); //显示信息
}

game::SettlementWeaponPanel * game::SettlementWeaponPanel::create(Data * wd, std::string ip)
{
	SettlementWeaponPanel *btn = new (std::nothrow) SettlementWeaponPanel;
	if ((btn && btn->init("UI//UI-settlement-right.png", "", "")) == false) return nullptr;
	btn->autorelease();
	//加载武器类型名
	std::string type = LanguageManager::getInstance()->getStringForKey(wd->findPairByKey("weaponType")->value);
	//加载属性
	auto attrVec = wd->findAttrs();
	std::string labelStr;
	for (auto i = attrVec.begin(); i != attrVec.end(); i++) {
		if (labelStr.empty() == false) labelStr += "\n"; //最开始不换行
		labelStr += ((*i).attrStr + ":" + util::shortenNum((*i).attrValue) + "(" + util::shortenNum((*i).attrCoef) + ")");
	}
	btn->label = cocos2d::Label::create(
		labelStr, "fonts//msyh.ttf", 18);
	btn->label->setColor(cocos2d::Color3B(110, 198, 235));//淡蓝色
	btn->label->setVisible(false); //刚开始看不见
	btn->setTitleLabel(btn->label);
	//添加icon
	auto icon = cocos2d::ui::Button::create(ip,"", "");
	icon->setPosition(cocos2d::Vec2(40, 80));
	icon->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	btn->addChild(icon);
	btn->setAnchorPoint(cocos2d::Vec2::ZERO);
	//添加点击回传
	btn->addClickEventListener(CC_CALLBACK_0(buttonCallback, btn));
	return btn;
}

void game::SettlementWeaponPanel::buttonCallback()
{
	this->label->setVisible(true); //显示信息
}

game::SettlementWindow * game::SettlementWindow::create(initData init)
{
	cocos2d::log("INFO:initing SettlementWindow...Part 0");
	//初始化自身
	SettlementWindow *sw = new (std::nothrow) SettlementWindow();
	if ((sw && sw->initWithFile("UI//UI-combat-settlement.png")) == false) return nullptr;
	sw->autorelease();
	//设置自身位置
	sw->setPosition(241, 768 - 662);
	sw->setAnchorPoint(cocos2d::Vec2::ZERO);
	//初始化飞机图标
	if (init.player == nullptr) {
		cocos2d::log("Warning: init.player = nullptr");
	}
	init.player->setOpacity(150); //设置透明度
	sw->addChild(init.player);//改变父节点
	init.player->setPosition(124, 768 - 319);
	//初始化下侧的标签
	float rdiff = init.levelData->valueConvert<float>(init.levelData->findPairByKey("relativeDifficulty")->value);
	float tdiff = init.levelData->valueConvert<float>(init.levelData->findPairByKey("timeDifficulty")->value);
	float difficulty = rdiff * tdiff;
	//TODO: 这里的计算公式需要改进
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
	//初始化按钮1
	auto checkAllLabel = cocos2d::Label::create(lang->getStringForKey("checkAll"), "fonts//msyh.ttf", 24);
	auto checkAllButton = cocos2d::ui::Button::create();
	checkAllLabel->setColor(cocos2d::Color3B(14, 184, 255));
	checkAllButton->setTitleLabel(checkAllLabel); //设置标签
	checkAllButton->setAnchorPoint(cocos2d::Vec2::ZERO);
	checkAllButton->setPosition(cocos2d::Vec2(14, 768 - 671));
	checkAllButton->addClickEventListener(CC_CALLBACK_0(game::SettlementWindow::checkAllCallback, sw)); //添加监听
	sw->addChild(checkAllButton);
	//初始化按钮2
	auto returnLabel = cocos2d::Label::create(lang->getStringForKey("returnToStation"), "fonts//msyh.ttf", 24);
	auto returnButton = cocos2d::ui::Button::create();
	returnLabel->setColor(cocos2d::Color3B(14, 184, 255));
	returnButton->setTitleLabel(returnLabel); //设置标签
	returnButton->setAnchorPoint(cocos2d::Vec2::ZERO);
	returnButton->setPosition(cocos2d::Vec2(14, 768 - 718));
	returnButton->addClickEventListener(CC_CALLBACK_0(game::SettlementWindow::returnCallback, sw)); //添加监听
	sw->addChild(returnButton);
	cocos2d::log("INFO:initing SettlementWindow...Part 2");
	//初始化宝石栏
	sw->leftView = cocos2d::ui::ScrollView::create();
	sw->leftView->setAnchorPoint(cocos2d::Vec2::ZERO);
	sw->leftView->setContentSize(cocos2d::Size(224, 535));
	sw->leftView->setInnerContainerSize(cocos2d::Size(224, 2000));
	sw->leftView->setPosition(cocos2d::Vec2(246, 768 - 745));
	sw->addChild(sw->leftView);
	//初始化
	sw->data = init;
	//添加宝石到宝石栏中
	sw->addGemToLeftView();
	//初始化武器栏
	sw->rightView = cocos2d::ui::ScrollView::create();
	sw->rightView->setAnchorPoint(cocos2d::Vec2::ZERO);
	sw->rightView->setContentSize(cocos2d::Size(723 - 475, 745 - 210));
	sw->rightView->setInnerContainerSize(cocos2d::Size(723 - 475, 2000));
	sw->rightView->setPosition(cocos2d::Vec2(475, 768 - 745));
	sw->addChild(sw->rightView);
	cocos2d::log("INFO:initing SettlementWindow...Part 3");
	//添加武器
	sw->addWeaponToRightView();
	//对两侧栏进行排序
	sw->sortPanel(sw->rightView);
	sw->sortPanel(sw->leftView);
	cocos2d::log("INFO:initing SettlementWindow...Done! ");
	return sw;
}

void game::SettlementWindow::addGemToLeftView()
{
	//关卡难度, 宝石属性赋值是在关卡难度上下浮动的
	float levelDifficulty = data.levelData->valueConvert<float>(data.levelData->findPairByKey("relativeDifficulty")->value);
	for (auto i = data.vec.begin(); i != data.vec.end(); i++) {
		if (i->_dropType == dropType::gemType) { //如果是gem
			Data* pData = new Data();
			float value;
			//生成一个pData
			pData->m_label = "gem_" + std::to_string(data.saveDL->getAvailableDataLabel("gem_"));
			cocos2d::log(("gemType:" + i->itemStr).c_str());
			if (i->itemStr == "gem_blue") { //linear类型强度与关卡难度成正比
				value = levelDifficulty * util::getRandom(0.5, 2);
				pData->addPair("gemType", "linear");
			}
			else if (i->itemStr == "gem_purple") {
				value = util::getRandom(0.5, 9.5);
				pData->addPair("gemType", "advanced");
			}
			pData->addPair("gemValue", util::shortenNum(value));
			pData->addPair("gemPrice", std::to_string((int)(10 * value)));
			//加入到存档中
			data.saveDL->loaderVector->push_back(pData);
			//实例化并加入到scrollView中
			this->leftView->addChild(
				SettlementGemPanel::create(pData, data.gameDL->findDataByLabel(i->itemStr)->findPairByKey("texturePath")->value)
			);
			//加入到storage中
			data.saveDL->findDataByLabel("storage")->addPair("gem", pData->m_label);
		}
	}
}

void game::SettlementWindow::addWeaponToRightView()
{
	//武器数值随着关卡难度浮动
	float levelDifficulty = data.levelData->valueConvert<float>(data.levelData->findPairByKey("relativeDifficulty")->value);
	for (auto i = data.vec.begin(); i != data.vec.end(); i++) {
		if (i->_dropType == dropType::weaponType) { //如果是武器类
			//构造一个data
			Data* pData = new Data();
			pData->m_label = "weapon_" + std::to_string(data.saveDL->getAvailableDataLabel("weapon_"));
			pData->addPair("weaponType", data.gameDL->findDataByLabel(i->itemStr)->findPairByKey("weaponType")->value);
			auto attrVec = data.gameDL->findDataByLabel(i->itemStr)->findAttrs();
			for (auto j = attrVec.begin(); j != attrVec.end(); j++) { //逐个属性录入
				pData->addPair("attribute", j->attrName);
				pData->addPair("attr_" + j->attrName, util::shortenNum((util::getRandom(0,0.1*levelDifficulty) + 1) * j->attrValue));
				pData->addPair("coef_" + j->attrName, util::shortenNum(j->attrValue));
			}
			//添加到save_1.txt
			data.saveDL->loaderVector->push_back(pData);
			//添加到storage中
			data.saveDL->findDataByLabel("storage")->addPair("weapon", pData->m_label);
			//实例化并添加到scrollView中
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
		height += (*i)->getContentSize().height; //自动调整高度
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
