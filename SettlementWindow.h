
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
			bool isPlayerDestroyed;//��ͨ�ػ���׹��
		};
		//��Ա
		initData data;
		cocos2d::ui::ScrollView* leftView; //��ű�ʯ
		cocos2d::ui::ScrollView* rightView; //�������
		SettlementWindow* create(initData init); //��ʼ��
	};
}

