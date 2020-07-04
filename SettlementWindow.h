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
		void buttonCallback(); //点击面板的callback
	};
	class SettlementWeaponPanel : public cocos2d::ui::Button
	{
	public:
		cocos2d::Label* label;
		static SettlementWeaponPanel* create(Data* wd, std::string ip);
		void buttonCallback(); //点击面板的callback
	};
	class SettlementWindow :
		public cocos2d::Sprite
	{
	public:
		struct initData {
			Data* levelData;
			cocos2d::Node* player; //用于显示飞船
			DataLoader* gameDL;
			DataLoader* saveDL;
			std::vector<dropItem> vec;
			int destroyEnemyCount;
			int getGemCount;
			int getMachinePartCount; //回收零件数量(暂时没有列出)
			int getWeaponCount;
			bool isPlayerDestroyed;//是通关还是坠亡
		};
		//成员
		initData data;
		/*UI界面元素*/
		cocos2d::ui::ScrollView* leftView; //存放宝石
		cocos2d::ui::ScrollView* rightView; //存放武器
		static SettlementWindow* create(initData init); //初始化
		//用于初始化的函数
		void addGemToLeftView(); //之前一定要对leftView和data初始化
		void addWeaponToRightView();
		void sortPanel(cocos2d::ui::ScrollView* sv); //将内部的panel自动排列
		//回调函数
		void checkAllCallback(); //检查全部
		void returnCallback(); //回到维修舱
	};
}

#endif __SETTLEMENTWINDOW_H__// !__SETTLEMENTWINDOW_H__
