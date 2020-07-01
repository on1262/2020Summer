#ifndef __WEAPONBULLET_H__
#define __WEAPONBULLET_H__

#include "Weapon.h"
#include "cocos2d.h"
#include "Data.h"
#include "FightScene.h"
namespace game {

	class WeaponBullet : public Weapon
	{
	public:
		//bullet自身特性
		int bulletSpeed;
		int damage;
		//函数
		bool loadWeapon(Data * pConfig, Data* pData, ally wAlly, cocos2d::Vec2 windowSize); //重载父函数
		//重写onEnter设置动画
		void onEnter();
		//重写activate实现子弹的移动功能
		void activate(FightScene *scene);
		//自身摧毁
		void destroy();
		//重写create
		CREATE_FUNC(WeaponBullet);
	};
}

#endif __WEAPONBULLET_H__ //__WEAPONBULLET_H__