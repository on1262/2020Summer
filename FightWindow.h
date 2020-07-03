
#ifndef __FIGHTWINDOW_H__
#define __FIGHTWINDOW_H__
#include "cocos2d.h"
#include "Level.h"

namespace game {
	class FightWindow : public cocos2d::Sprite
	{
	public:
		//通信
		FightScene* _scene;
		static FightWindow* _window; //全局只能有一个window
		
		//面板显示
		cocos2d::Label* bottomLabel; //显示护盾和血量
		cocos2d::Label* upperLabel; //显示战场信息
		//提供数据交互,接口由_scene统一提供,
		void update(float f) override;
		//创建实例
		static FightWindow* create(FightScene* s); //调用一次
		static FightWindow* create(std::string fileName);
	};
}

#endif __FIGHTWINDOW_H__//__FIGHTWINDOW_H__
