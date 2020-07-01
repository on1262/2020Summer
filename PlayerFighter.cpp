#include "Fighter.h"
#include "Weapon.h"
#include "SimpleAudioEngine.h"
#include "PlayerFighter.h"
#include "cocos2d.h"

USING_NS_CC;

//WASD�ƶ���ʵ��1
void game::PlayerFighter::fighterMoveCallback(EventKeyboard::KeyCode keyCode, Event* event)
{
	auto fighter = this;
	//Calculate the distance from fighter to boarder
	float wDistance = this->windowSize.y - fighter->getPositionY();
	auto actionW = EaseOut::create(MoveBy::create(0.005 * wDistance, Vec3(0, wDistance, 0)), 0.8);
	float sDistance = fighter->getPositionY();
	auto actionS = EaseOut::create(MoveBy::create(0.005 * sDistance, Vec3(0, -1.0 * sDistance, 0)), 0.8);
	float aDistance = fighter->getPositionX();
	auto actionA = EaseOut::create(MoveBy::create(0.005 * aDistance, Vec3(-1.0 * aDistance, 0, 0)), 0.8);
	float dDistance = this->windowSize.x - fighter->getPositionX();
	auto actionD = EaseOut::create(MoveBy::create(0.005 * dDistance, Vec3(dDistance, 0, 0)), 0.8);

	switch (keyCode) {
	case EventKeyboard::KeyCode::KEY_W:
		log("runW");
		actionW->setTag(1);
		fighter->stopActionByTag(5);
		fighter->runAction(actionW);
		break;
	case EventKeyboard::KeyCode::KEY_S:
		actionS->setTag(2);
		fighter->stopActionByTag(6);
		fighter->runAction(actionS);
		break;
	case EventKeyboard::KeyCode::KEY_A:
		actionA->setTag(3);
		fighter->stopActionByTag(7);
		fighter->runAction(actionA);
		break;
	case EventKeyboard::KeyCode::KEY_D:
		actionD->setTag(4);
		fighter->stopActionByTag(8);
		fighter->runAction(actionD);
		break;
	}
}

void game::PlayerFighter::setAutoFire(FightScene * scene)
{
	//��Ϊ��ҵķɻ���ֱ�����ΪFighter*�ģ����Բ���Ҫ������ʶ��
	Fighter::setAutoFire(scene);
}

//WASD�ƶ���ʵ��2
void game::PlayerFighter::fighterStopCallback(EventKeyboard::KeyCode keyCode, Event* event)
{
	auto fighter = this;
	//ע���������һ֡��ʼ
	if (keyCode == EventKeyboard::KeyCode::KEY_W) {
		fighter->stopActionByTag(1);
		log("stopW");
		auto stopW = EaseIn::create(MoveBy::create(0.8f, Vec3(0, 10, 0)), 1);
		stopW->setTag(5);
		fighter->runAction(stopW);
		return;
	}
	if (fighter->getActionByTag(2) != nullptr && keyCode == EventKeyboard::KeyCode::KEY_S) {
		fighter->stopActionByTag(2);
		auto stopS = EaseIn::create(MoveBy::create(0.8f, Vec3(0, -10, 0)), 1);
		stopS->setTag(6);
		fighter->runAction(stopS);
		return;
	}
	if (fighter->getActionByTag(3) != nullptr && keyCode == EventKeyboard::KeyCode::KEY_A) {
		fighter->stopActionByTag(3);
		auto stopA = EaseIn::create(MoveBy::create(0.8f, Vec3(-10, 0, 0)), 1);
		stopA->setTag(7);
		fighter->runAction(stopA);
		return;
	}
	if (fighter->getActionByTag(4) != nullptr && keyCode == EventKeyboard::KeyCode::KEY_D) {
		fighter->stopActionByTag(4);
		auto stopD = EaseIn::create(MoveBy::create(0.8f, Vec3(10, 0, 0)), 1);
		stopD->setTag(8);
		fighter->runAction(stopD);
		return;
	}
}

void game::PlayerFighter::onEnter()
{
	Sprite::onEnter();
	//PlayerFighter������ģ��̶���tag=1000,����ȫ����ʶ��
	this->setTag(1000);
	//������Ӽ�����
	auto listener = EventListenerKeyboard::create();
	listener->onKeyPressed = CC_CALLBACK_2(PlayerFighter::fighterMoveCallback, this);
	listener->onKeyReleased = CC_CALLBACK_2(PlayerFighter::fighterStopCallback, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this->getParent());
}