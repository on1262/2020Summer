#ifndef __SCENEMANAGER_H__
#define __SCENEMANAGER_H__

#include "FightScene.h"
#include "stationScene.h"
#include "StartScene.h"
#include "DataLoader.h"
#include "cocos2d.h"
/*�����л�scene���߼���д������*/
namespace game {
	class stationScene;

	class SceneManager
	{
	public:
		FightScene* pFightScene; //ջ�����ϲ�
		stationScene* pStationScene; //ջ�ĵڶ���(��ʵ�м�Ӧ�û���starMap)
		StartScene* pStartScene; //ջ����ײ�
		static SceneManager* _manager; //ʵ��
		static SceneManager* getInstance();
		//���ص�һ��scene, ���testScene = nullptr�Ļ�,Ĭ�ϼ���startScene
		void loadFirstScene(cocos2d::Scene* testScene);
		//�����ʼ��Ϸ����ش浵, ����stationScene
		//TODO: �����Ժ�Ҫ���ִ浵��
		void startSceneToStationScene();
		//���station�е��˳���Ϸ����startscene
		void stationSceneToStartScene();
		//���stationScene��starMapscene
		void stationSceneToStarMapScene();
		//@test û����starMap��ʱ��,ֱ�ӵ������fightScene
		void stationSceneToFightScene();
		//��fightscene����stationScene
		void fightSceneToStationScene();
	};
}

#endif __SCENEMANAGER_H__

