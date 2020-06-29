#include "WeaponBullet.h"


bool game::WeaponBullet::loadWeapon(Data * pConfig, Data * pData , ally wAlly, cocos2d::Vec2 windowSize)
{
	Weapon::loadWeapon(pConfig, pData, wAlly, windowSize); //���������ͨ�ò��ּ̳�
	
	try {
		this->bulletSpeed = pData->valueConvert<int>(pData->findPairByKey(std::string("bulletSpeed"))->value);
		this->damage = pData->valueConvert<int>(pData->findPairByKey(std::string("damage"))->value);
		return true;
	}
	catch (...) {
		cocos2d::log("Warning:Nullptr in loading WeaponBullet");
	}
	return false;
}

void game::WeaponBullet::onEnter()
{
	Weapon::onEnter();
}

void game::WeaponBullet::activate(HelloWorld *scene)
{
	Weapon::activate(scene); //���ø�����
	//����Ҫ���ӵ����ӵ���ײ�ж���
	if (this->weaponAlly == ally::player) {
		//������ײ�ж�
		scene->setPlayerBullets(HelloWorld::setFlag::reigster, this);
		//�����ӵ�ǰ������
		this->runAction(cocos2d::MoveBy::create(10 / bulletSpeed, cocos2d::Vec2(0, this->windowSize.y)));

	}
	else if (this->weaponAlly == ally::enemy) {
		//������ײ�ж�
		scene->setEnemyBullets(HelloWorld::setFlag::reigster, this);
		//�����ӵ�ǰ������
		this->runAction(cocos2d::MoveBy::create(10 / bulletSpeed, cocos2d::Vec2(0, -1* this->windowSize.y))); //���˵��ӵ�����Ҫ�෴~

	}
}

void game::WeaponBullet::destroy()
{
	if (this->pScene == nullptr) cocos2d::log("Error: Destory fighter with pScene=nullptr");
	//ȡ����ײ���
	if (this->weaponAlly == ally::player) {
		bool result = this->pScene->setPlayerBullets(HelloWorld::setFlag::cancel, this);
		if (result == false) {
			cocos2d::log("Warning: error in canceling collision detection for player bullet");
		}
	}
	this->removeChild(this, false);
}