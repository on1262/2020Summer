#include "gem.h"
#include "cocos2d.h"


gem * gem::getGem(game::Data * pData)
{
	gem* g = new gem;
	//��ʼ��
	try {
		std::string name = pData->m_label;
		std::string t = pData->findPairByKey("gemType")->value;
		float v = pData->valueConvert<float>(pData->findPairByKey("gemValue")->value);
		int p = pData->valueConvert<int>(pData->findPairByKey("gemPrice")->value);
		//��ֵ
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
	/*�ⲿ���漰����Ϸ��, ����һ���б�õ�������*/
	float linearAverage = 0.0;
	int linearCount = 0;
	bool isAdvancedExist = false;
	for (auto i = vec->begin(); i != vec->end(); i++) { //����linearƽ��ֵ
		if ((*i)->type == "linear") {
			linearAverage += (*i)->value;
			linearCount++;
		} 
		if ((*i)->type == "advanced") {
			isAdvancedExist = true;
		}
	}
	if (linearCount > 0)linearAverage = linearAverage / linearCount;
	else return 0.0; //����Ҫ��һ��linear�������޷��γ�����
	//�����advanced���ͣ���������
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
	//����gem����������
	auto icon = cocos2d::ui::Button::create(gem::getPicturePath(this->type),"");
	icon->setAnchorPoint(cocos2d::Vec2(0.5,0.5));
	this->iconRef = icon; //����ʶ��icon��Ӧ��gem
	return icon;
}

inline float gem::getAdvancedGain(float average)
{
	float f = 0.5;
	if (average <= this->value + 0.01) f += 0.05*(exp(5 * (average - this->value + 1)));
	if (average >= this->value - 0.01) f += 0.05*(exp(5 * ((10 - average) + this->value - 9)));
	return f;
}
