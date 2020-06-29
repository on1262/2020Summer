#ifndef __WEAPONGUN_H__
#define __WEAPONGUN_H__

#include "HelloWorldScene.h"
#include "Weapon.h"
#include "cocos2d.h"
#include "Data.h"

namespace game {
	class WeaponGun : public Weapon
	{
	public:
		//gun����
		int rpm;
		//����
		bool loadWeapon(Data * pConfig, Data* pData, ally wAlly, cocos2d::Vec2 windowSize); //���ظ�����
		void activate(HelloWorld *scene); //����
		//ע��ص�����
		void gunCallback(float);
		//��������
		CREATE_FUNC(WeaponGun);
	private:
		//����ӵ���ӵ��Ĳ�������
		Data* pBulletData;
		Data* pBulletConfig;
	};
}
#endif __WEAPONGUN_H__ //__WEAPONGUN_H__