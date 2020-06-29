#ifndef __LANGUAGE_H__
#define __LANGUAGE_H__

#include "cocos2d.h"
#include <string>

/*工具类，获取特定字段的中文*/
namespace game {
	class language
	{
	public:
		static void init(std::string path);
		static const char* get(std::string key);
		static cocos2d::__Dictionary *dic; //用于存储字段
	};
}

#endif __LANGUAGE_H__ //__LANGUAGE_H__

