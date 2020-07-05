#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "FightScene.h"
#include "stationScene.h"
#include "StartScene.h"
#include "DataLoader.h"
#include "cocos2d.h"
/*所有切换scene的逻辑都写在这里*/
namespace game {
	class stationScene;

	class SceneManager
	{
	public:
		FightScene* pFightScene; //栈的最上层
		stationScene* pStationScene; //栈的第二层(其实中间应该还有starMap)
		StartScene* pStartScene; //栈的最底层
		static SceneManager* _manager; //实例
		static SceneManager* getInstance();
		//加载第一个scene, 如果testScene = nullptr的话,默认加载startScene
		void loadFirstScene(cocos2d::Scene* testScene);
		//点击开始游戏或加载存档, 进入stationScene
		//TODO: 这里以后要区分存档号
		void startSceneToStationScene();
		//点击station中的退出游戏进入startscene
		void stationSceneToStartScene();
		//点击stationScene到starMapscene
		void stationSceneToStarMapScene();
		//@test 没有做starMap的时候,直接点击进入fightScene
		void stationSceneToFightScene();
		//从fightscene返回stationScene
		void fightSceneToStationScene();
	};
}

#endif __SCENEMANAGER_H__

