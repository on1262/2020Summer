#include "WeaponBullet.h"
#include "util.h"

bool game::WeaponBullet::loadWeapon(Data * pConfig, Data * pData , ally wAlly, cocos2d::Vec2 windowSize)
{
	Weapon::loadWeapon(pConfig, pData, wAlly, windowSize); //父函数完成通用部分继承
	try {
		//先获得武器增益后的属性值
		auto attrs = util::getFixedAttr(pData);
		for (auto j = attrs.begin(); j != attrs.end(); j++) {
			if (j->attrName == "bulletSpeed") {
				this->bulletSpeed = j->attrValue;
				continue;
			}
			if (j->attrName == "damage") {
				this->damage = j->attrValue;
				continue;
			}
		}
		return true;
	}
	catch (...) {
		cocos2d::log("Warning:Nullptr in loading WeaponBullet");
	}
	return false;
}

void game::WeaponBullet::onEnter()
{
	Weapon::onEnter();
}

void game::WeaponBullet::activate(FightScene *scene)
{
	Weapon::activate(scene); //调用父函数
	//这里要将子弹添加到碰撞判定中
	float distance;
	if (this->weaponAlly == ally::player) {
		//添加碰撞判定
		scene->setPlayerBullets(FightScene::setFlag::reigster, this);
		//设置子弹前进方向
		distance = this->windowSize.y;

	}
	else if (this->weaponAlly == ally::enemy) {
		//添加碰撞判定
		scene->setEnemyBullets(FightScene::setFlag::reigster, this);
		//设置子弹前进方向
		distance = -1.0 * this->windowSize.y; //敌人的子弹方向要相反~

	}
	//设置回调:出界以后就消失
	this->runAction(
		cocos2d::Sequence::create(
			cocos2d::MoveBy::create(10 / bulletSpeed, cocos2d::Vec2(0, distance)),
			cocos2d::CallFunc::create(CC_CALLBACK_0(WeaponBullet::destroy,this)),
			0
		)
	);
}

void game::WeaponBullet::destroy()
{
	if (this->pScene == nullptr) cocos2d::log("Error: destroyfighter with pScene=nullptr");
	//取消碰撞检测
	if (this->weaponAlly == ally::player) {
		bool result = this->pScene->setPlayerBullets(FightScene::setFlag::cancel, this);
		if (result == false) {
			cocos2d::log("Warning: error in canceling collision detection for player bullet");
		}
	}
	this->removeFromParent();
}
