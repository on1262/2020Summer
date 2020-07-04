/*!
	Weapon���ࣺ�ṩ������ɴ���ͨ�Žӿڣ���Ϣ��ȡ�淶���ӵ�ʵ������
	* ������������̳�Weapon
	* �ӵ��뼤�������������ӽڵ㣬������������ʱ���ӵ��ͼ���һ������
*/
#ifndef __WEAPON_H__
#define __WEAPON_H__

#include "FightScene.h"
#include "cocos2d.h"
#include "Data.h"
#include <string>

namespace game {
	class FightScene;
	enum ally{player, enemy}; //������Ӫ�жϣ������Ϊ����Fighter/player���Ͽɵ�
	class Fighter; //ǰ������
	class Weapon : public cocos2d::Sprite {
	public:
		//����ʶ��
		std::string weaponType; //���ڼ���Ƿ�ɰ��ŵ������
		//����ʶ��
		ally weaponAlly;
		//����״̬
		bool isDamaged; //�Ƿ����
		bool isFire; //�Ƿ��ڿ���״̬

		//����ӵ��ɴ���
		virtual void onEnter();
		//���������ǰ�������loadWeapon��������ʼ������������,windowsSize�����ж������Ƿ񳬳��߿�����ж�Ҳ��������Ӱ���
		virtual bool loadWeapon(Data* pConfig,Data* pData, ally wAlly, cocos2d::Vec2 windowSize);
		//������������˺�ģʽ,����ʱ��Ҫ���ø�����
		virtual void activate(FightScene *scene);
		//����ֹͣ����
		virtual void shutDown();
		CREATE_FUNC(Weapon);
	protected:
		bool isLoaded; //�Ƿ���enterǰ���ù�loadWeapon�����û�У������������Ϣ
		Data* config; //�����Լ����Ƿ�Data
		std::string texturePath;
		cocos2d::Vec2 windowSize;
		FightScene *pScene;//��ײ�ж��ã����ӵ��ӵ�
	};

}

#endif __WEAPON_H__ //__WEAPON_H__