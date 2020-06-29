#pragma once
#include <map>
#include <string>
/*所有地图着色器的基类*/
class tinter 
{
public:
	std::string style; //用于切换风格
	struct color {
		int R, G, B, A;
	};
	tinter(); //默认初始化：basic
	tinter(std::string colorStyle);//根据风格初始化
	//用于存储需要的颜色
	std::map<std::string, color> colorMap;
	color getColor(float grey); //高程着色器
};

