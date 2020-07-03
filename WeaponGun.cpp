#include "cocos2d.h"
#include "Data.h"
#include "WeaponGun.h"
#include "WeaponBullet.h"


bool game::WeaponGun::loadWeapon(Data* pConfig, Data* pData, ally wAlly, cocos2d::Vec2 windowSize)
{
	Weapon::loadWeapon(pConfig, pData,wAlly, windowSize); //重载父函数

	try {
		this->rpm = pData->valueConvert<int>(pData->findPairByKey(std::string("attr_rpm"))->value);
		//这部分应当为weaponGun设置一个bulletType，然后利用其寻址，这里直接指定了唯一的bullet
		this->pBulletConfig = pConfig->pLoader->findDataByLabel("bullet");
		this->pBulletData = pData->pLoader->findDataByLabel("weapon_2");
		return true;
	} catch (...) {
		cocos2d::log("Warning:Nullptr in loading WeaponGun");
	}
	return false;
}

void game::WeaponGun::activate(FightScene *scene) //activate一定要在创建节点树以后调用！
{
	Weapon::activate(scene);
	//武器不参与碰撞判定，所以这里不用加
	cocos2d::log("WeaponGun activated.");
	cocos2d::ccSchedulerFunc callback = std::bind(&game::WeaponGun::gunCallback, this, std::placeholders::_1);
	this->schedule(callback, 0.25,std::string("gunCallback")); //调度器，0.5秒调度自动开火
}

void game::WeaponGun::gunCallback(float)
{
	//如果玩家的飞机不存在，那么并不会创建任何东西
	cocos2d::Node* fighter = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(1000);
	if (fighter == nullptr) return;
	//创建一发子弹
	game::WeaponBullet* wb = game::WeaponBullet::create();
	wb->loadWeapon(pBulletConfig, pBulletData, weaponAlly, windowSize);
	cocos2d::Director::getInstance()->getRunningScene()->addChild(wb); //向scene添加场景
	//根据玩家现在的位置修正
	wb->setPosition(fighter->getPosition());
	//手动调用activate(). 因为要传入指针
	wb->activate(pScene);
	cocos2d::log("Created a instance of WeaponBullet.");
}
