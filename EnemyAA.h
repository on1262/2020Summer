

#ifndef __ENEMYAA_H__
#define __ENEMYAA_H__

#include "cocos2d.h"
#include "Weapon.h"
#include "Data.h"
#include "Fighter.h"
#include <vector>
#include <string>

namespace game {
	class EnemyAA : public Fighter
	{
	public:
		//��������ʱ�������һЩ���������ҷ���Ҳ���෴��
		void setAutoFire(FightScene *scene);
		void getDamage(float damage) override;
		void destroyCallback() override;
		CREATE_FUNC(EnemyAA);
		static float getRandom(float start, float end);
	};
}

#endif __ENEMYAA_H__ //__ENEMYAA_H__