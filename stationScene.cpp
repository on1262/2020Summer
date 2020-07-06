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

	//��ʼ������״̬
	this->fitWeapon = nullptr;
	this->isLoaded = false; //����
	this->pGameDataLoader = nullptr;
	this->pUserDataLoader = nullptr;
	// @test �⼸�д����ǲ��Դ���

	return Scene::init(); //ִ�и�����
}

void game::stationScene::onEnter()
{
	Scene::onEnter();
	if (this->isLoaded == false) {
		//���û���ڴ�����Խ����ݣ��򱨴�
		cocos2d::log("Error: Please call setDataLoader before onEnter().");
		//return;
	}
	//���ر���ͼƬ
	Sprite* stationBackground = Sprite::create("background//background-station.png");
	stationBackground->setAnchorPoint(Vec2(0, 0));//�������½�Ϊê��
	stationBackground->setPosition(205, 0);
	//������෽��
	Sprite* leftBoard = Sprite::create("UI//UI-station-left.png");
	leftBoard->setAnchorPoint(Vec2(0, 0));
	leftBoard->setPosition(0, 0);
	/*����UI����*/
	LanguageManager* dic = LanguageManager::getInstance();
	//���ء����ǡ���ʾ��
	auto planetLabel = Label::createWithTTF(dic->getStringForKey("planetAXIV"), "fonts//msyh.ttf", 24);
	planetLabel->setColor(Color3B(14, 184, 255)); //��ɫ
	planetLabel->setAnchorPoint(Vec2::ZERO);
	planetLabel->setPosition(11, 768 - 121);
	//���ء���ʾ����ʾ��
	auto infoLabel = Label::createWithTTF(dic->getStringForKey("helloText"), "fonts//msyh.ttf", 24);
	infoLabel->setColor(Color3B(14, 184, 255)); //��ɫ
	infoLabel->setAnchorPoint(Vec2::ZERO);
	infoLabel->setPosition(11, 768 - 184);
	//����ѡ��˵���
	//���÷ɴ�
	auto fitShipLabel = Label::createWithTTF(dic->getStringForKey("fitShip"), "fonts//msyh.ttf", 20);
	auto fitShipItem = MenuItemLabel::create(fitShipLabel, CC_CALLBACK_0(stationScene::chickFitShipWindow, this));
	fitShipLabel->setColor(Color3B(14, 184, 255)); //��ɫ
	//��������
	auto fitWeaponLabel = Label::createWithTTF(dic->getStringForKey("fitWeapon"), "fonts//msyh.ttf", 20);
	auto fitWeaponItem = MenuItemLabel::create(fitWeaponLabel,CC_CALLBACK_0(stationScene::chickFitWeaponWindow,this));
	fitWeaponLabel->setColor(Color3B(14, 184, 255)); //��ɫ
	//����ͼ
	auto openStarMapLabel = Label::createWithTTF(dic->getStringForKey("openStarMap"), "fonts//msyh.ttf", 20);
	auto openStarMapItem = MenuItemLabel::create(openStarMapLabel, CC_CALLBACK_0(stationScene::chickStarMapWindow, this));
	openStarMapLabel->setColor(Color3B(14, 184, 255)); //��ɫ
	//�浵
	auto openSaveLabel = Label::createWithTTF(dic->getStringForKey("openSave"), "fonts//msyh.ttf", 20);
	auto openSaveItem = MenuItemLabel::create(openSaveLabel, CC_CALLBACK_0(stationScene::chickSaveWindow, this));
	openSaveLabel->setColor(Color3B(14, 184, 255)); //��ɫ
	//ָ��λ��
	fitShipItem->setAnchorPoint(cocos2d::Vec2::ZERO);
	fitWeaponItem->setAnchorPoint(cocos2d::Vec2::ZERO);
	openStarMapItem->setAnchorPoint(cocos2d::Vec2::ZERO);
	openSaveItem->setAnchorPoint(cocos2d::Vec2::ZERO);
	fitShipItem->setPosition(10, 768-242);
	fitWeaponItem->setPosition(10, 768 - 282);
	openStarMapItem->setPosition(10, 768 - 322);
	openSaveItem->setPosition(10, 768 - 362);
	
	//��ӵ��˵�
	auto menu = Menu::create();
	menu->addChild(fitShipItem);
	menu->addChild(fitWeaponItem);
	menu->addChild(openStarMapItem);
	menu->addChild(openSaveItem);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(0,0);
	
	//�����������
	this->addChild(stationBackground);
	this->addChild(leftBoard);
	this->addChild(planetLabel);
	this->addChild(menu);
}

void game::stationScene::chickFitWeaponWindow()
{
	
	if (fitWeapon == nullptr) {
		this->closeAll(); //�����,�ٴ�
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
		this->closeAll(); //�����,�ٴ�
		cocos2d::log("INFO: create fit ship window");
		//����shipData
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
	//ֱ����manager���������һ��
	game::SceneManager::getInstance()->stationSceneToFightScene();
}

void game::stationScene::chickSaveWindow()
{
}

void game::stationScene::closeAll() //�ر����д���
{
	if (fitWeapon != nullptr) chickFitWeaponWindow();
	if (fitShip != nullptr) chickFitShipWindow();
}
