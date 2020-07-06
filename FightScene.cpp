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
 /*�����ⲿ������ֲ����scene��Ҫ��*/
#include "WeaponBullet.h"
#include "DataLoader.h"
#include "Data.h"
#include <vector>
#include "PlanetGenerator.h"
#include "PlayerFighter.h"
/*********************************/

USING_NS_CC;

/*tag��˵��
*0-1000��ϵͳ�����Զ������tag
*1000��player��tag
*2000�ǹؿ�������level��tag
*2001-2100��UItag
*3000�Ժ��ǵл���tag
*20000�Ժ��ǽ������tag
*�������������û�з���tag��
*�κ��Ѻ����tag������1000/2000/2001-2100/20000�Ժ�
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
	//����ײ�ж���������Ϊ�л�����
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
	//ע���ɻ�
	this->playerFighter == nullptr;
	fightWindow->unscheduleUpdate();//ȡ������
	//����һ���µ���ҷɻ�
	auto pf = PlayerFighter::create();
	pf->loadFighter(pGameDataLoader, pUserDataLoader, 1, game::ally::player, Vec2(1024, 768));
	pf->shutdown();
	//��levelע�����ɷɻ�
	this->level->setFinishedFlag(true);
	//ȡ����ײ���
	this->isCollisionDetectionOpen = false;
	SettlementWindow::initData init{
		level->getLevelData(),
		pf,
		pGameDataLoader,
		pUserDataLoader,
		*(this->level->getSpoils()),
		this->destoryedEnemyCount,
		this->getGemCount,
		0, //����Ҫ��
		this->getWeaponCount,
		isPlayerDestroyed
	};
	this->settlementWindow = SettlementWindow::create(init);
	this->addChild(settlementWindow);
	this->settlementWindow->setZOrder(11);
}

bool game::FightScene::detectCollision(Node* node1, Node* node2)
{
	if (node1 == nullptr || node2 == nullptr) { //����Ҳ���player�ķɻ�
		log("Info: Warning: use nullptr to detect collision.");
		return false;
	}
	CCRect node1Rect = node1->getBoundingBox();
	CCRect fixedNode1Rect;
	fixedNode1Rect.setRect(node1Rect.getMinX(), node1Rect.getMidY() - 0.1*node1Rect.size.height, 0.9*node1Rect.size.width, 0.2*node1Rect.size.height); //����һ����С�ľ���
	CCRect node2Rect = node2->getBoundingBox();
	CCRect fixedNode2Rect;
	fixedNode2Rect.setRect(node2Rect.getMinX(), node2Rect.getMidY() - 0.1*node2Rect.size.height, 0.9*node2Rect.size.width, 0.2*node2Rect.size.height); //����һ����С�ľ���
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
	//���enemy�Ƿ��player��ײ
	for (auto ef = this->activatedEnemyFighters->begin(); ef != activatedEnemyFighters->end(); ef++) {
		if (this->detectCollision(playerFighter, *ef) == true) {
			//@test ����ֻ��Ϊ�˷�����
			this->playerFighter->getDamage(50);
			(*ef)->getDamage(110);
			break; //ͬһʱ��ֻ���һ��fighter����
		}
	}
}

void game::FightScene::EnemyBulletWithPlayerCollision()
{
	//���enemyBullet�Ƿ��player��ײ
	for (auto eb = this->activatedEnemyBullets->begin(); eb != activatedEnemyBullets->end(); eb++) {
		if (this->detectCollision(playerFighter, *eb) == true) {
			//ǿ������ת��
			try {
				game::WeaponBullet* pb = dynamic_cast<game::WeaponBullet*>(*eb);
				playerFighter->getDamage(pb->damage); //��õ��ӵ����˺�
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
	//���playerBullet�Ƿ��enemy��ײ
	for (auto pb = this->activatedPlayerBullets->begin(); pb != activatedPlayerBullets->end(); pb++) {
		for (auto e = this->activatedEnemyFighters->begin(); e != activatedEnemyFighters->end(); e++) {
			if (this->detectCollision(*e, *pb) == true) {
				//ǿ������ת��
				try {
					game::WeaponBullet* playerbullet = dynamic_cast<game::WeaponBullet*>(*pb);
					playerbullet->destroy(); //�ӵ�û������ֵ,��ֱ������
					(*e)->getDamage(110); //@test ����������
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
	//���player�Ƿ��dropItem��ײ
	for (auto di = this->activatedDropIcons->begin(); di != this->activatedDropIcons->end(); di++) {
		if (this->detectCollision(playerFighter, (*di)->getPosition()) == true) {
			//��icon��struct��ӵ�ս��Ʒ��
			this->level->addDropItem((*di)->_dropItem);
			//�����Ϣ�������
			if ((*di)->_dropItem._dropType == dropType::gemType) this->getGemCount++;
			if ((*di)->_dropItem._dropType == dropType::weaponType) this->getWeaponCount++;
			//ע����ײ���
			iter = di;
			//����icon
			(*di)->destroy();
			break;
		}
	}
	if (iter != this->activatedDropIcons->end()) this->activatedDropIcons->erase(iter);
}


//����Ӧ�ÿ����÷��ͣ�֮�����ع�
bool game::FightScene::setEnemyFighters(setFlag f, game::Fighter * e)
{
	if (f == setFlag::reigster) {
		for (auto i = this->activatedEnemyFighters->begin(); i != activatedEnemyFighters->end(); i++) {
			if (*i == e) { //����Ѿ������ˣ��ͽ�ֹע��
				log("Warning: register the same pointer of an enemy fighter.");
				return false;
			}
		}
		this->activatedEnemyFighters->push_back(e);
		return true;
	}
	else if (f == setFlag::cancel) {
		for (auto i = this->activatedEnemyFighters->begin(); i != activatedEnemyFighters->end(); i++) {
			if (*i == e) { //�����⵽��������
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
			if (*i == eb) { //����Ѿ������ˣ��ͽ�ֹע��
				log("Warning: register the same pointer of an enemy bullets.");
				return false;
			}
		}
		this->activatedEnemyBullets->push_back(eb);
		return true;
	}
	else if (f == setFlag::cancel) {
		for (auto i = this->activatedEnemyBullets->begin(); i != activatedEnemyBullets->end(); i++) {
			if (*i == eb) { //�����⵽��������
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
			if (*i == pb) { //����Ѿ������ˣ��ͽ�ֹע��
				log("Warning: register the same pointer of an enemy bullets.");
				return false;
			}
		}
		this->activatedPlayerBullets->push_back(pb);
		return true;
	}
	else if (f == setFlag::cancel) {
		for (auto i = this->activatedPlayerBullets->begin(); i != activatedPlayerBullets->end(); i++) {
			if (*i == pb) { //�����⵽��������
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
		//���������ظ�icon�ļ��
		//TODO: �ظ��Լ��
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
		//ע������
		this->unscheduleUpdate();
		return;
	}
	if (this->playerFighter == nullptr) { //�����Ҳ����ڣ��Ͳ�������ײ��⣨�����˲�䣿��
		return;
	}
	//��ײ���ģ��
	EnemyFighterWithPlayerCollision();
	EnemyBulletWithPlayerCollision();
	EnemyWithPlayerBulletCollision();
	dropIconWithPlayerCollision();
	
}

/*���¶����й���ײ�ص�ǩ���ע��*/

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

	//���ڴ�СĬ��1024*768, ���е�UI��������������һ��С��ƣ����ܸ���
	//��ײ���س�ʼ��
	this->activatedPlayerBullets = new std::vector<game::Weapon*>;
	this->activatedEnemyFighters = new std::vector<game::Fighter*>;
	this->activatedEnemyBullets = new std::vector<game::Weapon*>;
	this->activatedDropIcons = new std::vector<game::dropIcon*>;
	//��ʼ��һЩ��������,������Ե�ʱ�����
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
	//�趨���Գ��������²��ֽ�Ϊ������
	Vec2 windowSize = Vec2(visibleSize.width, visibleSize.height);
	//�����ҷɻ�
	auto player = PlayerFighter::create();
	this->playerFighter = player;
	player->setPosition(Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y));
	player->loadFighter(pGameDataLoader, pUserDataLoader, 1, game::ally::player, Vec2(visibleSize.width, visibleSize.height));
	this->addChild(player);
	player->setAutoFire(this); //���һ��Ҫ��������
	cocos2d::log("player loaded");
	//��ӹؿ�������
	auto levelController = game::Level::create();
	levelController->loadLevel(pUserDataLoader, pGameDataLoader, levelID , 2, windowSize);
	levelController->setTag(2000);
	this->addChild(levelController);
	this->level = levelController;
	levelController->activate(this); //�����ؿ�������
	log("Scene loading: levelController activated");
	//������ײ��⣬������һ��������ʾ
	this->isCollisionDetectionOpen = true;
	this->scheduleUpdate(); //ÿ֡������ײ���
	log("Scene loading: collision detection activated.");
	/*ֻ����Ϸ��ʼ��ʱ����*/
	PlanetGenerator::getInstance()->setRepeatedPlainMap(false, 1,this, -20, cocos2d::Size(1024, 768), 4, 2048, 100, 1);
	//pg.setMapSize(192*4 ,256);
	//pg.generatePlainMap("plainMap_2", 4, 2);
	/* ��һ���ǹ�����ʾ���򶯻��ģ�����starMap��ʵ��
	game::DataLoader::loadAnimation("globalAnim", "planetGenerator//", "_global_1", 100, 7,-1);
	game::DataLoader::loadAnimation("globalAnimCloud", "planetGenerator//", "_global_1_cloud",100,8,-1);
	Sprite* planet = game::DataLoader::runAnimationByName("globalAnim",10);
	Sprite* planetCloud = game::DataLoader::runAnimationByName("globalAnimCloud", 11);
	planetCloud->addChild(planet,-1);
	planetCloud->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));
	planet->setNormalizedPosition(cocos2d::Vec2(0.5,0.5)); //�Ʋ����м���ʾ
	this->addChild(planetCloud);
	*/
	/*��ӿ������*/
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

