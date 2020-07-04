#include "stationScene.h"
#ifndef __FITWEAPONWINDOW_H__
#define __FITWEAPONWINDOW_H__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h" //图形控件
#include "DataLoader.h"
#include "Data.h"
#include "Weapon.h"
#include "gem.h"
#include <vector>
#include <string>
#include "util.h"

class stationScene;

/*创建一个面板*/
namespace game {
	class FitWeaponWindow : public cocos2d::Sprite
	{
	private:
		class WeaponPanel : public cocos2d::ui::Button
		{
		public:
			//通信类
			std::string weaponPath;//存放武器图片
			std::string backgroundPath;//存放背景图片
			Data* pData; //存放武器实例的相关信息
			//从Data中存取信息的工具(包括转中文)
			std::vector<gem*> gemInWeapon; //根据Data中的信息初始化gem
			void addGem(gem*g); //添加指定的gem
			bool deleteGem(gem*g); //通过点击icon移除指定的gem
			//gemIcon调用的callback
			void gemIconCallback(Ref* sender);
			FitWeaponWindow* window;
			static WeaponPanel* create(FitWeaponWindow* window, Data* pData, std::string weaponPath, std::string backgroundPath);
			void chickPanel();
			static WeaponPanel* create(const std::string &normalImage, const std::string& selectedImage, const std::string& disableImage, TextureResType texType);
		};
		class GemPanel : public cocos2d::ui::Button
		{
		public:
			//通信
			FitWeaponWindow *window;
			gem* pGem; //这里不是很遵守命名规范...等重构的时候再改进吧
			//重载生成函数，内部调用
			static GemPanel* create(const std::string &normalImage, const std::string& selectedImage, const std::string& disableImage, TextureResType texType);
			//直接生成的时候用这个
			static GemPanel* create(FitWeaponWindow *window, gem* g);
			//回调函数
			void chickPanel(Ref* sender);
		};

	public:
		//获取数据
		game::DataLoader* pGameDataLoader;
		game::DataLoader* pUserDataLoader;
		static FitWeaponWindow* _window;
		CREATE_FUNC(FitWeaponWindow);
		stationScene* scene; //获得一个引用
		Data* storage; //获取玩家仓库里的东西
		//存储两边列表中的东西
		std::vector<WeaponPanel*> weaponPanelVec;
		std::vector<GemPanel*> gemPanelVec; //只存储仓库中还没装备上的gem
		//显示在面板上的武器信息
		cocos2d::ui::Button* weaponImage;
		cocos2d::Label* weaponName, *gemGain, *performance, *story;//武器名称, 增益框，性能提升栏，武器说明
		WeaponPanel* showingWeaponPanel; //当前正在显示的weapon
		//对gem的添加、移除
		void addGem(gem*g);
		void deleteGemPanel(GemPanel *gp);
		//刷新左中右，只是对现有child进行排位, pos=left/right, 刷新中间的面板用chooseWeapon进行
		void flush(std::string pos);
		//滚动条
		cocos2d::ui::ScrollView* weaponScrollView;
		cocos2d::ui::ScrollView* gemScrollView;
		static FitWeaponWindow* create(stationScene* s); //初始化并创建一个窗口
		void chooseWeapon(FitWeaponWindow::WeaponPanel* wp); //当用户单击一个武器时，显示相关的信息, 也可以用来刷新
	};
}

#endif __FITWEAPONWINDOW_H__ // __FITWEAPONWINDOW_H__
