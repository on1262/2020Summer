#ifndef __DATALOADER_H__
#define	__DATALOADER_H__

#include <fstream>
#include <string>
#include <vector>
#include "Data.h"
	
namespace game {
	class Data;
	//ȫ�ֻ����õ�ѡ��
	enum DataLoaderType{saveDL,gameDL};
	class DataLoader
	{
	public:
		//��Ա
		std::string m_fullPath;
		std::vector<Data*> *loaderVector;
		//ȫ�ֻ����õķ���
		static DataLoader* saveDataLoader;
		static DataLoader* gameDataLoader;
		//����
		DataLoader(std::string path, std::string name);
		static DataLoader* getDataLoader(DataLoaderType dlt);
		static void setDataLoader(DataLoaderType dlt, DataLoader* dl);
		~DataLoader(); //ɾ�����е�Data
		Data* findDataByLabel(std::string label);
		bool saveAll();
		bool loadAll();
		bool deleteData(Data *pData);
		static Data* detectSave(std::string path);
		bool addPairByLabel(std::string label, std::string key, std::string value);
		int getAvailableDataLabel(std::string preffix); //����preffix��ѯ���ظ���label, ������Ӷ���
		//animationName:�洢Ϊanimation�����֣�pngPreffix:����plistʱpng֡���ֵ�ǰ׺�������·����Suffixͬ��frameCount:��֡����duration����������ʱ��
		static void loadAnimation(std::string animationName, std::string pngPreffix, std::string pngSuffix, int frameCount, float duration, int loops);
		static cocos2d::Sprite* runAnimationByName(std::string name, int tag);
	private:
		std::fstream* fs;
	};
}

#endif __DATALOADER_H__

