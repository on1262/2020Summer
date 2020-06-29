#ifndef __LEVEL_H__
#define __LEVEL_H__

#include "cocos2d.h"
#include "Weapon.h"
#include "Data.h"
#include "DataLoader.h"
#include "Fighter.h"
#include <vector>
#include <string>
namespace game {
	//����ṹ�����洢3��string��ɵ�enemy��������
	struct enemyType {
		Data* pData; //������һ��Data* ��ʼ�����ʵ��������Data��ʵ����Ӱ���ǵ���ģ�������������
		std::string fighterType;
		std::string stageZone; //�����ڵ�stage,��12345
		int generateProbability; //ȫ�ֳ��ָ���
	};

	/*��̬���ɹؿ������ƽ��ȣ���Ҫ���ӵ�scene���ӽڵ�*/
	class Level : public cocos2d::Node {
	public:
		CREATE_FUNC(Level);
		//���عؿ���Ϣ
		bool loadLevel(DataLoader * saveDL, DataLoader * configDL, int levelID,float playerFactor, cocos2d::Vec2 windowSize);
		void activate(HelloWorld *scene); //��ʼ�ؿ������Ҫ������Level�Ժ���ã�ǰ����Է�һЩ����֮��ģ�
		//ÿ3��Ļص�����
		void batchCallback(float);
		//3���ڵĴ���fighter�ص�����
		void fighterEnterCallback(float);
		//���ߣ�����runningBatch��stage
		void updateBatch();
		//���ߣ��õ�1-100���������
		static float getRandom(float start, float end);
	protected:
		//�ؿ�����ʱ
		DataLoader* saveDataLoader, *configDataLoader;
		int ID; //�ؿ�ID
		int relativeDifficulty;
		int timeDifficulty;
		float playerFactor; //���װ����ս�����Ѷȵ�����
		HelloWorld *pScene;//��ײ�ж��ã����ɻ�ע���
		//***һЩԤ������
		int stageNumber;
		int batchNumber;
		int batchTime;
		//***
		std::vector<enemyType>* enemyTypeVec;
		cocos2d::Vec2 windowSize;
		//�ؿ�����ʱ��Ϣ
		bool isStageStopped; //BOSS����ʱ�ͱ�����ʱ��Ҫ֪ͨlevel
		int runningStage; //��������ܵĽ׶�, 0�����ؿ�����
		int runningBatch; //�����ܵ����Σ�ע��batchȡֵ1-5,��5ʱ��Ҫ����batch�� 0�����ؿ�����
		std::vector<Fighter*> *waitingList; //���ص������ģ���Ҫ���ӵ�enemy 
		int runningEnemyTag; //��3000��ʼ�������µл��ķ���tag,���tag�����Գ���20000
		
	};
}
#endif __LEVEL_H__ //__LEVEL_H__