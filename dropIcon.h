#ifndef __DROPICON_H__
#define __DROPICON_H__

#include "cocos2d.h"
#include <string>

namespace game {
	enum iconType{ gemBlue, gemPurple, weaponGrey,weaponRed,weaponPurple,weaponBlue}; 
	enum dropType{gemType, weaponType}; //这里加type是因为避免和class gem重名
	//抽象的掉落物,icontype,dt,itemStr,dropZone,probability
	struct dropItem {
		iconType _iconType; //外观
		dropType _dropType; //掉落物的大类
		std::string itemStr; //掉落物在gameData中存储的类型
		std::string dropZone; //掉落的阶段
		float probability; //出现概率(百分制
	};
	//掉落物的样貌

	class dropIcon :
		public cocos2d::Sprite
	{
	public:
		dropItem _dropItem;
		static dropIcon* create(dropItem di, cocos2d::Vec2 direction); //生成一个掉落物, 沿direction方向漂浮
		//根据dropType获得相应的图标
		static std::string getPath(iconType dt);
		//销毁调用
		void destroy();
	};
}

#endif __DROPICON_H__ // !__DROPICON_H__


