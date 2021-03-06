#ifndef __DATALOADER_H__
#define	__DATALOADER_H__

#include <fstream>
#include <string>
#include <vector>
#include "Data.h"
	
namespace game {
	class Data;
	//全局化调用的选项
	enum DataLoaderType{saveDL,gameDL};
	class DataLoader
	{
	public:
		//成员
		std::string m_fullPath;
		std::vector<Data*> *loaderVector;
		//全局化调用的方法
		static DataLoader* saveDataLoader;
		static DataLoader* gameDataLoader;
		//方法
		DataLoader(std::string path, std::string name);
		static DataLoader* getDataLoader(DataLoaderType dlt);
		static void setDataLoader(DataLoaderType dlt, DataLoader* dl);
		~DataLoader(); //删除所有的Data
		Data* findDataByLabel(std::string label);
		bool saveAll();
		bool loadAll();
		bool deleteData(Data *pData);
		static Data* detectSave(std::string path);
		bool addPairByLabel(std::string label, std::string key, std::string value);
		int getAvailableDataLabel(std::string preffix); //根据preffix查询不重复的label, 用于添加东西
		//animationName:存储为animation的名字，pngPreffix:制作plist时png帧数字的前缀，带相对路径，Suffix同理，frameCount:总帧数，duration：动画持续时间
		static void loadAnimation(std::string animationName, std::string pngPreffix, std::string pngSuffix, int frameCount, float duration, int loops);
		static cocos2d::Sprite* runAnimationByName(std::string name, int tag);
	private:
		std::fstream* fs;
	};
}

#endif __DATALOADER_H__

