#ifndef __WEAPONBULLET_H__
#define __WEAPONBULLET_H__

#include "Weapon.h"
#include "cocos2d.h"
#include "Data.h"
#include "FightScene.h"
namespace game {

	class WeaponBullet : public Weapon
	{
	public:
		//bullet��������
		int bulletSpeed;
		int damage;
		//����
		bool loadWeapon(Data * pConfig, Data* pData, ally wAlly, cocos2d::Vec2 windowSize); //���ظ�����
		//��дonEnter���ö���
		void onEnter();
		//��дactivateʵ���ӵ����ƶ�����
		void activate(FightScene *scene);
		//����ݻ�
		void destroy();
		//��дcreate
		CREATE_FUNC(WeaponBullet);
	};
}

#endif __WEAPONBULLET_H__ //__WEAPONBULLET_H__