#ifndef __FIGHTER_H__
#define __FIGHTER_H__

#include "HelloWorldScene.h"
#include "cocos2d.h"
#include "Weapon.h"
#include "Data.h"
#include <vector>
#include <string>

class HelloWorld;

namespace game {
	enum ally;
	class Weapon; //ǰ������

	/*ע�⣬Fighter�Ǵ�ģ���д���һ��ȫ�µ�ս������������������Ͷ�ȡ��Ϸ��ʵʱ״̬
	Weapon����������config�е����ݾ����ģ�����weapon��������user����Ϸ���ݾ�����*/
	class Fighter : public cocos2d::Sprite
	{
	public:
		//�ɴ����
		std::string texturePath;
		//�ɴ�ʶ��
		std::string fighterType; //��������ʶ�����͵ģ�ͬ���͵���ͬ
		std::string fighterName; //������ʾ����ҵ����֣�ÿ�ܿ��Բ�ͬ
		int fighterID; //��ʱ��ϵͳʶ���ID,�Ƿ���ͬȡ����ϵͳ�趨
		//�ɴ�״̬
		bool isFire; //�Ƿ��ڿ���״̬
		int health; //����ֵ������0�Զ�����
		//�������
		float engineEnergy;
		//����״̬
		std::vector<Weapon*> *portVector; //֪ͨ����Ա���б�
		//���ٺ�Ļ��֣�������дһ��staticԭ��
		static const int score = 1;
		//����
		bool loadFighter(DataLoader* pGameDL, DataLoader* pUserDL, int fighterID, ally wAlly, cocos2d::Vec2 windowSize); //һ��Ҫ�������������������������
		bool loadFighter(DataLoader* pGameDL, DataLoader* pUserDL, Data* fighterData, ally wAlly, cocos2d::Vec2 windowSize); //���أ���Ҫ�Ƿ���ֱ����Data* ��ʼ��
		virtual bool init();
		virtual void onEnter(); //������isLoaded
		virtual void setAutoFire(HelloWorld *scene); //֪ͨ����������,���������������ӳ����Ժ�ִ�У�
		CREATE_FUNC(Fighter);
		virtual void getDamage(float damage);//��ײ����ã��Զ�������Ӧ������ֵ
		virtual void destory(); //���Ҫ�̳У���������
	protected:
		bool isLoaded; //�Ƿ�ִ�й�loadFighter����ֹ����
		Data* config; //�����Լ�������
		DataLoader* pGameDataLoader, *pUserDataLoader;
		cocos2d::Vec2 windowSize; //�������жϷɻ��Ƿ����
		HelloWorld* pScene; //������ײ�ж�
		//���ڵ�����Ӫʶ��
		ally fAlly;
	};
}

#endif __FIGHTER_H__ //__FIGHTER_H__
