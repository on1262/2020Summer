#ifndef __DROPICON_H__
#define __DROPICON_H__

#include "cocos2d.h"
#include <string>

namespace game {
	enum iconType{ gemBlue, gemPurple, weaponGrey,weaponRed,weaponPurple,weaponBlue}; 
	enum dropType{gemType, weaponType}; //�����type����Ϊ�����class gem����
	//����ĵ�����,icontype,dt,itemStr,dropZone,probability
	struct dropItem {
		iconType _iconType; //���
		dropType _dropType; //������Ĵ���
		std::string itemStr; //��������gameData�д洢������
		std::string dropZone; //����Ľ׶�
		float probability; //���ָ���(�ٷ���
	};
	//���������ò

	class dropIcon :
		public cocos2d::Sprite
	{
	public:
		dropItem _dropItem;
		static dropIcon* create(dropItem di, cocos2d::Vec2 direction); //����һ��������, ��direction����Ư��
		//����dropType�����Ӧ��ͼ��
		static std::string getPath(iconType dt);
		//���ٵ���
		void destroy();
	};
}

#endif __DROPICON_H__ // !__DROPICON_H__


