#include "util.h"

std::string game::util::getPercent(float x)
{
	return std::to_string((int)(x * 100)) + "%";
}

std::string game::util::shortenNum(float x) //�õ�����2λ��Ч���ֵ�x������string����ʽ����
{
	std::string s;
	std::stringstream ss;
	ss << ss.precision(2) << x;
	ss >> s;
	return s;
}

float game::util::getRandom(float start, float end) { //���ɣ�start,end���������ڵĸ������������õ�1-100ֵ����start��Ϊ1����end��Ϊ100

	float i = start + (end - start)*rand() / (RAND_MAX + 1.0);
	return i;
}

std::string game::util::getFullPath(std::string relatedPath)
{
	char buf[200];
	_fullpath(buf, ".\\", 200);
	return buf + relatedPath;
}
