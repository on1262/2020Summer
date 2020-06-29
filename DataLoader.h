#ifndef __DATALOADER_H__
#define	__DATALOADER_H__

#include <fstream>
#include <string>
#include <vector>
#include "Data.h"
	
namespace game {
	class Data;

	class DataLoader
	{
	public:
		//��Ա
		std::string m_fullPath;
		std::vector<Data*> *loaderVector;
		//����
		DataLoader(std::string path, std::string name);
		~DataLoader(); //ɾ�����е�Data
		Data* findDataByLabel(std::string label);
		bool saveAll();
		bool loadAll();
		bool deleteData(Data *pData);
		static Data* detectSave(std::string path);
		bool addPairByLabel(std::string label, std::string key, std::string value);

		//animationName:�洢Ϊanimation�����֣�pngPreffix:����plistʱpng֡���ֵ�ǰ׺�������·����Suffixͬ��frameCount:��֡����duration����������ʱ��
		static void loadAnimation(std::string animationName, std::string pngPreffix, std::string pngSuffix, int frameCount, float duration, int loops);
		static cocos2d::Sprite* runAnimationByName(std::string name, int tag);
	private:
		std::fstream* fs;
	};
}

#endif __DATALOADER_H__

