
#ifndef __PLAYERFIGHTER_H__
#define __PLAYERFIGHTER_H__
#include "cocos2d.h"
#include "Fighter.h"
namespace game {
	/*ע�⣬Fighter�Ǵ�ģ���д���һ��ȫ�µ�ս������������������Ͷ�ȡ��Ϸ��ʵʱ״̬
	Weapon����������config�е����ݾ����ģ�����weapon��������user����Ϸ���ݾ�����*/
	class PlayerFighter : public Fighter
	{
	public:
		//player��ӵ�з����ܵ��趨
		int shield;
		//���ڸ�scene�ļ���
		cocos2d::EventListenerKeyboard* listener;
		CREATE_FUNC(PlayerFighter);
		virtual void onEnter(); //��ʼ��
		void PlayerFighter::fighterStopCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
		void PlayerFighter::fighterMoveCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
		//�����ײ�ĵ���ʶ��
		void setAutoFire(FightScene *scene);
		//��д�˺��ͱ�ը����
		void getDamage(float damage) override;
		void destroyCallback() override;
		//��дͣ������
		void shutdown() override;
	};
}
#endif __PLAYERFIGHTER_H__ //__PLAYERFIGHTER_H__
