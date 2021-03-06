#ifndef __FIGHTER_H__
#define __FIGHTER_H__
#include "FightScene.h"
#include "cocos2d.h"
#include "Weapon.h"
#include "Data.h"
#include <vector>
#include <string>

namespace game {
	class FightScene;
	enum ally;
	class Weapon; //前置声明

	/*注意，Fighter是从模版中创建一个全新的战机，而不是用来保存和读取游戏的实时状态
	Weapon的类型是由config中的数据决定的，但是weapon对象是由user的游戏数据决定的*/
	class Fighter : public cocos2d::Sprite
	{
	public:
		//飞船外观
		std::string texturePath;
		//飞船识别
		std::string fighterType; //这是用来识别类型的，同类型的相同
		std::string fighterName; //这是显示给玩家的名字，每架可以不同
		int fighterID; //这时给系统识别的ID,是否相同取决于系统设定
		//飞船状态
		bool isFire; //是否处于开火状态
		int health; //生命值，低于0自动毁灭
		//引擎参数
		float engineEnergy;
		//武器状态
		std::vector<Weapon*> *portVector; //通知各成员的列表
		//击毁后的积分，这里先写一个static原型
		static const int score = 1;
		//函数
		bool loadFighter(DataLoader* pGameDL, DataLoader* pUserDL, int fighterID, ally wAlly, cocos2d::Vec2 windowSize); //一定要调用这个函数，用来加载数据
		bool loadFighter(DataLoader* pGameDL, DataLoader* pUserDL, Data* fighterData, ally wAlly, cocos2d::Vec2 windowSize); //重载，主要是方便直接用Data* 初始化
		virtual bool init();
		virtual void onEnter(); //这里检测isLoaded
		virtual void setAutoFire(FightScene *scene); //通知各武器开火,这个函数必须在添加场景以后执行！
		//停机, 关闭移动功能和所有武器的发射状态
		virtual void shutdown();
		/*伤害或被摧毁*/
		virtual void getDamage(float damage)=0;//碰撞后调用，自动减少相应的生命值
		void destroy();  //碰撞判定调用,播放被摧毁的动画,注销碰撞判定,继承的类只需要重写
		virtual void destroyCallback()=0; //继承类特异性操作的代码
	protected:
		bool isActivated;//设置武器开关
		bool isLoaded; //是否执行过loadFighter，防止出错
		Data* config; //保存自己的数据
		DataLoader* pGameDataLoader, *pUserDataLoader;
		cocos2d::Vec2 windowSize; //可用于判断飞机是否出界
		FightScene* pScene; //用于碰撞判定
		//用于敌我阵营识别
		ally fAlly;
	};
}

#endif __FIGHTER_H__ //__FIGHTER_H__
