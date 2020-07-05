#ifndef __FITSHIPWINDOW_H__
#define __FITSHIPWINDOW_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include <vector>
#include "DataLoader.h"
#include "Data.h"
#include "Fighter.h"

namespace game {
	/*显示武器的一个栏板*/
	class FitShipPanel :public cocos2d::Sprite
	{
	public:
		bool isEquipped; //是否已经装备到飞船上了
		Data* weaponData; //存储对应的Weapon
		cocos2d::Label* status; //存储已装备和未装备的标签
		cocos2d::ui::Button* button;
		static FitShipPanel* create(bool isEquipped, Data* weaponData);
		void buttonClickCallback(); //向飞船添加或移除武器
	};
	class FitShipWindow : public cocos2d::Sprite
	{
	public:

		//成员
		static FitShipWindow* _window; //一个时候只能有一个window
		cocos2d::ui::ScrollView* weaponView; //武器栏
		Data* shipData; //用于判断武器槽
		Fighter* pFighter; //用于展示外观,现在先不考虑
		cocos2d::Label* shipLabel; //用于显示飞船信息
		/*使用全局性的DataLoader, 需要传入ship的指针*/
		static FitShipWindow* create(Data* shipData);
		/*调用时用这个方法*/
		static FitShipWindow* getInstance();
		/*增加和删减weapon*/
		//storage中删除这个实例, 面板中删除panel,飞船如果有可用port则添加,如果没有就视为无效,最后对panel排序
		bool equipWeapon(FitShipPanel* panel, Data* wData); 
		//由给出的实例名查找Data
		bool unEquipWeapon(FitShipPanel * panel, Data * wData);
		//调整panel,装备的武器要放在最前
		void sortPanel();
		//刷新武器和飞船外观(TODO:飞船外观的实时改变以后再做)
		void flush();
	};
}
#endif __FITSHIPWINDOW_H__

