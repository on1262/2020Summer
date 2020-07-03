
#include "cocos2d.h"
#include "DataLoader.h"
#include <string>
#include <vector>
#include "cocos/ui/CocosGUI.h"
#include "dropIcon.h"
namespace game {
	class SettlementWindow :
		public cocos2d::Sprite
	{
	public:
		struct initData {
			std::string levelLabel;
			DataLoader* gameDL;
			DataLoader* saveDL;
			std::vector<dropItem> vec;
			int destroyEnemyCount;
			bool isPlayerDestroyed;//是通关还是坠亡
		};
		//成员
		initData data;
		cocos2d::ui::ScrollView* leftView; //存放宝石
		cocos2d::ui::ScrollView* rightView; //存放武器
		SettlementWindow* create(initData init); //初始化
	};
}

