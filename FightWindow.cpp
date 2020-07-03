#include "FightWindow.h"
#include "LanguageManager.h"

game::FightWindow* game::FightWindow::_window = nullptr;

void game::FightWindow::update(float f)
{
	if (bottomLabel == nullptr || upperLabel == nullptr) {
		cocos2d::log("ERROR: update fightwindow without initing label");
		return;
	}
	//��_scene�ɼ�����
	bottomLabel->setString(
		LanguageManager::getInstance()->getStringForKey("shield") + std::to_string((int)_scene->getPlayerShield())
		+ "%\n"
		+ LanguageManager::getInstance()->getStringForKey("playerHealth") + std::to_string((int)_scene->getPlayerHealth())
		+ "%"
	);
}

game::FightWindow * game::FightWindow::create(FightScene* s)
{
	//��������ʼ��ʵ��
	_window = FightWindow::create("UI//UI-fightPanel.png");
	_window->_scene = s;
	//��ʼ��������Ϣ
	_window->bottomLabel = cocos2d::Label::create("------\n------", "fonts//msyh.ttf", 24);
	_window->bottomLabel->setAnchorPoint(cocos2d::Vec2::ZERO);
	_window->bottomLabel->setPosition(6, 30);
	_window->addChild(_window->bottomLabel);
	_window->upperLabel = cocos2d::Label::create("   ", "fonts//msyh.ttf", 18);
	_window->upperLabel->setAnchorPoint(cocos2d::Vec2(0, 1));
	_window->upperLabel->setPosition(6, 768 - 414);
	_window->addChild(_window->upperLabel);
	//ע���Զ�����
	_window->scheduleUpdate();
	return _window;
}

game::FightWindow * game::FightWindow::create(std::string fileName)
{
	FightWindow *sprite = new (std::nothrow) FightWindow();
	if (sprite && sprite->initWithFile(fileName))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;

}
