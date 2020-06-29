#include "DataLoader.h"
#include "Data.h"
#include "cocos2d.h"
#include <string>
#include <stdlib.h>

game::DataLoader::DataLoader(std::string path, std::string name)
{
	std::string fullName = path + name;
	fs = new std::fstream(fullName, std::ios_base::in | std::ios_base::out);
	if (fs->fail()) cocos2d::log("open savefile error!");
	//读取基本信息，并进行解析
	this->m_fullPath = fullName;
	loaderVector = new std::vector<Data*>;
	loadAll();
}

game::DataLoader::~DataLoader()
{
	//释放全部资源
	delete fs; //释放fstream
	fs = nullptr;
	for (auto i = loaderVector->begin(); i != loaderVector->end(); i++)
		delete *i;
	delete loaderVector;
	loaderVector = nullptr;

}

/* find后不检查是否为空指针的话是非常危险的事！！*/
game::Data * game::DataLoader::findDataByLabel(std::string label) //从动态游戏数据中找到data
{
	if (this->loaderVector->empty() == true)
		return nullptr;

	//查找data
	for (auto i = loaderVector->begin(); i != loaderVector->end(); i++) {

		game::Data* pData = *i;
		if (pData->m_label == label)
			return pData;
	}
	cocos2d::log(("Warning: can't find Data* by label:" + label + ". Return nullptr").c_str());
	return nullptr;
}

bool game::DataLoader::saveAll()
{
	if (fs == nullptr || fs->is_open())
		return false;//这些情况不应该出现
	//覆盖同名文件
	fs->open(this->m_fullPath, std::ios_base::out | std::ios_base::trunc);
	if (fs->fail())
		return false;
	std::string line;//缓存
	for (auto i = loaderVector->begin(); i != loaderVector->end(); i++) {
		line = std::string((*i)->m_label);
		fs->write("* ",2);
		fs->write(line.c_str(), line.length()); //写入label
		fs->write("\n", 1);
		for (auto j = (*i)->begin(); j != (*i)->end(); j++) {
			//传入key-value
			pair* p = &(*j);
			line = std::string(p->key);
			line = line + "=" + p->value + "\n";
			fs->write(line.c_str(), line.length());
		}
	}
	fs->close();

	if (fs->good()) return true;
	else return false;
}

/* 将所有数据装包加载,装包形式：
	* datalabel
	key=value
*/
bool game::DataLoader::loadAll()
{
	std::string line;
	while (fs->get() == '*' && fs->get() == ' ') //有新的包出现
	{
		Data* pData = new Data();
		pData->setLoader(this);
		loaderVector->push_back(pData); //压入
		//读取label
		char chrLabel[30];
		fs->getline(chrLabel, 30, '\n');
		line = std::string(chrLabel);
		pData->setLabel(chrLabel);
		cocos2d::log(("Loading label: " + line).c_str()); //调试信息
		//读取键对数据
		fs->clear();//清除状态数据
		while (!fs->fail()) {
			if (fs->get() != '*') {
				fs->unget(); //推出
			}
			else {
				fs->unget(); //推出
				break;
			}
			char chrString[40];
			fs->getline(chrString, 40, '\n');
			line = std::string(chrString);
			int pos = line.find('=');//分割key和value
			if (pos == -1) {
				//这发生在结尾处读取
				if (fs->fail()) cocos2d::log("reach the end of file.");
				else cocos2d::log("error format.");
				break;
			}
			std::string key = std::string(line, 0, pos); //pos是=符号
			std::string value = std::string(line, pos + 1, line.length()); //pos是=符号
		cocos2d::log(("Loading pair: key=" + key + "|value=" + value + "|").c_str()); //调试信息
			pair p = { key, value };
			pData->push_back(p); //应该是值引用
		}
	}
	fs->close();
	return false;
}

bool game::DataLoader::deleteData(Data * pData) //删除Data在loaderVector中的指针，并释放指针指向的堆空间
{
	for (auto i = loaderVector->begin(); i != loaderVector->end(); i++) {
		if (*i == pData) {
			delete *i; //释放Data
			loaderVector->erase(i);
			return true;
		}
	}
	return false; //找不到这样的data
}

game::Data * game::DataLoader::detectSave(std::string path)
{
	return nullptr;
}

bool game::DataLoader::addPairByLabel(std::string label, std::string key, std::string value)
{
	Data* pData = this->findDataByLabel(label);
	if (pData == nullptr || pData->findPairByKey(key) != nullptr) {
		return false; //如果找不到这个label或者已经存在了这个key
	}
	else
	{
		pData->addPair(key, value);
	}
	return false;
}

//loops置-1表示循环播放, preffix需要包括前面的path
void game::DataLoader::loadAnimation(std::string animationName,std::string pngPreffix,std::string pngSuffix,int frameCount, float duration, int loops)
{
	auto anim = cocos2d::Animation::create();
	for (int i = 1; i <= frameCount; i++)
	{
		//char buf[200];
		//_fullpath(buf, ".\\", 200);

		std::string path = "F://Project//2020SummerGame//GameProj//Resources//" + pngPreffix + std::to_string(i) + pngSuffix + ".png";
		cocos2d::log(path.c_str());
		anim->addSpriteFrameWithFile(path);
	}
	anim->setRestoreOriginalFrame(false);
	anim->setLoops(loops);
	anim->setDelayPerUnit(duration / frameCount);
	cocos2d::log("Animation loaded");
	cocos2d::AnimationCache::getInstance()->addAnimation(anim, animationName); //添加到缓存
}

cocos2d::Sprite* game::DataLoader::runAnimationByName(std::string name,int tag) //不需要自行添加runAction, 只要addChild即可
{
	auto anim = cocos2d::AnimationCache::getInstance()->getAnimation(name);
	cocos2d::log(("Duration:" + std::to_string(anim->getDuration())).c_str());
	auto animate = cocos2d::Animate::create(anim);
	cocos2d::Sprite* animSprite = cocos2d::Sprite::create();
	//animSprite->setDisplayFrameWithAnimationName(name,tag);
	animSprite->runAction(animate);
	return animSprite;
	
}
