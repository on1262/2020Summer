#ifndef __REFLACTION_H__
#define __REFLACTION_H__

/*这个头文件定义了反射的方法，所有出现在gameData中的类都需要在这里注册*/
//Fighter部分
#include "Fighter.h"
#include "PlayerFighter.h"
#include "EnemyAA.h"
//Weapon部分
#include "Weapon.h"
#include "WeaponBullet.h"
#include "WeaponGun.h"
#include "cocos2d.h"
#include <string>
using namespace game;

static Weapon* wReflect(std::string name) {
	if (name == std::string("bullet")) return WeaponBullet::create();
	if (name == std::string("gun")) return WeaponGun::create();
	//如果找不到任何代码
	cocos2d::log("Warning: Undefined reflaction type.Return nullptr. Please add it to Reflaction.h");
	return nullptr;
}

static Fighter* fReflect(std::string name) {
	if (name == std::string("player")) return PlayerFighter::create();
	if (name == std::string("enemyAA")) return EnemyAA::create();
	//如果找不到任何代码
	cocos2d::log("Warning: Undefined reflaction type.Return nullptr. Please add it to Reflaction.h");
	return nullptr;
}
#endif __REFLACTION_H__ // __REFLACTION_H__
