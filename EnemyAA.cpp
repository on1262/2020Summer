#include "cocos2d.h"
#include "EnemyAA.h"

USING_NS_CC;

void game::EnemyAA::setAutoFire(FightScene *scene)
{
	//���ø��෽��(��������)
	Fighter::setAutoFire(scene);
	//�����ײ�ж����л�
	scene->setEnemyFighters(FightScene::setFlag::reigster, this);
	//��������ת180��(��Ϊ����player�����෴
	this->setRotation(180.0);
	//��������ٲ�ʽ����
	this->setPosition(getRandom(0, windowSize.x), windowSize.y);
	//���һ���ӵ�ǰ���ӹ��Ķ���~
	this->runAction(
		cocos2d::Sequence::create(
			cocos2d::EaseIn::create(cocos2d::MoveBy::create(2, cocos2d::Vec2(0, -1 * windowSize.y)), 1.0),
			cocos2d::CallFunc::create(CC_CALLBACK_0(EnemyAA::destroy, this)),
			0
		)
	);
}

void game::EnemyAA::getDamage(float damage)
{
	health -= damage;
	if (health <= 0) this->destroy();
}

void game::EnemyAA::destroyCallback()
{
	if (health <= 0) { //ֻ���ڱ�����ʱ�ű�������, �Զ��˳�ʱû��
		//������ʯ������
		pScene->level->generateDropItem(2, this->getPosition());
		//��ɱ����1
		this->pScene->addDestroyedEnemyCount(1);
	}
}

float game::EnemyAA::getRandom(float start, float end) { //���ɣ�start,end���������ڵĸ������������õ�1-100ֵ����start��Ϊ1����end��Ϊ100

	float i = start + (end - start)*rand() / (RAND_MAX + 1.0);
	return i;
}
