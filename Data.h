#ifndef __DATA_H__
#define	__DATA_H__

#include "cocos2d.h"
#include <string>
#include <sstream>
#include "DataLoader.h"
	
namespace game {
	struct pair { std::string key; std::string value; };
	class DataLoader;
	

	class Data : public std::vector<pair, std::allocator<pair>>
	{
	public:
		DataLoader *pLoader;
		std::string m_label;
		void setLabel(std::string label);
		void setLoader(DataLoader *pLoader);
		pair* findPairByKey(std::string k);
		bool addPair(std::string key, std::string value);
		bool deletePair(std::string key);
		template <typename T> T valueConvert(std::string value) //�����Զ�ת���ĺ���, ����ģ��Ӧ�û᷽��һЩ
		{
			std::stringstream ss(value);
			T result;
			if (typeid(T).name() == typeid(bool).name()) {
				ss >> std::boolalpha >> result; //��bool���������⴦��ʽ
			}
			else {
				ss >> result;
			}
			return result;
		}
	};
}

#endif __DATA_H__ //__DATA_H__
