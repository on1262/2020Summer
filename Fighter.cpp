#include "Reflaction.h"
#include "Fighter.h"
#include "Weapon.h"
#include "SimpleAudioEngine.h"
#include <string>
USING_NS_CC;

bool game::Fighter::loadFighter(DataLoader* pGameDL, DataLoader* pUserDL, int fighterID, ally wAlly, cocos2d::Vec2 windowSize) //用ID加载一个实例，注意这里要加载设定和实例化参数
{
	try {
		//定位数据包：由ID找到实例化数据，由实例数据的fighterType找到对应的配置数据
		Data* pData = pUserDL->findDataByLabel("fighter_" + std::to_string(fighterID)); //由ID找到实例化数据包
		Data* pConfig = pGameDL->findDataByLabel(pData->findPairByKey("fighterType")->value); //由type找到配置数据
		//初始化实例
		this->pGameDataLoader = pGameDL;
		this->pUserDataLoader = pUserDL;
		this->isFire = false;
		this->windowSize = windowSize;
		this->fAlly = wAlly;
		//这里开始加载武器列表
		this->portVector = new std::vector<Weapon*>; //初始化武器列表
		int portID = 1;
		while (pConfig->findPairByKey(std::string("port_") + std::to_string(portID)) != nullptr) { //当飞船确实有这个装备槽时
			std::string portName = std::string("port_") + std::to_string(portID); //port_1
			std::string portType = pConfig->findPairByKey(portName)->value; //gun
			std::string instanceID = pData->findPairByKey(portName)->value; //weapon_1(gun)
			Data* pWeaponData = pUserDL->findDataByLabel(instanceID); //weapon_1
			Data* pWeaponConfig = pGameDL->findDataByLabel(portType); //gun
			cocos2d::log(("Loading port: portName=" + portName + "|portType=" + portType + "|instanceID=" + instanceID + "|").c_str());
			if (pWeaponConfig == nullptr || pWeaponData == nullptr || pWeaponData->findPairByKey("weaponType")->value != portType) { //如果武器类型和插槽不匹配或没有这个武器
				cocos2d::log("Warning: error weaponType or bad data.");
				portID++;
				break; //不匹配则不加载这个武器，同时尝试下一个
			}
			//初始化武器到portVector里
			Weapon* w = wReflect(portType);
			w->loadWeapon(pWeaponConfig, pWeaponData,wAlly, windowSize);
			portVector->push_back(w); //添加到管理列表中
			this->addChild(w); //实际添加到节点中
			//***********这里设置武器位置默认居中，以后要改进*
			w->setNormalizedPosition(cocos2d::Vec2(0.5, 0.5));
			//************************************************
			//这里一定要设置portID++, 不然会导致重复添加
			portID++;
		}
		//加载各种配置数据
		this->texturePath = pConfig->findPairByKey(std::string("texturePath"))->value;
		this->fighterType = pData->findPairByKey(std::string("fighterType"))->value;
		this->fighterName = pData->findPairByKey(std::string("fighterName"))->value;
		this->fighterID = fighterID;//虽然可以用save里的数据读出来，但是会比较麻烦
		this->health = pData->valueConvert<int>(pData->findPairByKey(std::string("health"))->value);
		this->engineEnergy = pData->valueConvert<float>(pData->findPairByKey(std::string("engineEnergy"))->value);
		//加载图像(fighter一开始用create加载时不设置图片)
		this->setTexture(texturePath);
		
		isLoaded = true; //更新状态
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
	//切分序号
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
		this->isLoaded = false;//初始化
		return true; 
	}
}

void game::Fighter::onEnter()
{
	Sprite::onEnter();//调用父函数
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
	//取消碰撞检测
	Scene *p1 = Director::getInstance()->getRunningScene();
	if (this->fAlly == ally::enemy) {
		bool result = this->pScene->setEnemyFighters(FightScene::setFlag::cancel, this);
		if (result == false) {
			cocos2d::log("Warning: error in canceling collision detection for enemy fighter");
		}
	}
	//添加摧毁动画（这部分是实验性的）
	CCAnimation* destroyAnimation = CCAnimationCache::sharedAnimationCache()->animationByName("explosion_1");
	CCAnimate* animate = CCAnimate::create(destroyAnimation);
	CCSprite* grossini = CCSprite::create();
	CCSpriteFrame* frame = destroyAnimation->getFrames().front()->getSpriteFrame();
	grossini->setDisplayFrame(frame);
	grossini->setPosition(this->getPosition());
	pScene->addChild(grossini, 0);
	grossini->runAction(animate);
	//删除节点对象
	this->pScene->removeChild(this,false);
	this->release(); //如果要改写的话，最好在最后调用这个，否则自身就先被release掉了
	return;
}
