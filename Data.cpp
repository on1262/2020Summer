#include "Data.h"
#include <vector>
#include "LanguageManager.h"
void game::Data::setLabel(std::string label)
{
	this->m_label = label;
}

void game::Data::setLoader(game::DataLoader * pLoader)
{
	this->pLoader = pLoader;
}

game::pair * game::Data::findPairByKey(std::string k) {
	for (iterator i = this->begin(); i != this->end(); i++) {
		if ((*i).key == k) return &(*i);
	}
	return nullptr;
}

std::vector<game::Data::weaponAttr> game::Data::findAttrs()
{
	std::vector<weaponAttr> vec;
	auto attrs = this->findPairsByKey("attribute");
	for (auto i = attrs.begin(); i != attrs.end(); i++) {
		std::string name = *i; //名字
		std::string showName = LanguageManager::getInstance()->getStringForKey(*i);//译名
		float value;
		float coef;
		for (auto j = this->begin(); j != this->end(); j++) {
			if (j->key == "attr_" + *i) {
				value = this->valueConvert<float>(j->value);
			}
			if (j->key == "coef_" + *i) {
				coef = this->valueConvert<float>(j->value);
			}
		}
		vec.push_back(game::Data::weaponAttr{ showName,name, value, coef });
	}
	return vec;
}

bool game::Data::addPair(std::string key, std::string value)
{
	pair p{ key,value };
	this->push_back(p);
	return true;
}

bool game::Data::deletePair(std::string key, bool isDeleteAll)
{
	if (this->findPairByKey(key) == nullptr)
		return false;
	else {
		pair *p = this->findPairByKey(key);
		for (auto i = this->begin(); i != this->end(); i++)
		{
			if (&(*i) == p) { //如果地址相同
				this->erase(i);
				if (isDeleteAll == false) {
					return true; //如果只删除一个，就退出
				}
			}
		}
	}
	return true;
}

bool game::Data::deletePair(std::string key, std::string value, bool isDeleteAll) {
	if (this->findPairByKey(key) == nullptr)
		return false;
	else {
		for (auto i = this->begin(); i != this->end(); i++) {
			if (i->key == key && i->value == value) {
				this->erase(i);
				if (isDeleteAll == false) {
					return true;
				}
			}
		}
	}
}

std::vector<std::string> game::Data::findPairsByKey(std::string key)
{
	vector<std::string> vec;
	for (iterator i = this->begin(); i != this->end(); i++) {
		if ((*i).key == key) vec.push_back(i->value);
	}

	if (vec.empty() == false) return vec;
	else {
		cocos2d::log("Error: find no pairs in Data");
		return vec;
	}
}