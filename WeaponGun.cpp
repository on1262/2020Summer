#include "cocos2d.h"
#include "Data.h"
#include "WeaponGun.h"
#include "WeaponBullet.h"


bool game::WeaponGun::loadWeapon(Data* pConfig, Data* pData, ally wAlly, cocos2d::Vec2 windowSize)
{
	Weapon::loadWeapon(pConfig, pData,wAlly, windowSize); //���ظ�����

	try {
		this->rpm = pData->valueConvert<int>(pData->findPairByKey(std::string("attr_rpm"))->value);
		//�ⲿ��Ӧ��ΪweaponGun����һ��bulletType��Ȼ��������Ѱַ������ֱ��ָ����Ψһ��bullet
		this->pBulletConfig = pConfig->pLoader->findDataByLabel("bullet");
		this->pBulletData = pData->pLoader->findDataByLabel("weapon_2");
		return true;
	} catch (...) {
		cocos2d::log("Warning:Nullptr in loading WeaponGun");
	}
	return false;
}

void game::WeaponGun::activate(FightScene *scene) //activateһ��Ҫ�ڴ����ڵ����Ժ���ã�
{
	Weapon::activate(scene);
	//������������ײ�ж����������ﲻ�ü�
	cocos2d::log("WeaponGun activated.");
	cocos2d::ccSchedulerFunc callback = std::bind(&game::WeaponGun::gunCallback, this, std::placeholders::_1);
	this->schedule(callback, 0.25,std::string("gunCallback")); //��������0.5������Զ�����
}

void game::WeaponGun::gunCallback(float)
{
	//�����ҵķɻ������ڣ���ô�����ᴴ���κζ���
	cocos2d::Node* fighter = cocos2d::Director::getInstance()->getRunningScene()->getChildByTag(1000);
	if (fighter == nullptr) return;
	//����һ���ӵ�
	game::WeaponBullet* wb = game::WeaponBullet::create();
	wb->loadWeapon(pBulletConfig, pBulletData, weaponAlly, windowSize);
	cocos2d::Director::getInstance()->getRunningScene()->addChild(wb); //��scene��ӳ���
	//����������ڵ�λ������
	wb->setPosition(fighter->getPosition());
	//�ֶ�����activate(). ��ΪҪ����ָ��
	wb->activate(pScene);
	cocos2d::log("Created a instance of WeaponBullet.");
}
