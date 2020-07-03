/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/


#ifndef __FIGHTSCENE_H__
#define __FIGHTSCENE_H__

#include "cocos2d.h"
#include "DataLoader.h"
#include "Fighter.h"
#include "Weapon.h"
#include "Level.h"
#include "FightWindow.h"
#include "dropIcon.h"
#include <vector>

namespace game {
	class Level;
	class Fighter;
	class FightWindow;
	class FightScene : public cocos2d::Scene
	{
	public:
		/*��Ա*/
		//����ļ���ȡ��:��Ϸ�趨��ȡ���û����ݶ�ȡ
		DataLoader* pGameDataLoader;
		DataLoader* pUserDataLoader;
		//�洢Ψһ����ҷɻ�������Ժ�����Ż�����ô��Ҫ��д��Vector
		Fighter* playerFighter;
		//��ǰ�ؿ���ָ��
		Level* level;
		//����ӵ��ͷɻ��洢�����������е���ʶ��, ע�⣬����ע��Ĳ������еĶ��󣬶���activate�������ú����Ч����
		std::vector<game::Fighter*> *activatedEnemyFighters;
		std::vector<game::Weapon*> *activatedPlayerBullets;
		std::vector<game::Weapon*> *activatedEnemyBullets;
		std::vector<game::dropIcon*> *activatedDropIcons;
		//��ײ��⿪�أ�ÿ��ˢ�¼��
		bool isCollisionDetectionOpen;
		//ս����ϢUI
		FightWindow* fightWindow;
		/*ս����ϢUI�ӿ�*/
		float getPlayerHealth();
		float getPlayerShield();
		int getEnemyFighterCount();
		/*ս����Ϣ���ݴ洢*/
		int destoryedEnemyCount; //��׹��
		int getGemCount; //��õı�ʯ����
		int getWeaponCount;
		/*������ϢUI*/

		/*��ײ�ж�����*/
		enum setFlag { reigster, cancel };
		bool detectCollision(Node * node1, Node * node2);//��ײ�ж�����,2��Node
		bool detectCollision(Node* node1, cocos2d::Vec2 node2pos); //�������ж�
		//�ֲ�update�еļ���ж�
		void EnemyFighterWithPlayerCollision();
		void EnemyBulletWithPlayerCollision();
		void EnemyWithPlayerBulletCollision();
		void dropIconWithPlayerCollision();
		//������ײ�ڽڵ�ע����ע��, ����Ǹ�weapon/fighter��activate��onExit��ʱ����õ�
		bool setEnemyFighters(setFlag f, game::Fighter* e);
		bool setEnemyBullets(setFlag f, game::Weapon* eb);
		bool setPlayerBullets(setFlag f, game::Weapon* pb);
		bool setIcons(setFlag f, game::dropIcon* di);
		//update��ÿ֡������ײ�ж�
		void update(float delta);
		//��д�ķ���
		static cocos2d::Scene* createScene();
		virtual bool init();
		virtual void onEnter();
		CREATE_FUNC(FightScene);
	};
}
#endif __FIGHTSCENE_H__// __FIGHTSCENE_H__
