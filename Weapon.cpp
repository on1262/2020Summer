#include "Weapon.h"
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;

void game::Weapon::onEnter() {
	//基类初始化
	Sprite::onEnter();
	//这里的设定是必须先调用weaponLoadS
	if (this->isLoaded == false) {
		cocos2d::log("Warning: Weapon is not loaded before enter.");
	}

}

bool game::Weapon::loadWeapon(game::Data * pConfig,game::Data* pData, ally wAlly, Vec2 windowSize)
{
	try {
		//登记阵营
		this->weaponAlly = wAlly;
		//管理配置
		this->isFire = false;
		this->isDamaged = false;
		this->windowSize = windowSize; //保存窗口大小，但是要注意，窗口在闯关期间不能变化
		this->weaponType = pData->findPairByKey(std::string("weaponType"))->value;
		this->texturePath = pConfig->findPairByKey(std::string("texturePath"))->value;
		//加载武器图片
		if (texturePath != std::string("empty")) { //如果为空，则不加载图片
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

//武器发射或开启伤害模式,重载时需要调用父函数

void game::Weapon::activate(HelloWorld * scene) //管理武器的启动状态（动画，伤害设定）
{
	//这里统一注册，但是在重写的函数中需要调用注册方法！
	this->pScene = scene;
	//这个函数应该是虚函数，需要继承并调用！
	this->isFire = true; //通用
}



