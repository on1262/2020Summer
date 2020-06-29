#include "Reflaction.h"
#include "Level.h"
#include "Fighter.h"
#include "cocos2d.h"
#include <cstdlib>
#include <ctime>
USING_NS_CC;

bool game::Level::loadLevel(DataLoader * saveDL, DataLoader * configDL, int levelID, float playerFactor,  Vec2 windowSize)
{
	if (saveDL == nullptr || configDL == nullptr) {
		cocos2d::log("Warning: nullptr in loading level.");
		return false;
	}
	//保存信息
	this->saveDataLoader = saveDL;
	this->configDataLoader = configDL;
	this->windowSize = windowSize;
	//初始化waitinglist
	this->waitingList = new std::vector<Fighter*>;
	//初始化tag
	this->runningEnemyTag = 3000;
	/************这里的信息是需要定制的，现在为了调试直接指定了*/
	this->stageNumber = 9;
	this->batchNumber = 5;
	this->batchTime = 3;//每三秒进行一次
	this->playerFactor = playerFactor;
	//关卡信息存放在save里
	Data* levelData = saveDL->findDataByLabel("level_" + std::to_string(levelID));
	if (levelData == nullptr) {
		cocos2d::log("Warning: nullptr in loading level data.");
		return false;
	}
	//加载关卡数据
	this->relativeDifficulty = levelData->valueConvert<int>(levelData->findPairByKey("relativeDifficulty")->value);
	this->timeDifficulty = levelData->valueConvert<int>(levelData->findPairByKey("relativeDifficulty")->value);
	//读取敌机类型数据，添加给enemyTypeVec
	this->enemyTypeVec = new std::vector<enemyType>;
	int enemyID = 1;
	while (levelData->findPairByKey("enemy_" + std::to_string(enemyID)) != nullptr) {
		try {
			std::string enemyInstance = levelData->findPairByKey("enemy_" + std::to_string(enemyID))->value;
			Data* pEData = saveDataLoader->findDataByLabel(enemyInstance); //fighter_n的label
			std::string sz = levelData->findPairByKey("stageZone_" + std::to_string(enemyID))->value;
			std::string ft = pEData->findPairByKey("fighterType")->value;
			int gp = levelData->valueConvert<int>(levelData->findPairByKey("generateProbability_" + std::to_string(enemyID))->value);
			//构建enemyType
			enemyType et = { pEData, ft, sz, gp };
			//添加
			this->enemyTypeVec->push_back(et);
		}
		catch (...) {
			cocos2d::log("Warning: error in loading enemyData.");
			return false;
		}
		enemyID++;
	}
	return true;
}

void game::Level::activate(HelloWorld *pScene) 
{
	cocos2d::log("Level activated.");
	//初始化随机数生成器
	srand(unsigned(time(0)));
	//初始化stage和batch
	runningStage = 1;
	runningBatch = 1;
	//初始化scene
	this->pScene = pScene;
	//加载一个实验动画，这部分以后要删去
	CCSpriteFrameCache *frameCache = SpriteFrameCache::getInstance();
	frameCache->addSpriteFramesWithFile("explosion_1.plist");
	Vector<SpriteFrame*> sfVec;
	for (int i = 1; i <= 15; i++)
	{
		CCSpriteFrame *frame = frameCache->getSpriteFrameByName(("explosion_1__" + std::to_string(i) + ".png").c_str());
		sfVec.pushBack(frame);
	}
	CCAnimation* animation = CCAnimation::createWithSpriteFrames(sfVec, 0.025f);
	animation->setRestoreOriginalFrame(false);
	cocos2d::log("Animation loaded");
	//有多个动画时也可以使用CCAnimationCache
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "explosion_1");
	//设置回调
	cocos2d::ccSchedulerFunc callback = std::bind(&game::Level::batchCallback, this, std::placeholders::_1);
	this->schedule(callback, (float) batchTime, std::string("batchCallback")); //调度器，每batchtime秒更新一次batch
}

void game::Level::batchCallback(float)
{
	if (this->isStageStopped == true) { //如果还在Boss战阶段，就等待
		return;
	}
	float batchDifficulty = 1.0; //这个应该是根据不同批次来调整难度
	/* 批次生成数量=批次难度*关卡相对难度*时间梯度难度*玩家水平修正系数 */
	int generateNumber = (int)(batchDifficulty * relativeDifficulty * (timeDifficulty * runningStage)* playerFactor);
	//调试输出
	cocos2d::log(("Info: Generating enemy in Stage=" + std::to_string(runningStage) + "|batch=" + std::to_string(runningBatch) + "|GenerateNumber=" + std::to_string(generateNumber)).c_str());
	//创建绑定
	cocos2d::ccSchedulerFunc callback = std::bind(&game::Level::fighterEnterCallback, this, std::placeholders::_1);

	for (int genNum = 1; genNum <= generateNumber; genNum++) { //进行generateNumber次抽奖
		//遍历，将大于出现概率的都添加一份在scene里
		int prob = (int)Level::getRandom(1.0, 100.0);
		for (auto i = this->enemyTypeVec->begin(); i != this->enemyTypeVec->end(); i++) {
			if (i->generateProbability >= prob && i->stageZone.find((char)(48 + this->runningStage)) != std::string::npos) { //48是0的ASCII,当i可以在这个stage生成，且被抽中时
				cocos2d::log(("Info: generating enemy. enemyType=" + i->fighterType).c_str());
				Fighter* e = fReflect(i->fighterType); //反射获取对象
				if (e == nullptr) cocos2d::log("ERROR: NULLPTR.");
				e->loadFighter(this->configDataLoader, this->saveDataLoader, i->pData,ally::enemy, this->windowSize); //加载飞机
				float enterTime = Level::getRandom(0.1, 2.9);
				//更新adding fighter
				this->waitingList->push_back(e);
				e->retain(); //涉及到内存管理问题,不加这个会被自动垃圾回收

				//创建一个定时器
				
				this->scheduleOnce(callback, enterTime, "fighterCB_" + std::to_string(this->runningStage) + "_" + std::to_string(this->runningBatch) + "_" + std::to_string(genNum)); //调度器
			}
		}
	}
	//升级batch
	updateBatch();
}

void game::Level::fighterEnterCallback(float)
{
	if (this->waitingList->empty() == true)
	{
		return;
	}
	else {
		Fighter* wf = this->waitingList->back();
		cocos2d::Director::getInstance()->getRunningScene()->addChild(wf); //生成了多少次就调用多少次addChild，理论上不会有问题
		//统一管理tag,便于查找,敌机的tag是从3000开始的
		wf->setTag(this->runningEnemyTag);
		runningEnemyTag++;
		wf->setAutoFire(this->pScene); //加入后就开火
		this->waitingList->pop_back();
	}
}

void game::Level::updateBatch()
{
	if (this->runningBatch == 5) {
		runningBatch = 1;
		if (this->runningStage == 9) { //结束~
			runningBatch = 0;
			runningStage = 0;
		}
		else {
			runningStage++;
			runningBatch = 1; //更新
		}
	}
	else {
		//如果是单纯地更新batch
		runningBatch++;
	}
}

float game::Level::getRandom(float start, float end) { //生成（start,end）开区间内的浮点数，如果想得到1-100值，将start设为1，将end设为100

	float i = start + (end - start)*rand() / (RAND_MAX + 1.0);
	return i;
}