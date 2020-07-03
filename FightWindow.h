
#ifndef __FIGHTWINDOW_H__
#define __FIGHTWINDOW_H__
#include "cocos2d.h"
#include "Level.h"

namespace game {
	class FightWindow : public cocos2d::Sprite
	{
	public:
		//ͨ��
		FightScene* _scene;
		static FightWindow* _window; //ȫ��ֻ����һ��window
		
		//�����ʾ
		cocos2d::Label* bottomLabel; //��ʾ���ܺ�Ѫ��
		cocos2d::Label* upperLabel; //��ʾս����Ϣ
		//�ṩ���ݽ���,�ӿ���_sceneͳһ�ṩ,
		void update(float f) override;
		//����ʵ��
		static FightWindow* create(FightScene* s); //����һ��
		static FightWindow* create(std::string fileName);
	};
}

#endif __FIGHTWINDOW_H__//__FIGHTWINDOW_H__
