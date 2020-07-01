#include "gem.h"
#include "cocos2d.h"


gem * gem::getGem(game::Data * pData)
{
	gem* g = new gem;
	//初始化
	try {
		std::string name = pData->m_label;
		std::string t = pData->findPairByKey("gemType")->value;
		float v = pData->valueConvert<float>(pData->findPairByKey("gemValue")->value);
		int p = pData->valueConvert<int>(pData->findPairByKey("gemPrice")->value);
		//赋值
		g->instanceName = name;
		g->type = t;
		g->value = v;
		g->price = p;
	}
	catch (...) {
		cocos2d::log("Error: Failed in loading gem");
	}
	return g;

}

float gem::getGain(std::vector<gem*>* vec)
{
	/*这部分涉及到游戏性, 根据一个列表得到总增益*/
	float linearAverage = 0.0;
	int linearCount = 0;
	bool isAdvancedExist = false;
	for (auto i = vec->begin(); i != vec->end(); i++) { //计算linear平均值
		if ((*i)->type == "linear") {
			linearAverage += (*i)->value;
			linearCount++;
		} 
		if ((*i)->type == "advanced") {
			isAdvancedExist = true;
		}
	}
	if (linearCount > 0)linearAverage = linearAverage / linearCount;
	else return 0.0; //至少要有一个linear，否则无法形成增益
	//如果有advanced类型，计算增益
	float advancedGain = 0.0;
	if (isAdvancedExist == true) {
		for (auto i = vec->begin(); i != vec->end(); i++) {
			if ((*i)->type == "advanced") {
				advancedGain += (*i)->getAdvancedGain(linearAverage);
			}
		}
		return advancedGain; 
	}
	else {
		return 0.1 * linearAverage;
	}
	return 0.0f;
}

std::string gem::getPicturePath(std::string type)
{
	std::string gemPath("gem//");
	if (type.empty() == true) {
		cocos2d::log("Error: call getPicturePath() without initing gem");
		return std::string("errorTypeGem");
	}
	if (type == "linear") {
		return gemPath + "gem-blue.png";
	}
	if (type == "advanced") {
		return gemPath + "gem-purple.png";
	}
	cocos2d::log("Error: getGemPath with undefined type");
	return std::string("no founded type");
}

gem::gem()
{
	this->iconRef = nullptr;
}

cocos2d::ui::Button* gem::getGemIcon()
{
	//按照gem的类型区分
	auto icon = cocos2d::ui::Button::create(gem::getPicturePath(this->type),"");
	icon->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
	this->iconRef = icon; //用于识别icon对应的gem
	return icon;
}

inline float gem::getAdvancedGain(float average)
{
	float f = 0.5;
	if (average <= this->value + 0.01) f += 0.05*(exp(5 * (average - this->value + 1)));
	if (average >= this->value - 0.01) f += 0.05*(exp(5 * ((10 - average) + this->value - 9)));
	return f;
}
