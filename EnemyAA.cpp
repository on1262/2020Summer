#include "cocos2d.h"
#include "EnemyAA.h"

USING_NS_CC;

void game::EnemyAA::setAutoFire(FightScene *scene)
{
	//调用父类方法(启动武器)
	Fighter::setAutoFire(scene);
	//添加碰撞判定：敌机
	scene->setEnemyFighters(FightScene::setFlag::reigster, this);
	//将自身旋转180度(因为是与player方向相反
	this->setRotation(180.0);
	//随机设置瀑布式掉落
	this->setPosition(getRandom(0, windowSize.x), windowSize.y);
	//添加一个从当前成掠过的动画~
	this->runAction(cocos2d::EaseIn::create(cocos2d::MoveBy::create(2, cocos2d::Vec2(0, -2*windowSize.y)),1.0));
}

void game::EnemyAA::getDamage(float damage)
{
	health -= damage;
	if (health <= 0) this->destroy();
}

void game::EnemyAA::destroyCallback()
{
	//爆出宝石和武器
	pScene->level->generateDropItem(2, this->getPosition());
}

float game::EnemyAA::getRandom(float start, float end) { //生成（start,end）开区间内的浮点数，如果想得到1-100值，将start设为1，将end设为100

	float i = start + (end - start)*rand() / (RAND_MAX + 1.0);
	return i;
}
