#include "SceneManager.h"

game::SceneManager* game::SceneManager::_manager = nullptr;

game::SceneManager * game::SceneManager::getInstance()
{
	if (_manager != nullptr) return _manager;
	else {
		_manager = new SceneManager();
		return _manager;
	}
	return nullptr;
}

void game::SceneManager::loadFirstScene(cocos2d::Scene * testScene)
{
	if (testScene != nullptr) {
		cocos2d::Director::getInstance()->runWithScene(testScene);
		return;
	}
	else {
		//�½���ʼ��scene
		_manager->pStartScene = StartScene::create();
		cocos2d::Director::getInstance()->runWithScene(_manager->pStartScene);
		return;
	}
}

void game::SceneManager::startSceneToStationScene() //Ĭ�ϴ浵��save_1.txt
{
	//����Ѿ����ع���
	if (_manager->pStationScene != nullptr) {
		
	}
	//��ʼ��DataLoader
	if (DataLoader::getDataLoader(DataLoaderType::gameDL) == nullptr || DataLoader::getDataLoader(DataLoaderType::saveDL) == nullptr) {
		//�ļ���ȡ����ʼ��:userDataLoader��gameDataLoader
		DataLoader::setDataLoader(DataLoaderType::gameDL, new game::DataLoader(std::string("gameData//"), std::string("gameData.txt")));
		DataLoader::setDataLoader(DataLoaderType::saveDL, new game::DataLoader(std::string("save//"), std::string("save_1.txt")));
	}
	_manager->pStationScene = game::stationScene::create();
	_manager->pStationScene->setDataLoader(DataLoader::getDataLoader(DataLoaderType::gameDL), DataLoader::getDataLoader(DataLoaderType::saveDL));
	cocos2d::Director::getInstance()->pushScene(_manager->pStationScene);
}

void game::SceneManager::stationSceneToStartScene()
{
	//ѹ��
	cocos2d::Director::getInstance()->popToRootScene();
}

void game::SceneManager::stationSceneToStarMapScene()
{

}

void game::SceneManager::stationSceneToFightScene()
{
	//ѹ��
	_manager->pFightScene = FightScene::create(1);
	_manager->pFightScene->setDataLoader(DataLoader::getDataLoader(DataLoaderType::gameDL), DataLoader::getDataLoader(DataLoaderType::saveDL));
	cocos2d::Director::getInstance()->pushScene(_manager->pFightScene);
}

void game::SceneManager::fightSceneToStationScene()
{
	//TODO: ������Ҫ�Ľ�
	//�����starMapScene, ������ѹ�����ε�stationScene
	cocos2d::Director::getInstance()->popScene();
}
