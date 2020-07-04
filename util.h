#ifndef __UTIL_H__	
#define __UTIL_H__
/*存放一些常用的工具*/
#include <string>
#include <sstream>
#include <stdlib.h>
namespace game {
	class util
	{
	public:
		static std::string getPercent(float x);
		static std::string shortenNum(float x);
		static float getRandom(float start, float end);
		static std::string getFullPath(std::string relatedPath); //相对路径开头是resources内的文件夹名
	};
}

#endif __UTIL_H__
