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
#include "WeaponBullet.h"
#include "DataLoader.h"
#include "Data.h"
#include <vector>
#include "PlanetGenerator.h"
#include "PlayerFighter.h"
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

void game::FightScene::setDataLoader(DataLoader * pGame, DataLoader * pUser)
{
	this->pGameDataLoader = pGame;
	this->pUserDataLoader = pUser;
}

float game::FightScene::getPlayerHealth()
{
	if (this->playerFighter != nullptr) return this->playerFighter->health;
	else return 0.0f;
}

float game::FightScene::getPlayerShield()
{
	if(this->playerFighter == nullptr) return 0.0f;
	auto pf = dynamic_cast<PlayerFighter*>(playerFighter);
	if (pf != nullptr) return pf->shield;
	return 0.0f;
}

int game::FightScene::getEnemyFighterCount()
{
	//用碰撞判定的数据作为敌机数量
	return this->activatedEnemyFighters->size();
}

int game::FightScene::getDestroyedEnemyCount()
{
	return this->destoryedEnemyCount;
}

void game::FightScene::addDestroyedEnemyCount(int num)
{
	this->destoryedEnemyCount += num;
}


void game::FightScene::gameFinished(bool isPlayerDestroyed)
{
	//注销飞机
	this->playerFighter == nullptr;
	fightWindow->unscheduleUpdate();//取消更新
	//生成一个新的玩家飞机
	auto pf = PlayerFighter::create();
	pf->loadFighter(pGameDataLoader, pUserDataLoader, 1, game::ally::player, Vec2(1024, 768));
	pf->shutdown();
	//向level注销生成飞机
	this->level->setFinishedFlag(true);
	//取消碰撞检测
	this->isCollisionDetectionOpen = false;
	SettlementWindow::initData init{
		level->getLevelData(),
		pf,
		pGameDataLoader,
		pUserDataLoader,
		*(this->level->getSpoils()),
		this->destoryedEnemyCount,
		this->getGemCount,
		0, //这里要改
		this->getWeaponCount,
		isPlayerDestroyed
	};
	this->settlementWindow = SettlementWindow::create(init);
	this->addChild(settlementWindow);
	this->settlementWindow->setZOrder(11);
}

bool game::FightScene::detectCollision(Node* node1, Node* node2)
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

bool game::FightScene::detectCollision(Node * node1, Vec2 node2pos)
{
	if (node1 == nullptr) {
		cocos2d::log("Warning: call detectCollision with nullptr");
		return false;
	}
	return node1->getBoundingBox().containsPoint(node2pos);
}

void game::FightScene::EnemyFighterWithPlayerCollision()
{
	//检测enemy是否和player碰撞
	for (auto ef = this->activatedEnemyFighters->begin(); ef != activatedEnemyFighters->end(); ef++) {
		if (this->detectCollision(playerFighter, *ef) == true) {
			//@test 这里只是为了方便检测
			this->playerFighter->getDamage(50);
			(*ef)->getDamage(110);
			break; //同一时间只检测一个fighter即可
		}
	}
}

void game::FightScene::EnemyBulletWithPlayerCollision()
{
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
}

void game::FightScene::EnemyWithPlayerBulletCollision()
{
	//检测playerBullet是否和enemy碰撞
	for (auto pb = this->activatedPlayerBullets->begin(); pb != activatedPlayerBullets->end(); pb++) {
		for (auto e = this->activatedEnemyFighters->begin(); e != activatedEnemyFighters->end(); e++) {
			if (this->detectCollision(*e, *pb) == true) {
				//强制类型转换
				try {
					game::WeaponBullet* playerbullet = dynamic_cast<game::WeaponBullet*>(*pb);
					playerbullet->destroy(); //子弹没有生命值,故直接销毁
					(*e)->getDamage(110); //@test 测试用数据
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

void game::FightScene::dropIconWithPlayerCollision()
{

	auto iter = this->activatedDropIcons->end();
	//检测player是否和dropItem碰撞
	for (auto di = this->activatedDropIcons->begin(); di != this->activatedDropIcons->end(); di++) {
		if (this->detectCollision(playerFighter, (*di)->getPosition()) == true) {
			//将icon的struct添加到战利品中
			this->level->addDropItem((*di)->_dropItem);
			//添加信息到面板上
			if ((*di)->_dropItem._dropType == dropType::gemType) this->getGemCount++;
			if ((*di)->_dropItem._dropType == dropType::weaponType) this->getWeaponCount++;
			//注销碰撞检测
			iter = di;
			//销毁icon
			(*di)->destroy();
			break;
		}
	}
	if (iter != this->activatedDropIcons->end()) this->activatedDropIcons->erase(iter);
}


//这里应该可以用泛型？之后再重构
bool game::FightScene::setEnemyFighters(setFlag f, game::Fighter * e)
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

bool game::FightScene::setEnemyBullets(setFlag f, game::Weapon * eb)
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

bool game::FightScene::setPlayerBullets(setFlag f, game::Weapon * pb)
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

bool game::FightScene::setIcons(setFlag f, game::dropIcon * di)
{
	if (f == setFlag::reigster) {
		//这里少了重复icon的检测
		//TODO: 重复性检测
		this->activatedDropIcons->push_back(di);
		return true;
	} 
	else if (f == setFlag::cancel) {
		auto iter = this->activatedDropIcons->begin();
		bool isFound = false;
		for (auto i = this->activatedDropIcons->begin(); i != this->activatedDropIcons->end(); i++) {
			if ((*i) == di) {
				iter = i;
				isFound = true;
				break;
			}
		}
		this->activatedDropIcons->erase(iter);
		if (isFound = false) cocos2d::log("Warning: can not find icons in deleting iter of vector");
		return isFound;
	}
	return false;
}

void game::FightScene::update(float delta)
{
	if (this->isCollisionDetectionOpen == false) {
		//注销自身
		this->unscheduleUpdate();
		return;
	}
	if (this->playerFighter == nullptr) { //如果玩家不存在，就不进行碰撞检测（死后的瞬间？）
		return;
	}
	//碰撞检测模块
	EnemyFighterWithPlayerCollision();
	EnemyBulletWithPlayerCollision();
	EnemyWithPlayerBulletCollision();
	dropIconWithPlayerCollision();
	
}

/*以下都是有关碰撞池的签入和注销*/

Scene* game::FightScene::createScene()
{
	return FightScene::create(1);
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in FightScene.cpp\n");
}

// on "init" you need to initialize your instance
bool game::FightScene::init()
{

	if (!Scene::init())
	{
		return false;
	}

	//窗口大小默认1024*768, 所有的UI，背景都按照这一大小设计，不能更改
	//碰撞检测池初始化
	this->activatedPlayerBullets = new std::vector<game::Weapon*>;
	this->activatedEnemyFighters = new std::vector<game::Fighter*>;
	this->activatedEnemyBullets = new std::vector<game::Weapon*>;
	this->activatedDropIcons = new std::vector<game::dropIcon*>;
	//初始化一些其他参数,避免调试的时候出错
	this->level = nullptr;
	this->playerFighter = nullptr;
	this->fightWindow = nullptr;
	this->settlementWindow = nullptr;
	this->getGemCount = 0;
	this->destoryedEnemyCount = 0;
	this->getWeaponCount = 0;
	return true;
}

void game::FightScene::onEnter()
{
	cocos2d::log("INFO: fightScene onEnter()");
	Scene::onEnter();
	auto visibleSize = Director::getInstance()->getVisibleSize();
	Vec2 origin = Director::getInstance()->getVisibleOrigin();
	//设定测试场景：以下部分仅为测试用
	Vec2 windowSize = Vec2(visibleSize.width, visibleSize.height);
	//添加玩家飞机
	auto player = PlayerFighter::create();
	this->playerFighter = player;
	player->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	player->loadFighter(pGameDataLoader, pUserDataLoader, 1, game::ally::player, Vec2(visibleSize.width, visibleSize.height));
	this->addChild(player);
	player->setAutoFire(this); //这个一定要在最后调用
	cocos2d::log("player loaded");
	//添加关卡生成器
	auto levelController = game::Level::create();
	levelController->loadLevel(pUserDataLoader, pGameDataLoader, levelID , 2, windowSize);
	levelController->setTag(2000);
	this->addChild(levelController);
	this->level = levelController;
	levelController->activate(this); //启动关卡生成器
	log("Scene loading: levelController activated");
	//开启碰撞检测，这里用一个参数表示
	this->isCollisionDetectionOpen = true;
	this->scheduleUpdate(); //每帧调用碰撞检测
	log("Scene loading: collision detection activated.");
	/*只在游戏初始化时生成*/
	PlanetGenerator::getInstance()->setRepeatedPlainMap(false, 1,this, -20, cocos2d::Size(1024, 768), 4, 2048, 100, 1);
	//pg.setMapSize(192*4 ,256);
	//pg.generatePlainMap("plainMap_2", 4, 2);
	/* 这一段是关于显示星球动画的，放在starMap里实现
	game::DataLoader::loadAnimation("globalAnim", "planetGenerator//", "_global_1", 100, 7,-1);
	game::DataLoader::loadAnimation("globalAnimCloud", "planetGenerator//", "_global_1_cloud",100,8,-1);
	Sprite* planet = game::DataLoader::runAnimationByName("globalAnim",10);
	Sprite* planetCloud = game::DataLoader::runAnimationByName("globalAnimCloud", 11);
	planetCloud->addChild(planet,-1);
	planetCloud->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	planet->setNormalizedPosition(cocos2d::Vec2(0.5,0.5)); //云彩在中间显示
	this->addChild(planetCloud);
	*/
	/*添加控制面板*/
	this->fightWindow = game::FightWindow::create(this);
	this->fightWindow->setAnchorPoint(cocos2d::Vec2::ZERO);
	this->fightWindow->setPosition(0,50);
	this->fightWindow->setLocalZOrder(100);
	this->addChild(fightWindow);
}

game::FightScene* game::FightScene::create(int levelID)
{
	FightScene *ret = new (std::nothrow) FightScene();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
		return nullptr;
	}
	ret->levelID = levelID;
	return ret;
}

