#ifndef __STATIONSCENE_H__
#define __STATIONSCENE_H__

#include "cocos2d.h"
#include "DataLoader.h"
#include "FitWeaponWindow.h"
#include "FitShipWindow.h"
#include "SceneManager.h"

namespace game {
	class FitShipWindow;
	/*ά�޲ֳ���������һ��UI���棬��������װ�䡢�浵���򿪲ֿ⡢����ͼ�Ȱ�ť*/
	class stationScene :
		public cocos2d::Scene
	{
	public:
		//����ļ���ȡ��:�������������ݽ��Ӷ���DataLoader����
		DataLoader* pGameDataLoader;
		DataLoader* pUserDataLoader;
		//�ж��Ƿ��ڴ���ǰ�ṩ���ݶԽ�
		bool isLoaded;
		//����
		void setDataLoader(game::DataLoader* pGame, game::DataLoader* pUser);//�������ݽ���
		bool init();
		void onEnter(); //���뱳��ͼƬ���ı���Ϣ
		CREATE_FUNC(stationScene);
		//�򿪺͹رյĻص�����
		void chickFitWeaponWindow();
		void chickFitShipWindow();
		void chickStorageWindow();
		void chickStarMapWindow();
		void chickSaveWindow();
		void closeAll(); //�����л�
	private:
		//�л����ѡ��ʱ��
		FitWeaponWindow* fitWeapon; //װ���������
		FitShipWindow* fitShip; //װ��ɴ����

	};
}

#endif __STATIONSCENE_H__ // __STATIONSCENE_H__

