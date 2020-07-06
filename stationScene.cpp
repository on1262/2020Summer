#include "stationScene.h"
#include "LanguageManager.h"
USING_NS_CC;

void game::stationScene::setDataLoader(game::DataLoader * pGame, game::DataLoader * pUser)
{
	this->pGameDataLoader = pGame;
	this->pUserDataLoader = pUser;
	if (pGame != nullptr && pUser != nullptr) {
		this->isLoaded = true;
	}
}

bool game::stationScene::init()
{

	//初始化面板打开状态
	this->fitWeapon = nullptr;
	this->isLoaded = false; //设置
	this->pGameDataLoader = nullptr;
	this->pUserDataLoader = nullptr;
	// @test 这几行代码是测试代码

	return Scene::init(); //执行父函数
}

void game::stationScene::onEnter()
{
	Scene::onEnter();
	if (this->isLoaded == false) {
		//如果没有在创建后对接数据，则报错
		cocos2d::log("Error: Please call setDataLoader before onEnter().");
		//return;
	}
	//加载背景图片
	Sprite* stationBackground = Sprite::create("background//background-station.png");
	stationBackground->setAnchorPoint(Vec2(0, 0));//设置左下角为锚点
	stationBackground->setPosition(205, 0);
	//加载左侧方框
	Sprite* leftBoard = Sprite::create("UI//UI-station-left.png");
	leftBoard->setAnchorPoint(Vec2(0, 0));
	leftBoard->setPosition(0, 0);
	/*加载UI界面*/
	LanguageManager* dic = LanguageManager::getInstance();
	//加载“行星”显示栏
	auto planetLabel = Label::createWithTTF(dic->getStringForKey("planetAXIV"), "fonts//msyh.ttf", 24);
	planetLabel->setColor(Color3B(14, 184, 255)); //蓝色
	planetLabel->setAnchorPoint(Vec2::ZERO);
	planetLabel->setPosition(11, 768 - 121);
	//加载“提示”显示栏
	auto infoLabel = Label::createWithTTF(dic->getStringForKey("helloText"), "fonts//msyh.ttf", 24);
	infoLabel->setColor(Color3B(14, 184, 255)); //蓝色
	infoLabel->setAnchorPoint(Vec2::ZERO);
	infoLabel->setPosition(11, 768 - 184);
	//加载选项菜单栏
	//配置飞船
	auto fitShipLabel = Label::createWithTTF(dic->getStringForKey("fitShip"), "fonts//msyh.ttf", 20);
	auto fitShipItem = MenuItemLabel::create(fitShipLabel, CC_CALLBACK_0(stationScene::chickFitShipWindow, this));
	fitShipLabel->setColor(Color3B(14, 184, 255)); //蓝色
	//加载武器
	auto fitWeaponLabel = Label::createWithTTF(dic->getStringForKey("fitWeapon"), "fonts//msyh.ttf", 20);
	auto fitWeaponItem = MenuItemLabel::create(fitWeaponLabel,CC_CALLBACK_0(stationScene::chickFitWeaponWindow,this));
	fitWeaponLabel->setColor(Color3B(14, 184, 255)); //蓝色
	//打开星图
	auto openStarMapLabel = Label::createWithTTF(dic->getStringForKey("openStarMap"), "fonts//msyh.ttf", 20);
	auto openStarMapItem = MenuItemLabel::create(openStarMapLabel, CC_CALLBACK_0(stationScene::chickStarMapWindow, this));
	openStarMapLabel->setColor(Color3B(14, 184, 255)); //蓝色
	//存档
	auto openSaveLabel = Label::createWithTTF(dic->getStringForKey("openSave"), "fonts//msyh.ttf", 20);
	auto openSaveItem = MenuItemLabel::create(openSaveLabel, CC_CALLBACK_0(stationScene::chickSaveWindow, this));
	openSaveLabel->setColor(Color3B(14, 184, 255)); //蓝色
	//指定位置
	fitShipItem->setAnchorPoint(cocos2d::Vec2::ZERO);
	fitWeaponItem->setAnchorPoint(cocos2d::Vec2::ZERO);
	openStarMapItem->setAnchorPoint(cocos2d::Vec2::ZERO);
	openSaveItem->setAnchorPoint(cocos2d::Vec2::ZERO);
	fitShipItem->setPosition(10, 768-242);
	fitWeaponItem->setPosition(10, 768 - 282);
	openStarMapItem->setPosition(10, 768 - 322);
	openSaveItem->setPosition(10, 768 - 362);
	
	//添加到菜单
	auto menu = Menu::create();
	menu->addChild(fitShipItem);
	menu->addChild(fitWeaponItem);
	menu->addChild(openStarMapItem);
	menu->addChild(openSaveItem);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(0,0);
	
	//加载所有组件
	this->addChild(stationBackground);
	this->addChild(leftBoard);
	this->addChild(planetLabel);
	this->addChild(menu);
}

void game::stationScene::chickFitWeaponWindow()
{
	
	if (fitWeapon == nullptr) {
		this->closeAll(); //先清空,再打开
		cocos2d::log("INFO: create fit weapon window");
		fitWeapon = game::FitWeaponWindow::create(this);
		this->addChild(fitWeapon);
	}
	else {
		this->removeChild(fitWeapon, true);
		fitWeapon = nullptr;
	}

}

void game::stationScene::chickFitShipWindow()
{
	
	if (fitShip == nullptr) {
		this->closeAll(); //先清空,再打开
		cocos2d::log("INFO: create fit ship window");
		//传入shipData
		auto dl = game::DataLoader::getDataLoader(game::DataLoaderType::saveDL);
		auto shipData = dl->findDataByLabel(
			dl->findDataByLabel("player_data")->findPairByKey("choosedFighter")->value
		);
		fitShip = game::FitShipWindow::create(shipData);
		this->addChild(fitShip);
	}
	else {
		this->removeChild(fitShip, true);
		fitShip = nullptr;
	}
}

void game::stationScene::chickStorageWindow()
{
}

void game::stationScene::chickStarMapWindow()
{
	//直接向manager申请进入下一关
	game::SceneManager::getInstance()->stationSceneToFightScene();
}

void game::stationScene::chickSaveWindow()
{
}

void game::stationScene::closeAll() //关闭所有窗口
{
	if (fitWeapon != nullptr) chickFitWeaponWindow();
	if (fitShip != nullptr) chickFitShipWindow();
}
