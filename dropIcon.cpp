#include "dropIcon.h"

game::dropIcon * game::dropIcon::create(dropItem di, cocos2d::Vec2 direction)
{
	dropIcon *sprite = new (std::nothrow) dropIcon();
	if (sprite && sprite->initWithFile(getPath(di._iconType)))
	{
		//添加到缓存时不会release
		//sprite->autorelease();
		//添加一些内容
		
		sprite->runAction(cocos2d::EaseSineOut::create(cocos2d::MoveBy::create(2.5, cocos2d::Vec3(direction.x, direction.y, 0))));
		sprite->setLocalZOrder(0);
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

std::string game::dropIcon::getPath(game::iconType it)
{
	switch (it)
	{
	case game::gemBlue:
		return std::string("drop//drop-gem-blue.png");
		break;
	case game::gemPurple:
		return std::string("drop//drop-gem-purple.png");
		break;
	case game::weaponGrey:
		return std::string("drop//drop-weapon-grey.png");
		break;
	case game::weaponRed:
		return std::string("drop//drop-weapon-red.png");
		break;
	case game::weaponPurple:
		return std::string("drop//drop-weapon-purple.png");
		break;
	case game::weaponBlue:
		return std::string("drop//drop-weapon-blue.png");
		break;
	default:
		cocos2d::log("ERROR: error type of dropIcon");
		return std::string("ERROR TYPE");
		break;
	}
	return std::string();
}

void game::dropIcon::destroy()
{
	this->removeFromParent();
	this->release(); //销毁
}
