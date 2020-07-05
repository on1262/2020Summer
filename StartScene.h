#ifndef __STARTSCENE_H__
#define __STARTSCENE_H__

#include "cocos2d.h"
//#include "cocos\ui\CocosGUI.h"

/*开始界面*/
namespace game {
	class StartScene : public cocos2d::Scene
	{
	public:
		static StartScene* create();
		//4个按钮的回调函数
		void startGameCallback();
		void saveCallback();
		void aboutCallback();
		void exitCallback();
	};
}

#endif __STARTSCENE_H__// !__STARTSCENE_H__


