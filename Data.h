#ifndef __DATA_H__
#define	__DATA_H__

#include "cocos2d.h"
#include <string>
#include <sstream>
#include <vector>
#include "DataLoader.h"
	
namespace game {
	struct pair { std::string key; std::string value; };
	class DataLoader;
	

	class Data : public std::vector<pair, std::allocator<pair>>
	{
	public:
		struct weaponAttr { //武器的属性，从文档中提取
			std::string attrStr; //显示的属性名（中文）
			std::string attrName; //记录的属性名
			float attrValue; //记录的属性值
			float attrCoef;//增益灵敏度
		};
		DataLoader *pLoader;
		std::string m_label;
		void setLabel(std::string label);
		void setLoader(DataLoader *pLoader);
		pair* findPairByKey(std::string k);
		std::vector<weaponAttr> findAttrs();
		bool addPair(std::string key, std::string value);
		bool deletePair(std::string key, bool isDeleteAll);
		bool deletePair(std::string key, std::string value, bool isDeleteAll);//删除具有特定key和value的数据
		std::vector<std::string> findPairsByKey(std::string key); //找到同一个key的所有值, 注意，这里是值复制
		template <typename T> T valueConvert(std::string value) //这是自动转换的函数, 用了模版应该会方便一些
		{
			std::stringstream ss(value);
			T result;
			if (typeid(T).name() == typeid(bool).name()) {
				ss >> std::boolalpha >> result; //对bool类型有特殊处理方式
			}
			else {
				ss >> result;
			}
			return result;
		}
	};
}

#endif __DATA_H__ //__DATA_H__
