#include "Weapon.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

void game::Weapon::onEnter() {
	//�����ʼ��
	Sprite::onEnter();
	//������趨�Ǳ����ȵ���weaponLoadS
	if (this->isLoaded == false) {
		cocos2d::log("Warning: Weapon is not loaded before enter.");
	}

}

bool game::Weapon::loadWeapon(game::Data * pConfig,game::Data* pData, ally wAlly, Vec2 windowSize)
{
	try {
		//�Ǽ���Ӫ
		this->weaponAlly = wAlly;
		//��������
		this->isFire = false;
		this->isDamaged = false;
		this->windowSize = windowSize; //���洰�ڴ�С������Ҫע�⣬�����ڴ����ڼ䲻�ܱ仯
		this->weaponType = pData->findPairByKey(std::string("weaponType"))->value;
		this->texturePath = pConfig->findPairByKey(std::string("texturePath"))->value;
		//��������ͼƬ
		if (texturePath != std::string("empty")) { //���Ϊ�գ��򲻼���ͼƬ
			this->setTexture(texturePath);
		}
		this->isLoaded = true;
		return true;
	}
	catch (...) {
		cocos2d::log("Warning: Nulltpr. Something wrong in weapon config or missing texture.");
		this->isLoaded = false;
		return false;
	}
	return false;
}

//������������˺�ģʽ,����ʱ��Ҫ���ø�����

void game::Weapon::activate(HelloWorld * scene) //��������������״̬���������˺��趨��
{
	//����ͳһע�ᣬ��������д�ĺ�������Ҫ����ע�᷽����
	this->pScene = scene;
	//�������Ӧ�����麯������Ҫ�̳в����ã�
	this->isFire = true; //ͨ��
}



