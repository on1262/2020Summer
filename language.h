#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__

#include "cocos2d.h"
#include <string>

/*�����࣬��ȡ�ض��ֶε�����*/
namespace game {
	class language
	{
	public:
		static void init(std::string path);
		static const char* get(std::string key);
		static cocos2d::__Dictionary *dic; //���ڴ洢�ֶ�
	};
}

#endif __LANGUAGE_H__ //__LANGUAGE_H__

