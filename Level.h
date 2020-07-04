#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "cocos2d.h"
#include "Weapon.h"
#include "Data.h"
#include "DataLoader.h"
#include "Fighter.h"
#include <vector>
#include <string>
#include "dropIcon.h"
namespace game {
	//这个结构用来存储3个string组成的enemy类型描述
	struct enemyType {
		Data* pData; //这里用一份Data* 初始化多个实例，但是Data对实例的影响是单向的，并不会有问题
		std::string fighterType;
		std::string stageZone; //出现在的stage,如12345
		int generateProbability; //全局出现概率
	};

	/*动态生成关卡并控制进度，需要添加到scene的子节点*/
	class Level : public cocos2d::Node {
	public:
		CREATE_FUNC(Level);
		//加载关卡信息
		bool loadLevel(DataLoader * saveDL, DataLoader * configDL, int levelID,float playerFactor, cocos2d::Vec2 windowSize);
		void activate(FightScene *scene); //开始关卡，这个要在添加Level以后调用（前面可以放一些剧情之类的）
		//每3秒的回调函数
		void batchCallback(float);
		//3秒内的创建fighter回调函数
		void fighterEnterCallback(float);
		//在指定位置创建掉落物
		void generateDropItem(int count, cocos2d::Vec2 position);
		//添加战利品
		void addDropItem(dropItem di);
		//工具，更新runningBatch和stage
		void updateBatch();
		//工具，得到1-100的随机整数
		static float getRandom(float start, float end);
		game::Data* getLevelData();
		std::vector<dropItem>* getSpoils();
		void setFinishedFlag(bool flag);
	protected:
		//关卡载入时
		DataLoader* saveDataLoader, *configDataLoader;
		int ID; //关卡ID
		int relativeDifficulty;
		int timeDifficulty;
		float playerFactor; //玩家装备、战绩对难度的修正
		FightScene *pScene;//碰撞判定用，给飞机注册的
		//***一些预设数据
		int stageNumber;
		int batchNumber;
		int batchTime;
		//***
		std::vector<enemyType>* enemyTypeVec; //存储备选的enemyType
		std::vector<dropItem>* dropItemVec; //存储备选的掉落物
		std::vector<dropItem>* spoils; //存储玩家获得的战利品
		cocos2d::Vec2 windowSize;
		//关卡运行时信息
		bool isStageStopped; //BOSS生成时和被击败时都要通知level
		int runningStage; //检查正在跑的阶段, 0代表关卡结束
		int runningBatch; //正在跑的批次，注意batch取值1-5,到5时需要重置batch， 0代表关卡结束
		std::vector<Fighter*> *waitingList; //给回调函数的，需要添加的enemy 
		int runningEnemyTag; //从3000开始更新最新敌机的分配tag,这个tag不可以超过20000
		//关卡结束信息
		bool isFinished;
	};
}
#endif __LEVEL_H__ //__LEVEL_H__