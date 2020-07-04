#include "util.h"

std::string game::util::getPercent(float x)
{
	return std::to_string((int)(x * 100)) + "%";
}

std::string game::util::shortenNum(float x) //得到保留2位有效数字的x，并以string的形式传出
{
	std::string s;
	std::stringstream ss;
	ss << ss.precision(2) << x;
	ss >> s;
	return s;
}

float game::util::getRandom(float start, float end) { //生成（start,end）开区间内的浮点数，如果想得到1-100值，将start设为1，将end设为100

	float i = start + (end - start)*rand() / (RAND_MAX + 1.0);
	return i;
}

std::string game::util::getFullPath(std::string relatedPath)
{
	char buf[200];
	_fullpath(buf, ".\\", 200);
	return buf + relatedPath;
}
