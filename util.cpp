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

std::vector<game::Data::weaponAttr> game::util::getFixedAttr(Data* pWeapon)
{
	//���������������
	auto gemStrs = (pWeapon->findPairsByKey("gem"));
	auto gemVec = new std::vector<game::gem*>();

	for (auto i = gemStrs.begin(); i != gemStrs.end(); i++) {
		gemVec->push_back(
			game::gem::getGem(DataLoader::getDataLoader(DataLoaderType::saveDL)->findDataByLabel(*i))
		);
	}
	float gain;
	if (gemStrs.empty() == true) {
		gain = 0; //���û�б�ʯ,������Ϊ0
	}
	else {
		gain = gem::getGain(gemVec);
	}
	cocos2d::log(("INFO: util: Gem gain=" + std::to_string(gain) + "| weapon=" + pWeapon->m_label).c_str());
	//��ȡ������׼ֵ
	auto attrVec = pWeapon->findAttrs();
	for (auto j = attrVec.begin(); j != attrVec.end(); j++) {
		j->attrValue = j->attrValue + j->attrCoef * gain; //�޸�����
	}
	return attrVec;
}
