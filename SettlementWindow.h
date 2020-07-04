#ifndef __SETTLEMENTWINDOW_H__
#define __SETTLEMENTWINDOW_H__
#include "cocos2d.h"
#include "Data.h"
#include "DataLoader.h"
#include <string>
#include <vector>
#include "cocos/ui/CocosGUI.h"
#include "dropIcon.h"

namespace game {
	class SettlementGemPanel: public cocos2d::ui::Button
	{
	public:
		cocos2d::Label* label;
		static SettlementGemPanel* create(Data* gd, std::string ip);
		void buttonCallback(); //�������callback
	};
	class SettlementWeaponPanel : public cocos2d::ui::Button
	{
	public:
		cocos2d::Label* label;
		static SettlementWeaponPanel* create(Data* wd, std::string ip);
		void buttonCallback(); //�������callback
	};
	class SettlementWindow :
		public cocos2d::Sprite
	{
	public:
		struct initData {
			Data* levelData;
			cocos2d::Node* player; //������ʾ�ɴ�
			DataLoader* gameDL;
			DataLoader* saveDL;
			std::vector<dropItem> vec;
			int destroyEnemyCount;
			int getGemCount;
			int getMachinePartCount; //�����������(��ʱû���г�)
			int getWeaponCount;
			bool isPlayerDestroyed;//��ͨ�ػ���׹��
		};
		//��Ա
		initData data;
		/*UI����Ԫ��*/
		cocos2d::ui::ScrollView* leftView; //��ű�ʯ
		cocos2d::ui::ScrollView* rightView; //�������
		static SettlementWindow* create(initData init); //��ʼ��
		//���ڳ�ʼ���ĺ���
		void addGemToLeftView(); //֮ǰһ��Ҫ��leftView��data��ʼ��
		void addWeaponToRightView();
		void sortPanel(cocos2d::ui::ScrollView* sv); //���ڲ���panel�Զ�����
		//�ص�����
		void checkAllCallback(); //���ȫ��
		void returnCallback(); //�ص�ά�޲�
	};
}

#endif __SETTLEMENTWINDOW_H__// !__SETTLEMENTWINDOW_H__
