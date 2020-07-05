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
		//新建开始的scene
		_manager->pStartScene = StartScene::create();
		cocos2d::Director::getInstance()->runWithScene(_manager->pStartScene);
		return;
	}
}

void game::SceneManager::startSceneToStationScene() //默认存档是save_1.txt
{
	//如果已经加载过了
	if (_manager->pStationScene != nullptr) {
		
	}
	//初始化DataLoader
	if (DataLoader::getDataLoader(DataLoaderType::gameDL) == nullptr || DataLoader::getDataLoader(DataLoaderType::saveDL) == nullptr) {
		//文件存取器初始化:userDataLoader和gameDataLoader
		DataLoader::setDataLoader(DataLoaderType::gameDL, new game::DataLoader(std::string("gameData//"), std::string("gameData.txt")));
		DataLoader::setDataLoader(DataLoaderType::saveDL, new game::DataLoader(std::string("save//"), std::string("save_1.txt")));
	}
	_manager->pStationScene = game::stationScene::create();
	_manager->pStationScene->setDataLoader(DataLoader::getDataLoader(DataLoaderType::gameDL), DataLoader::getDataLoader(DataLoaderType::saveDL));
	cocos2d::Director::getInstance()->pushScene(_manager->pStationScene);
}

void game::SceneManager::stationSceneToStartScene()
{
	//压出
	cocos2d::Director::getInstance()->popToRootScene();
}

void game::SceneManager::stationSceneToStarMapScene()
{

}

void game::SceneManager::stationSceneToFightScene()
{
	//压入
	_manager->pFightScene = FightScene::create(1);
	_manager->pFightScene->setDataLoader(DataLoader::getDataLoader(DataLoaderType::gameDL), DataLoader::getDataLoader(DataLoaderType::saveDL));
	cocos2d::Director::getInstance()->pushScene(_manager->pFightScene);
}

void game::SceneManager::fightSceneToStationScene()
{
	//TODO: 这里需要改进
	//如果有starMapScene, 则连续压出两次到stationScene
	cocos2d::Director::getInstance()->popScene();
}
