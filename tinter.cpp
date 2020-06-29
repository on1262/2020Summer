#include "tinter.h"

tinter::tinter() {
	tinter::tinter(std::string("Basic"));
}
tinter::tinter(std::string colorStyle)
{
	if (colorStyle == "earth-like") {
		//风格化颜色
		colorMap["Ice"] = color{ 191,231,231,255 };
		colorMap["Grass"] = color{ 150,207,133,255 };
		colorMap["Sand"] = color{ 223,233,170,255 };
		colorMap["Sea"] = color{ 75,109,133,255 };
		colorMap["Cloud"] = color{ 245,245,212,255 };
	}
	if (colorStyle == "basic") {
//测试基本色
		colorMap["Blue"] = color{ 0,0,255,255 };
		colorMap["Red"] = color{ 255,0,0,255 };
		colorMap["Green"] = color{ 0,255,0 ,255 };
		colorMap["Yellow"] = color{ 255,255,0 ,255 };
		colorMap["White"] = color{ 0,0,0,255 };
	}
		
}

tinter::color tinter::getColor(float grey)
{
	//默认返回white
	//注：这里因为性能原因，不使用find来查找
	if (grey < 0.3) {
		return colorMap["Ice"];
	}
	else if (grey > 0.65 && grey < 0.8) {
		return colorMap["Sand"];
	}
	else if (grey > 0.3 && grey < 0.65) {
		return colorMap["Grass"];
	}
	else if (grey > 0.8) {
		return colorMap["Sea"];
	}
	return colorMap["Cloud"];
}
