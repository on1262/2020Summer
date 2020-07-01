#ifndef __STATIONSCENE_H__
#define __STATIONSCENE_H__

#include "cocos2d.h"
#include "DataLoader.h"
#include "FitWeaponWindow.h"



/*维修仓场景，包含一个UI界面，用于武器装配、存档、打开仓库、打开星图等按钮*/
class stationScene :
	public cocos2d::Scene
{
public:
	//添加文件存取器:各个场景的数据交接都由DataLoader负责
	game::DataLoader* pGameDataLoader;
	game::DataLoader* pUserDataLoader;
	//判断是否在创建前提供数据对接
	bool isLoaded;
	//方法
	void setDataLoader(game::DataLoader* pGame, game::DataLoader* pUser);//设置数据交接
	bool init();
	void onEnter(); //载入背景图片、文本信息
	CREATE_FUNC(stationScene);
	//打开和关闭的回调方法
	void chickFitWeaponWindow();
	void chickFitShipWindow();
	void chickStorageWindow();
	void chickStarMapWindow();
	void chickSaveWindow();
	void closeAll(); //用于切换
private:
	//切换面板选项时用
	game::FitWeaponWindow* fitWeapon;

};

#endif __STATIONSCENE_H__ // __STATIONSCENE_H__

