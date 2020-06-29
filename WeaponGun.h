#ifndef __WEAPONGUN_H__
#define __WEAPONGUN_H__

#include "HelloWorldScene.h"
#include "Weapon.h"
#include "cocos2d.h"
#include "Data.h"

namespace game {
	class WeaponGun : public Weapon
	{
	public:
		//gun特性
		int rpm;
		//函数
		bool loadWeapon(Data * pConfig, Data* pData, ally wAlly, cocos2d::Vec2 windowSize); //重载父函数
		void activate(HelloWorld *scene); //激活
		//注册回调函数
		void gunCallback(float);
		//更新类型
		CREATE_FUNC(WeaponGun);
	private:
		//自身拥有子弹的参数设置
		Data* pBulletData;
		Data* pBulletConfig;
	};
}
#endif __WEAPONGUN_H__ //__WEAPONGUN_H__