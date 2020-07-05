#ifndef __UTIL_H__	
#define __UTIL_H__
/*���һЩ���õĹ���*/
#include <string>
#include <sstream>
#include <stdlib.h>
#include "DataLoader.h"
#include "gem.h"
#include <vector>
namespace game {
	class util
	{
	public:
		static std::string getPercent(float x);
		static std::string shortenNum(float x);
		static float getRandom(float start, float end);
		static std::string getFullPath(std::string relatedPath); //���·����ͷ��resources�ڵ��ļ�����
		//���ַ�������ʵ��,Ȼ�����
		static std::vector<Data::weaponAttr> getFixedAttr(Data* pWeapon);
	};
}

#endif __UTIL_H__