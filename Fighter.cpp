#include "Reflaction.h"
#include "Fighter.h"
#include "Weapon.h"
#include "SimpleAudioEngine.h"
#include <string>
USING_NS_CC;

bool game::Fighter::loadFighter(DataLoader* pGameDL, DataLoader* pUserDL, int fighterID, ally wAlly, cocos2d::Vec2 windowSize) //��ID����һ��ʵ����ע������Ҫ�����趨��ʵ��������
{
	try {
		//��λ���ݰ�����ID�ҵ�ʵ�������ݣ���ʵ�����ݵ�fighterType�ҵ���Ӧ����������
		Data* pData = pUserDL->findDataByLabel("fighter_" + std::to_string(fighterID)); //��ID�ҵ�ʵ�������ݰ�
		Data* pConfig = pGameDL->findDataByLabel(pData->findPairByKey("fighterType")->value); //��type�ҵ���������
		//��ʼ��ʵ��
		this->pGameDataLoader = pGameDL;
		this->pUserDataLoader = pUserDL;
		this->isFire = false;
		this->windowSize = windowSize;
		this->fAlly = wAlly;
		//���￪ʼ���������б�
		this->portVector = new std::vector<Weapon*>; //��ʼ�������б�
		int portID = 1;
		while (pConfig->findPairByKey(std::string("port_") + std::to_string(portID)) != nullptr) { //���ɴ�ȷʵ�����װ����ʱ
			std::string portName = std::string("port_") + std::to_string(portID); //port_1
			std::string portType = pConfig->findPairByKey(portName)->value; //gun
			std::string instanceID = pData->findPairByKey(portName)->value; //weapon_1(gun)
			Data* pWeaponData = pUserDL->findDataByLabel(instanceID); //weapon_1
			Data* pWeaponConfig = pGameDL->findDataByLabel(portType); //gun
			cocos2d::log(("Loading port: portName=" + portName + "|portType=" + portType + "|instanceID=" + instanceID + "|").c_str());
			if (pWeaponConfig == nullptr || pWeaponData == nullptr || pWeaponData->findPairByKey("weaponType")->value != portType) { //����������ͺͲ�۲�ƥ���û���������
				cocos2d::log("Warning: error weaponType or bad data.");
				portID++;
				break; //��ƥ���򲻼������������ͬʱ������һ��
			}
			//��ʼ��������portVector��
			Weapon* w = wReflect(portType);
			w->loadWeapon(pWeaponConfig, pWeaponData,wAlly, windowSize);
			portVector->push_back(w); //��ӵ������б���
			this->addChild(w); //ʵ����ӵ��ڵ���
			//***********������������λ��Ĭ�Ͼ��У��Ժ�Ҫ�Ľ�*
			w->setNormalizedPosition(cocos2d::Vec2(0.5, 0.5));
			//************************************************
			//����һ��Ҫ����portID++, ��Ȼ�ᵼ���ظ����
			portID++;
		}
		//���ظ�����������
		this->texturePath = pConfig->findPairByKey(std::string("texturePath"))->value;
		this->fighterType = pData->findPairByKey(std::string("fighterType"))->value;
		this->fighterName = pData->findPairByKey(std::string("fighterName"))->value;
		this->fighterID = fighterID;//��Ȼ������save������ݶ����������ǻ�Ƚ��鷳
		this->health = pData->valueConvert<int>(pData->findPairByKey(std::string("health"))->value);
		this->engineEnergy = pData->valueConvert<float>(pData->findPairByKey(std::string("engineEnergy"))->value);
		//����ͼ��(fighterһ��ʼ��create����ʱ������ͼƬ)
		this->setTexture(texturePath);
		
		isLoaded = true; //����״̬
		cocos2d::log("Successful: Fighter loaded.");
		return true;
	}
	catch (...) {
		cocos2d::log("Warning:Nullptr. Something wrong in config or texture missing.");
		isLoaded = false;
		return false;
	}
	return false;
}

bool game::Fighter::loadFighter(DataLoader * pGameDL, DataLoader * pUserDL, Data * fighterData, ally wAlly, cocos2d::Vec2 windowSize)
{
	//�з����
	std::string label = fighterData->m_label;
	int fighterID = fighterData->valueConvert<int>(std::string(label, label.find('_')+ 1, label.length()));
	cocos2d::log(("Info: load fighter by Data*, fighterID=" + std::to_string(fighterID)).c_str());
	return this->loadFighter(pGameDL, pUserDL, fighterID, wAlly, windowSize);
}

bool game::Fighter::init()
{
	if (!Sprite::init()) {
		return false;
	}
	else {
		this->isLoaded = false;//��ʼ��
		return true; 
	}
}

void game::Fighter::onEnter()
{
	Sprite::onEnter();//���ø�����
	if (this->isLoaded == false) cocos2d::log("Warning: Fighter is not loaded before being added in Scene.");
}

void game::Fighter::setAutoFire(FightScene *scene)
{
	this->pScene = scene;
	for (auto i = this->portVector->begin(); i != this->portVector->end(); i++) {
		(*i)->activate(this->pScene);
	}
}

void game::Fighter::getDamage(float damage)
{
	this->health -= (int)damage;
	if (health <= 0) this->destory();
}

void game::Fighter::destory()
{
	if (this->pScene == nullptr) cocos2d::log("Error: Destory fighter with pScene=nullptr");
	//ȡ����ײ���
	Scene *p1 = Director::getInstance()->getRunningScene();
	if (this->fAlly == ally::enemy) {
		bool result = this->pScene->setEnemyFighters(FightScene::setFlag::cancel, this);
		if (result == false) {
			cocos2d::log("Warning: error in canceling collision detection for enemy fighter");
		}
	}
	//��Ӵݻٶ������ⲿ����ʵ���Եģ�
	CCAnimation* destroyAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("explosion_1");
	CCAnimate* animate = CCAnimate::create(destroyAnimation);
	CCSprite* grossini = CCSprite::create();
	CCSpriteFrame* frame = destroyAnimation->getFrames().front()->getSpriteFrame();
	grossini->setDisplayFrame(frame);
	grossini->setPosition(this->getPosition());
	pScene->addChild(grossini, 0);
	grossini->runAction(animate);
	//ɾ���ڵ����
	this->pScene->removeChild(this,false);
	this->release(); //���Ҫ��д�Ļ�����������������������������ȱ�release����
	return;
}
