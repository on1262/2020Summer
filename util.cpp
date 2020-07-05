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

std::vector<game::Data::weaponAttr> game::util::getFixedAttr(Data* pWeapon)
{
	//获得武器的总增益
	auto gemStrs = (pWeapon->findPairsByKey("gem"));
	auto gemVec = new std::vector<game::gem*>();

	for (auto i = gemStrs.begin(); i != gemStrs.end(); i++) {
		gemVec->push_back(
			game::gem::getGem(DataLoader::getDataLoader(DataLoaderType::saveDL)->findDataByLabel(*i))
		);
	}
	float gain;
	if (gemStrs.empty() == true) {
		gain = 0; //如果没有宝石,则增益为0
	}
	else {
		gain = gem::getGain(gemVec);
	}
	cocos2d::log(("INFO: util: Gem gain=" + std::to_string(gain) + "| weapon=" + pWeapon->m_label).c_str());
	//获取武器基准值
	auto attrVec = pWeapon->findAttrs();
	for (auto j = attrVec.begin(); j != attrVec.end(); j++) {
		j->attrValue = j->attrValue + j->attrCoef * gain; //修改增益
	}
	return attrVec;
}
