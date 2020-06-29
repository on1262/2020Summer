#ifndef __PLAYERFIGHTER_H__
#define __PLAYERFIGHTER_H__

#include "cocos2d.h"
#include "Weapon.h"
#include "Data.h"
#include "Fighter.h"
#include <vector>
#include <string>

namespace game {
	class Weapon; //ǰ������

	/*ע�⣬Fighter�Ǵ�ģ���д���һ��ȫ�µ�ս������������������Ͷ�ȡ��Ϸ��ʵʱ״̬
	Weapon����������config�е����ݾ����ģ�����weapon��������user����Ϸ���ݾ�����*/
	class PlayerFighter : public Fighter
	{
	public:
		CREATE_FUNC(PlayerFighter);
		virtual void onEnter(); //������д���WASD����
		void PlayerFighter::fighterStopCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
		void PlayerFighter::fighterMoveCallback(cocos2d::EventKeyboard::KeyCode keyCode, cocos2d::Event* event);
		//�����ײ�ĵ���ʶ��
		void setAutoFire(HelloWorld *scene);
	};
}
#endif __PLAYERFIGHTER_H__ //__PLAYERFIGHTER_H__
