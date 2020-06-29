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
	//��ʼ������״̬
	isFitWeaponOpened = false;
	isFitShipOpened = false;
	isStorageOpened = false;
	isStarMapOpened = false;
	isSaveOpened = false;

	this->isLoaded = false; //����
	this->pGameDataLoader = nullptr;
	this->pUserDataLoader = nullptr;
	return Scene::init(); //ִ�и�����
}

void stationScene::onEnter()
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
	auto fitWeaponLabel = Label::createWithTTF(dic->getStringForKey("fitWeapon"), "fonts//msyh.ttf", 20);
	auto test = MenuItemLabel::create(fitWeaponLabel,CC_CALLBACK_0(stationScene::chickFitWeaponWindow,this));
	fitWeaponLabel->setColor(Color3B(14, 184, 255)); //��ɫ
	auto menu = Menu::create();
	menu->addChild(test);
	test->setAnchorPoint(Vec2::ZERO);
	menu->setAnchorPoint(Vec2::ZERO);
	menu->setPosition(11, 768 - 246);
	
	//�����������
	this->addChild(stationBackground);
	this->addChild(leftBoard);
	this->addChild(planetLabel);
	this->addChild(menu);
}

void stationScene::chickFitWeaponWindow()
{
	if (isFitWeaponOpened) {

	}
	cocos2d::log("Chicked!");
	//Director::getInstance()->end();
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

void stationScene::closeAll() //�ر����д���
{
	if (isFitWeaponOpened) chickFitShipWindow();
	if (isFitShipOpened) chickFitShipWindow();
	if (isStorageOpened) chickStorageWindow();
	if (isStarMapOpened) chickSaveWindow();
	if (isSaveOpened) chickSaveWindow();
}
