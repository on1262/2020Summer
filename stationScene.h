#ifndef __STATIONSCENE_H__
#define __STATIONSCENE_H__

#include "cocos2d.h"
#include "DataLoader.h"
#include "FitWeaponWindow.h"



/*ά�޲ֳ���������һ��UI���棬��������װ�䡢�浵���򿪲ֿ⡢����ͼ�Ȱ�ť*/
class stationScene :
	public cocos2d::Scene
{
public:
	//����ļ���ȡ��:�������������ݽ��Ӷ���DataLoader����
	game::DataLoader* pGameDataLoader;
	game::DataLoader* pUserDataLoader;
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
	game::FitWeaponWindow* fitWeapon;

};

#endif __STATIONSCENE_H__ // __STATIONSCENE_H__

