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
	//������Ϣ
	this->saveDataLoader = saveDL;
	this->configDataLoader = configDL;
	this->windowSize = windowSize;
	//��ʼ��waitinglist
	this->waitingList = new std::vector<Fighter*>;
	//��ʼ��tag
	this->runningEnemyTag = 3000;
	/************�������Ϣ����Ҫ���Ƶģ�����Ϊ�˵���ֱ��ָ����*/
	this->stageNumber = 9;
	this->batchNumber = 5;
	this->batchTime = 3;//ÿ�������һ��
	this->playerFactor = playerFactor;
	//�ؿ���Ϣ�����save��
	Data* levelData = saveDL->findDataByLabel("level_" + std::to_string(levelID));
	if (levelData == nullptr) {
		cocos2d::log("Warning: nullptr in loading level data.");
		return false;
	}
	//���عؿ�����
	this->relativeDifficulty = levelData->valueConvert<int>(levelData->findPairByKey("relativeDifficulty")->value);
	this->timeDifficulty = levelData->valueConvert<int>(levelData->findPairByKey("relativeDifficulty")->value);
	//��ȡ�л��������ݣ���Ӹ�enemyTypeVec
	this->enemyTypeVec = new std::vector<enemyType>;
	int enemyID = 1;
	while (levelData->findPairByKey("enemy_" + std::to_string(enemyID)) != nullptr) {
		try {
			std::string enemyInstance = levelData->findPairByKey("enemy_" + std::to_string(enemyID))->value;
			Data* pEData = saveDataLoader->findDataByLabel(enemyInstance); //fighter_n��label
			std::string sz = levelData->findPairByKey("stageZone_" + std::to_string(enemyID))->value;
			std::string ft = pEData->findPairByKey("fighterType")->value;
			int gp = levelData->valueConvert<int>(levelData->findPairByKey("generateProbability_" + std::to_string(enemyID))->value);
			//����enemyType
			enemyType et = { pEData, ft, sz, gp };
			//���
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
	//��ʼ�������������
	srand(unsigned(time(0)));
	//��ʼ��stage��batch
	runningStage = 1;
	runningBatch = 1;
	//��ʼ��scene
	this->pScene = pScene;
	//����һ��ʵ�鶯�����ⲿ���Ժ�Ҫɾȥ
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
	//�ж������ʱҲ����ʹ��CCAnimationCache
	CCAnimationCache::sharedAnimationCache()->addAnimation(animation, "explosion_1");
	//���ûص�
	cocos2d::ccSchedulerFunc callback = std::bind(&game::Level::batchCallback, this, std::placeholders::_1);
	this->schedule(callback, (float) batchTime, std::string("batchCallback")); //��������ÿbatchtime�����һ��batch
}

void game::Level::batchCallback(float)
{
	if (this->isStageStopped == true) { //�������Bossս�׶Σ��͵ȴ�
		return;
	}
	float batchDifficulty = 1.0; //���Ӧ���Ǹ��ݲ�ͬ�����������Ѷ�
	/* ������������=�����Ѷ�*�ؿ�����Ѷ�*ʱ���ݶ��Ѷ�*���ˮƽ����ϵ�� */
	int generateNumber = (int)(batchDifficulty * relativeDifficulty * (timeDifficulty * runningStage)* playerFactor);
	//�������
	cocos2d::log(("Info: Generating enemy in Stage=" + std::to_string(runningStage) + "|batch=" + std::to_string(runningBatch) + "|GenerateNumber=" + std::to_string(generateNumber)).c_str());
	//������
	cocos2d::ccSchedulerFunc callback = std::bind(&game::Level::fighterEnterCallback, this, std::placeholders::_1);

	for (int genNum = 1; genNum <= generateNumber; genNum++) { //����generateNumber�γ齱
		//�����������ڳ��ָ��ʵĶ����һ����scene��
		int prob = (int)Level::getRandom(1.0, 100.0);
		for (auto i = this->enemyTypeVec->begin(); i != this->enemyTypeVec->end(); i++) {
			if (i->generateProbability >= prob && i->stageZone.find((char)(48 + this->runningStage)) != std::string::npos) { //48��0��ASCII,��i���������stage���ɣ��ұ�����ʱ
				cocos2d::log(("Info: generating enemy. enemyType=" + i->fighterType).c_str());
				Fighter* e = fReflect(i->fighterType); //�����ȡ����
				if (e == nullptr) cocos2d::log("ERROR: NULLPTR.");
				e->loadFighter(this->configDataLoader, this->saveDataLoader, i->pData,ally::enemy, this->windowSize); //���طɻ�
				float enterTime = Level::getRandom(0.1, 2.9);
				//����adding fighter
				this->waitingList->push_back(e);
				e->retain(); //�漰���ڴ��������,��������ᱻ�Զ���������

				//����һ����ʱ��
				
				this->scheduleOnce(callback, enterTime, "fighterCB_" + std::to_string(this->runningStage) + "_" + std::to_string(this->runningBatch) + "_" + std::to_string(genNum)); //������
			}
		}
	}
	//����batch
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
		cocos2d::Director::getInstance()->getRunningScene()->addChild(wf); //�����˶��ٴξ͵��ö��ٴ�addChild�������ϲ���������
		//ͳһ����tag,���ڲ���,�л���tag�Ǵ�3000��ʼ��
		wf->setTag(this->runningEnemyTag);
		runningEnemyTag++;
		wf->setAutoFire(this->pScene); //�����Ϳ���
		this->waitingList->pop_back();
	}
}

void game::Level::updateBatch()
{
	if (this->runningBatch == 5) {
		runningBatch = 1;
		if (this->runningStage == 9) { //����~
			runningBatch = 0;
			runningStage = 0;
		}
		else {
			runningStage++;
			runningBatch = 1; //����
		}
	}
	else {
		//����ǵ����ظ���batch
		runningBatch++;
	}
}

float game::Level::getRandom(float start, float end) { //���ɣ�start,end���������ڵĸ������������õ�1-100ֵ����start��Ϊ1����end��Ϊ100

	float i = start + (end - start)*rand() / (RAND_MAX + 1.0);
	return i;
}