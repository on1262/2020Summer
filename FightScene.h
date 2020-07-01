/****************************************************************************
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.
 
 http://www.cocos2d-x.org
 
 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:
 
 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.
 
 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#ifndef __FIGHTSCENE_H__
#define __FIGHTSCENE_H__

#include "cocos2d.h"
#include "DataLoader.h"
#include "Fighter.h"
#include "Weapon.h"
#include <vector>


class FightScene : public cocos2d::Scene
{
public:
	//添加文件存取器:游戏设定读取和用户数据读取
	game::DataLoader* pGameDataLoader;
	game::DataLoader* pUserDataLoader;
	//存储唯一的玩家飞机，如果以后添加僚机，那么需要改写成Vector
	game::Fighter* playerFighter;
	//添加子弹和飞机存储器，用来进行敌我识别, 注意，这里注册的不是所有的对象，而是activate（）调用后的有效对象。
	std::vector<game::Fighter*> *activatedEnemyFighters;
	std::vector<game::Weapon*> *activatedPlayerBullets;
	std::vector<game::Weapon*> *activatedEnemyBullets;
	//碰撞检测开关，每次刷新检测
	bool isCollisionDetectionOpen;
	//碰撞判定方法,2个Node
	bool detectCollision(Node * node1, Node * node2);
	//参与碰撞在节点注册与注销, 这个是各weapon/fighter在activate和onExit的时候调用的
	enum setFlag{reigster,cancel};
	bool setEnemyFighters(setFlag f, game::Fighter* e);
	bool setEnemyBullets(setFlag f, game::Weapon* eb);
	bool setPlayerBullets(setFlag f, game::Weapon* pb);
	//update，每帧更新碰撞判定
	void update(float delta);
	//scene
    static cocos2d::Scene* createScene();
	
    virtual bool init();
	virtual void onEnter();
    // a selector callback
    void menuCloseCallback(cocos2d::Ref* pSender);

    // implement the "static create()" method manually
    CREATE_FUNC(FightScene);
};

#endif __FIGHTSCENE_H__// __FIGHTSCENE_H__
