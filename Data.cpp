#include "Data.h"

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

bool game::Data::addPair(std::string key, std::string value)
{
	if (this->findPairByKey(key) != nullptr) {
		cocos2d::log("Warning: Pair with the same key failed to be added into Data");
		return false; //如果key冲突
	} else {
		pair p{ key,value };
		this->push_back(p);
		return true;
	}
	return false;
}

bool game::Data::deletePair(std::string key)
{
	if (this->findPairByKey(key) == nullptr)
		return false;
	else {
		pair *p = this->findPairByKey(key);
		for (auto i = this->begin(); i != this->end(); i++)
		{
			if (&(*i) == p) { //如果地址相同
				this->erase(i);
			}
		}
	}
	return false;
}

