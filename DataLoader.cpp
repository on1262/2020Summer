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
	//��ȡ������Ϣ�������н���
	this->m_fullPath = fullName;
	loaderVector = new std::vector<Data*>;
	loadAll();
}

game::DataLoader::~DataLoader()
{
	//�ͷ�ȫ����Դ
	delete fs; //�ͷ�fstream
	fs = nullptr;
	for (auto i = loaderVector->begin(); i != loaderVector->end(); i++)
		delete *i;
	delete loaderVector;
	loaderVector = nullptr;

}

/* find�󲻼���Ƿ�Ϊ��ָ��Ļ��Ƿǳ�Σ�յ��£���*/
game::Data * game::DataLoader::findDataByLabel(std::string label) //�Ӷ�̬��Ϸ�������ҵ�data
{
	if (this->loaderVector->empty() == true)
		return nullptr;

	//����data
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
		return false;//��Щ�����Ӧ�ó���
	//����ͬ���ļ�
	fs->open(this->m_fullPath, std::ios_base::out | std::ios_base::trunc);
	if (fs->fail())
		return false;
	std::string line;//����
	for (auto i = loaderVector->begin(); i != loaderVector->end(); i++) {
		line = std::string((*i)->m_label);
		fs->write("* ",2);
		fs->write(line.c_str(), line.length()); //д��label
		fs->write("\n", 1);
		for (auto j = (*i)->begin(); j != (*i)->end(); j++) {
			//����key-value
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

/* ����������װ������,װ����ʽ��
	* datalabel
	key=value
*/
bool game::DataLoader::loadAll()
{
	std::string line;
	while (fs->get() == '*' && fs->get() == ' ') //���µİ�����
	{
		Data* pData = new Data();
		pData->setLoader(this);
		loaderVector->push_back(pData); //ѹ��
		//��ȡlabel
		char chrLabel[30];
		fs->getline(chrLabel, 30, '\n');
		line = std::string(chrLabel);
		pData->setLabel(chrLabel);
		cocos2d::log(("Loading label: " + line).c_str()); //������Ϣ
		//��ȡ��������
		fs->clear();//���״̬����
		while (!fs->fail()) {
			if (fs->get() != '*') {
				fs->unget(); //�Ƴ�
			}
			else {
				fs->unget(); //�Ƴ�
				break;
			}
			char chrString[40];
			fs->getline(chrString, 40, '\n');
			line = std::string(chrString);
			int pos = line.find('=');//�ָ�key��value
			if (pos == -1) {
				//�ⷢ���ڽ�β����ȡ
				if (fs->fail()) cocos2d::log("reach the end of file.");
				else cocos2d::log("error format.");
				break;
			}
			std::string key = std::string(line, 0, pos); //pos��=����
			std::string value = std::string(line, pos + 1, line.length()); //pos��=����
		cocos2d::log(("Loading pair: key=" + key + "|value=" + value + "|").c_str()); //������Ϣ
			pair p = { key, value };
			pData->push_back(p); //Ӧ����ֵ����
		}
	}
	fs->close();
	return false;
}

bool game::DataLoader::deleteData(Data * pData) //ɾ��Data��loaderVector�е�ָ�룬���ͷ�ָ��ָ��Ķѿռ�
{
	for (auto i = loaderVector->begin(); i != loaderVector->end(); i++) {
		if (*i == pData) {
			delete *i; //�ͷ�Data
			loaderVector->erase(i);
			return true;
		}
	}
	return false; //�Ҳ���������data
}

game::Data * game::DataLoader::detectSave(std::string path)
{
	return nullptr;
}

bool game::DataLoader::addPairByLabel(std::string label, std::string key, std::string value)
{
	Data* pData = this->findDataByLabel(label);
	if (pData == nullptr || pData->findPairByKey(key) != nullptr) {
		return false; //����Ҳ������label�����Ѿ����������key
	}
	else
	{
		pData->addPair(key, value);
	}
	return false;
}

//loops��-1��ʾѭ������, preffix��Ҫ����ǰ���path
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
	cocos2d::AnimationCache::getInstance()->addAnimation(anim, animationName); //��ӵ�����
}

cocos2d::Sprite* game::DataLoader::runAnimationByName(std::string name,int tag) //����Ҫ�������runAction, ֻҪaddChild����
{
	auto anim = cocos2d::AnimationCache::getInstance()->getAnimation(name);
	cocos2d::log(("Duration:" + std::to_string(anim->getDuration())).c_str());
	auto animate = cocos2d::Animate::create(anim);
	cocos2d::Sprite* animSprite = cocos2d::Sprite::create();
	//animSprite->setDisplayFrameWithAnimationName(name,tag);
	animSprite->runAction(animate);
	return animSprite;
	
}
