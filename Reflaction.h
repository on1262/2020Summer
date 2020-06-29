#ifndef __REFLACTION_H__
#define __REFLACTION_H__

/*���ͷ�ļ������˷���ķ��������г�����gameData�е��඼��Ҫ������ע��*/
//Fighter����
#include "Fighter.h"
#include "PlayerFighter.h"
#include "EnemyAA.h"
//Weapon����
#include "Weapon.h"
#include "WeaponBullet.h"
#include "WeaponGun.h"
#include "cocos2d.h"
#include <string>
using namespace game;

static Weapon* wReflect(std::string name) {
	if (name == std::string("bullet")) return WeaponBullet::create();
	if (name == std::string("gun")) return WeaponGun::create();
	//����Ҳ����κδ���
	cocos2d::log("Warning: Undefined reflaction type.Return nullptr. Please add it to Reflaction.h");
	return nullptr;
}

static Fighter* fReflect(std::string name) {
	if (name == std::string("player")) return PlayerFighter::create();
	if (name == std::string("enemyAA")) return EnemyAA::create();
	//����Ҳ����κδ���
	cocos2d::log("Warning: Undefined reflaction type.Return nullptr. Please add it to Reflaction.h");
	return nullptr;
}
#endif __REFLACTION_H__ // __REFLACTION_H__
