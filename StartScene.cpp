#include "StartScene.h"
#include "LanguageManager.h"
#include "SceneManager.h"
game::StartScene * game::StartScene::create()
{
	StartScene *ss = new (std::nothrow) StartScene();
	if (ss && ss->init())
	{
		ss->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ss);
		return nullptr;
	}
	//添加背景图片
	auto background = cocos2d::Sprite::create("background//homePage.png");
	background->setAnchorPoint(cocos2d::Vec2::ZERO);
	background->setPosition(0, 0);
	ss->addChild(background);
	auto lang = LanguageManager::getInstance();
	//主菜单
	auto menu = cocos2d::Menu::create();
	auto startGameLabel = cocos2d::Label::create(lang->getStringForKey("startGame"), "fonts//msyh.ttf", 24);
	auto openSaveLabel = cocos2d::Label::create(lang->getStringForKey("openSave"), "fonts//msyh.ttf", 24);
	auto aboutLabel = cocos2d::Label::create(lang->getStringForKey("aboutPage"), "fonts//msyh.ttf", 24);
	auto exitLabel = cocos2d::Label::create(lang->getStringForKey("exit"), "fonts//msyh.ttf", 24);
	startGameLabel->setColor(cocos2d::Color3B(14, 184, 255));
	openSaveLabel->setColor(cocos2d::Color3B(14, 184, 255));
	aboutLabel->setColor(cocos2d::Color3B(14, 184, 255));
	exitLabel->setColor(cocos2d::Color3B(14, 184, 255));
	//menuItem
	auto item1 = cocos2d::MenuItemLabel::create(startGameLabel,CC_CALLBACK_0(StartScene::startGameCallback, ss));
	auto item2 = cocos2d::MenuItemLabel::create(openSaveLabel,CC_CALLBACK_0(StartScene::saveCallback, ss));
	auto item3 = cocos2d::MenuItemLabel::create(aboutLabel,CC_CALLBACK_0(StartScene::aboutCallback, ss));
	auto item4 = cocos2d::MenuItemLabel::create(exitLabel,CC_CALLBACK_0(StartScene::exitCallback, ss));
	//设置位置
	
	item1->setPosition(512, 768 - 370);
	item2->setPosition(512, 768 - 414);
	item3->setPosition(512, 768 - 455);
	item4->setPosition(512, 768 - 498);
	
	//menu
	menu->alignItemsVertically();
	menu->setPosition(0,0);
	menu->addChild(item1);
	menu->addChild(item2);
	menu->addChild(item3);
	menu->addChild(item4);
	//添加到scene
	ss->addChild(menu);
	return ss;
}

void game::StartScene::startGameCallback()
{
	SceneManager::getInstance()->startSceneToStationScene();
}

void game::StartScene::saveCallback()
{
	//暂时没有save的选项~
}

void game::StartScene::aboutCallback()
{
	//暂时没有做about的选项
}

void game::StartScene::exitCallback()
{
	//退出游戏
	cocos2d::Director::getInstance()->end();
}
