#include "stationScene.h"
#include "LanguageManager.h"
USING_NS_CC;

void stationScene::setDataLoader(game::DataLoader * pGame, game::DataLoader * pUser)
{
	this->pGameDataLoader = pGame;
	this->pUserDataLoader = pUser;
	if (pGame != nullptr && pUser != nullptr) {
		this->isLoaded = true;
	}
}

bool stationScene::init()
{

	//初始化面板打开状态
	this->fitWeapon = nullptr;
	this->isLoaded = false; //设置
	this->pGameDataLoader = nullptr;
	this->pUserDataLoader = nullptr;
	// @test 这几行代码是测试代码
	//文件存取器初始化:userDataLoader和gameDataLoader
	this->pGameDataLoader = new game::DataLoader(std::string("gameData//"), std::string("gameData.txt"));
	this->pUserDataLoader = new game::DataLoader(std::string("save//"), std::string("save_1.txt"));

	return Scene::init(); //执行父函数
}

void stationScene::onEnter()
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
	auto fitWeaponLabel = Label::createWithTTF(dic->getStringForKey("fitWeapon"), "fonts//msyh.ttf", 20);
	auto test = MenuItemLabel::create(fitWeaponLabel,CC_CALLBACK_0(stationScene::chickFitWeaponWindow,this));
	fitWeaponLabel->setColor(Color3B(14, 184, 255)); //蓝色
	auto menu = Menu::create();
	menu->addChild(test);
	test->setAnchorPoint(Vec2::ZERO);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(11, 768 - 246);
	
	//加载所有组件
	this->addChild(stationBackground);
	this->addChild(leftBoard);
	this->addChild(planetLabel);
	this->addChild(menu);
}

void stationScene::chickFitWeaponWindow()
{

	if (fitWeapon == nullptr) {
		cocos2d::log("INFO: create fit weapon window");
		fitWeapon = game::FitWeaponWindow::create(this);
		this->addChild(fitWeapon);
	}

}

void stationScene::chickFitShipWindow()
{
}

void stationScene::chickStorageWindow()
{
}

void stationScene::chickStarMapWindow()
{
}

void stationScene::chickSaveWindow()
{
}

void stationScene::closeAll() //关闭所有窗口
{
	if (fitWeapon == nullptr) chickFitWeaponWindow();
}
