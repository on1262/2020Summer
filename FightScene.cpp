/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#include "FightScene.h"

#include "SimpleAudioEngine.h"
 /*下面这部分是移植到新scene需要的*/
#include "cocos2d.h"
#include "PlayerFighter.h"
#include "Fighter.h"
#include "Weapon.h"
#include "WeaponBullet.h"
#include "DataLoader.h"
#include "Data.h"
#include "Level.h"
#include <vector>
#include "PlanetGenerator.h"
/*********************************/

USING_NS_CC;

/*tag数说明
*0-1000是系统可能自动分配的tag
*1000是player的tag
*2000是关卡管理器level的tag
*2001-2100是UItag
*3000以后是敌机的tag
*20000以后是奖励物的tag
*导弹、火箭等是没有分配tag的
*任何友好物的tag都会是1000/2000/2001-2100/20000以后
**********/

bool FightScene::detectCollision(Node* node1, Node* node2)
{
	if (node1 == nullptr || node2 == nullptr) { //如果找不到player的飞机
		log("Info: Warning: use nullptr to detect collision.");
		return false;
	}
	CCRect node1Rect = node1->getBoundingBox();
	CCRect fixedNode1Rect;
	fixedNode1Rect.setRect(node1Rect.getMinX(), node1Rect.getMidY() - 0.1*node1Rect.size.height, 0.9*node1Rect.size.width, 0.2*node1Rect.size.height); //设置一个较小的矩形
	CCRect node2Rect = node2->getBoundingBox();
	CCRect fixedNode2Rect;
	fixedNode2Rect.setRect(node2Rect.getMinX(), node2Rect.getMidY() - 0.1*node2Rect.size.height, 0.9*node2Rect.size.width, 0.2*node2Rect.size.height); //设置一个较小的矩形
	if (fixedNode1Rect.intersectsRect(fixedNode2Rect)) return true;
	return false;
}
//这里应该可以用泛型？之后再重构
bool FightScene::setEnemyFighters(setFlag f, game::Fighter * e)
{
	if (f == setFlag::reigster) {
		for (auto i = this->activatedEnemyFighters->begin(); i != activatedEnemyFighters->end(); i++) {
			if (*i == e) { //如果已经存在了，就禁止注册
				log("Warning: register the same pointer of an enemy fighter.");
				return false;
			}
		}
		this->activatedEnemyFighters->push_back(e);
		return true;
	}
	else if (f == setFlag::cancel) {
		for (auto i = this->activatedEnemyFighters->begin(); i != activatedEnemyFighters->end(); i++) {
			if (*i == e) { //如果检测到，就销毁
				activatedEnemyFighters->erase(i);
				return true;
			}
		}
		log("Warning: cancel a nonexistent pointer of an enemy fighter.");
		return false;
	}
	return false;
}

bool FightScene::setEnemyBullets(setFlag f, game::Weapon * eb)
{
	if (f == setFlag::reigster) {
		for (auto i = this->activatedEnemyBullets->begin(); i != activatedEnemyBullets->end(); i++) {
			if (*i == eb) { //如果已经存在了，就禁止注册
				log("Warning: register the same pointer of an enemy bullets.");
				return false;
			}
		}
		this->activatedEnemyBullets->push_back(eb);
		return true;
	}
	else if (f == setFlag::cancel) {
		for (auto i = this->activatedEnemyBullets->begin(); i != activatedEnemyBullets->end(); i++) {
			if (*i == eb) { //如果检测到，就销毁
				activatedEnemyBullets->erase(i);
				return true;
			}
		}
		log("Warning: cancel a nonexistent pointer of an enemy bullet.");
		return false;
	}
	return false;
}

bool FightScene::setPlayerBullets(setFlag f, game::Weapon * pb)
{
	if (f == setFlag::reigster) {
		for (auto i = this->activatedPlayerBullets->begin(); i != activatedPlayerBullets->end(); i++) {
			if (*i == pb) { //如果已经存在了，就禁止注册
				log("Warning: register the same pointer of an enemy bullets.");
				return false;
			}
		}
		this->activatedPlayerBullets->push_back(pb);
		return true;
	}
	else if (f == setFlag::cancel) {
		for (auto i = this->activatedPlayerBullets->begin(); i != activatedPlayerBullets->end(); i++) {
			if (*i == pb) { //如果检测到，就销毁
				activatedPlayerBullets->erase(i);
				return true;
			}
		}
		log("Warning: cancel a nonexistent pointer of an player bullet.");
		return false;
	}
	return false;
}

void FightScene::update(float delta)
{
	if (this->isCollisionDetectionOpen == false) {
		log("Info: Collision detection closed.");
		return;
	}
	if (this->playerFighter == nullptr) { //如果玩家不存在，就不进行碰撞检测（死后的瞬间？）
		return;
	}
	//检测enemy是否和player碰撞
	for (auto ef = this->activatedEnemyFighters->begin(); ef != activatedEnemyFighters->end(); ef++) {
		if (this->detectCollision(playerFighter, *ef) == true) {
			//this->playerFighter->getDamage(0);
			(*ef)->getDamage(110);
			break; //同一时间只检测一个fighter即可
		}
	}
	//检测enemyBullet是否和player碰撞
	for (auto eb = this->activatedEnemyBullets->begin(); eb != activatedEnemyBullets->end(); eb++) {
		if (this->detectCollision(playerFighter, *eb) == true) {
			//强制类型转换
			try {
				game::WeaponBullet* pb = dynamic_cast<game::WeaponBullet*>(*eb);
				playerFighter->getDamage(pb->damage); //获得敌子弹的伤害
				log(("player get damage, health=" + std::to_string(playerFighter->health)).c_str());
			}
			catch (...) {
				log("Error: can not get WeaponBullet* by dynamic_cast.");
			}
			break;
		}
	}
	//检测playerBullet是否和enemy碰撞
	for (auto pb = this->activatedPlayerBullets->begin(); pb != activatedPlayerBullets->end(); pb++) {
		for (auto e = this->activatedEnemyFighters->begin(); e != activatedEnemyFighters->end(); e++) {
			if (this->detectCollision(*e, *pb) == true) {
				//强制类型转换
				try {
					game::WeaponBullet* playerbullet = dynamic_cast<game::WeaponBullet*>(*pb);
					playerbullet->destroy(); //子弹销毁
					(*e)->destory();//飞机销毁
					return;
				}
				catch (...) {
					log("Error: can not get WeaponBullet* by dynamic_cast.");
				}
				break;
			}
		}
	}
}

/*以下都是有关碰撞池的签入和注销*/

Scene* FightScene::createScene()
{
	return FightScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in FightScene.cpp\n");
}

// on "init" you need to initialize your instance
bool FightScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}

	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();

	/////////////////////////////
	// 2. add a menu item with "X" image, which is clicked to quit the program
	//    you may modify it.

	// add a "close" icon to exit the progress. it's an autorelease object
	auto closeItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png",
		CC_CALLBACK_1(FightScene::menuCloseCallback, this));

	if (closeItem == nullptr ||
		closeItem->getContentSize().width <= 0 ||
		closeItem->getContentSize().height <= 0)
	{
		problemLoading("'CloseNormal.png' and 'CloseSelected.png'");
	}
	else
	{
		float x = origin.x + visibleSize.width - closeItem->getContentSize().width / 2;
		float y = origin.y + closeItem->getContentSize().height / 2;
		closeItem->setPosition(Vec2(x, y));
	}

	// create menu, it's an autorelease object
	auto menu = Menu::create(closeItem, NULL);
	menu->setPosition(Vec2::ZERO);
	this->addChild(menu, 1);

	//文件存取器初始化:userDataLoader和gameDataLoader
	this->pGameDataLoader = new game::DataLoader(std::string("gameData//"), std::string("gameData.txt"));
	this->pUserDataLoader = new game::DataLoader(std::string("save//"), std::string("save_1.txt"));
	//碰撞检测池初始化
	this->activatedPlayerBullets = new std::vector<game::Weapon*>;
	this->activatedEnemyFighters = new std::vector<game::Fighter*>;
	this->activatedEnemyBullets = new std::vector<game::Weapon*>;

	return true;
}

void FightScene::onEnter()
{
	Scene::onEnter();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//设定测试场景：以下部分仅为测试用
	Vec2 windowSize = Vec2(visibleSize.width, visibleSize.height);
	//添加飞机
	auto player = game::PlayerFighter::create();
	this->playerFighter = player;
	player->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	player->loadFighter(pGameDataLoader, pUserDataLoader, 1, game::ally::player, Vec2(visibleSize.width, visibleSize.height));
	//this->addChild(player);
	//player->setAutoFire(this); //这个一定要在最后调用
	//添加关卡生成器
	auto levelController = game::Level::create();
	levelController->loadLevel(pUserDataLoader, pGameDataLoader, 1, 2, windowSize);
	levelController->setTag(2000);
	this->addChild(levelController);
	//levelController->activate(this); //启动关卡生成器
	log("Scene loading: levelController activated");
	//开启碰撞检测，这里用一个参数表示
	this->isCollisionDetectionOpen = true;
	//this->scheduleUpdate(); //每帧调用碰撞检测
	log("Scene loading: collision detection activated.");
	/*调试代码：行星动画生成器*/
	PlanetGenerator pg;
	/*只在游戏初始化时生成，*/
	for (int i = 1; i < 2; i++) {
		//pg.generateHeightMap(("_global_" + std::to_string(i)),60,1,100,100,2,i + 2);
	}
	pg.setMapSize(192*4 ,256);
	//pg.generatePlainMap("plainMap_2", 4, 2);
	/*调试代码，添加特定的动画到scene中*/
	Sprite* background = cocos2d::Sprite::create("plainMap//plainMap_1.png");
	this->addChild(background);
	background->setPosition(Vec2(512,768*2));
	background->runAction(MoveBy::create(40, Vec3(0, -768 * 8, 0)));
	Sprite* background2 = Sprite::create("plainMap//plainMap_2.png");
	this->addChild(background2);
	background2->setPosition(Vec2(512, 768*6));
	background2->runAction(MoveBy::create(40, Vec3(0, -768 * 8, 0)));
	/*
	game::DataLoader::loadAnimation("globalAnim", "planetGenerator//", "_global_1", 100, 7,-1);
	game::DataLoader::loadAnimation("globalAnimCloud", "planetGenerator//", "_global_1_cloud",100,8,-1);
	Sprite* planet = game::DataLoader::runAnimationByName("globalAnim",10);
	Sprite* planetCloud = game::DataLoader::runAnimationByName("globalAnimCloud", 11);
	planetCloud->addChild(planet,-1);
	planetCloud->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	planet->setNormalizedPosition(cocos2d::Vec2(0.5,0.5)); //云彩在中间显示
	this->addChild(planetCloud);
	*/
}

void FightScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

}