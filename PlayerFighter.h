#ifndef __PLAYERFIGHTER_H__
#define __PLAYERFIGHTER_H__

#include "cocos2d.h"
#include "Weapon.h"
#include "Data.h"
#include "Fighter.h"
#include <vector>
#include <string>

namespace game {
	class Weapon; //前置声明

	/*注意，Fighter是从模版中创建一个全新的战机，而不是用来保存和读取游戏的实时状态
	Weapon的类型是由config中的数据决定的，但是weapon对象是由user的游戏数据决定的*/
	class PlayerFighter : public Fighter
	{
	public:
		CREATE_FUNC(PlayerFighter);
		virtual void onEnter(); //这里重写添加WASD控制
		void PlayerFighter::fighterStopCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
		void PlayerFighter::fighterMoveCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
		//添加碰撞的敌我识别
		void setAutoFire(HelloWorld *scene);
	};
}
#endif __PLAYERFIGHTER_H__ //__PLAYERFIGHTER_H__
