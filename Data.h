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
		struct weaponAttr { //���������ԣ����ĵ�����ȡ
			std::string attrStr; //��ʾ�������������ģ�
			std::string attrName; //��¼��������
			float attrValue; //��¼������ֵ
			float attrCoef;//����������
		};
		DataLoader *pLoader;
		std::string m_label;
		void setLabel(std::string label);
		void setLoader(DataLoader *pLoader);
		pair* findPairByKey(std::string k);
		std::vector<weaponAttr> findAttrs();
		bool addPair(std::string key, std::string value);
		bool deletePair(std::string key, bool isDeleteAll);
		bool deletePair(std::string key, std::string value, bool isDeleteAll);//ɾ�������ض�key��value������
		std::vector<std::string> findPairsByKey(std::string key); //�ҵ�ͬһ��key������ֵ, ע�⣬������ֵ����
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
