
#ifndef __PLAYERFIGHTER_H__
#define __PLAYERFIGHTER_H__
#include "cocos2d.h"
#include "Fighter.h"
namespace game {
	/*注意，Fighter是从模版中创建一个全新的战机，而不是用来保存和读取游戏的实时状态
	Weapon的类型是由config中的数据决定的，但是weapon对象是由user的游戏数据决定的*/
	class PlayerFighter : public Fighter
	{
	public:
		//player是拥有防护盾的设定
		int shield;
		//用于给scene的监听
		cocos2d::EventListenerKeyboard* listener;
		CREATE_FUNC(PlayerFighter);
		virtual void onEnter(); //初始化
		void PlayerFighter::fighterStopCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
		void PlayerFighter::fighterMoveCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
		//添加碰撞的敌我识别
		void setAutoFire(FightScene *scene);
		//重写伤害和爆炸机制
		void getDamage(float damage) override;
		void destroyCallback() override;
		//重写停机机制
		void shutdown() override;
	};
}
#endif __PLAYERFIGHTER_H__ //__PLAYERFIGHTER_H__
