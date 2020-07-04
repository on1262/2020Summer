/*!
	Weapon基类：提供武器与飞船的通信接口，信息读取规范和子弹实例销毁
	* 所有武器必须继承Weapon
	* 子弹与激光属于武器的子节点，当武器被销毁时，子弹和激光一并销毁
*/
#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "FightScene.h"
#include "cocos2d.h"
#include "Data.h"
#include <string>

namespace game {
	class FightScene;
	enum ally{player, enemy}; //敌我阵营判断，这个是为所有Fighter/player都认可的
	class Fighter; //前置声明
	class Weapon : public cocos2d::Sprite {
	public:
		//武器识别
		std::string weaponType; //用于检测是否可安放到插槽中
		//敌我识别
		ally weaponAlly;
		//武器状态
		bool isDamaged; //是否被损毁
		bool isFire; //是否处于开火状态

		//被添加到飞船上
		virtual void onEnter();
		//武器被添加前必须调用loadWeapon，用来初始化武器的配置,windowsSize用于判断武器是否超出边框，这个判定也是受配置影响的
		virtual bool loadWeapon(Data* pConfig,Data* pData, ally wAlly, cocos2d::Vec2 windowSize);
		//武器发射或开启伤害模式,重载时需要调用父函数
		virtual void activate(FightScene *scene);
		//武器停止发射
		virtual void shutDown();
		CREATE_FUNC(Weapon);
	protected:
		bool isLoaded; //是否在enter前调用过loadWeapon，如果没有，则输出错误信息
		Data* config; //保存自己的那份Data
		std::string texturePath;
		cocos2d::Vec2 windowSize;
		FightScene *pScene;//碰撞判定用，给子弹加的
	};

}

#endif __WEAPON_H__ //__WEAPON_H__