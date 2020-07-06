#ifndef __PLANETGENERATOR_H__
#define __PLANETGENERATOR_H__
//png编码解码器，svpng的具体使用见：https://github.com/miloyip/svpng
#include "svpng.h"
//perlin噪音
#include "PerlinNoise.h"
//Simplex噪音
#include "SimplexNoise.h"
#include <map>
#include <string>
//颜色风格
#include "tinter.h"
//其他
#include "cocos2d.h"
/*这个类用于生成循环平面背景图, 不循环平面背景图, 指定大小的球面动图等等 */

class PlanetGenerator
{
public:
	enum cloudQuantity { none, little, medium, large, veryLarge };
	struct cloudPoint { //x,y,size,renderSize
		int x, y, size, renderSize; //size分为1,2,3三种，renderSize是渲染的大小
	};
	PlanetGenerator();
	static PlanetGenerator* _generator;
	/*对外调用的接口*/
	static PlanetGenerator* getInstance(); //获取实例
	/*@param repeatedMapIndex: 用于区分不同关卡的地图序号(对于连续性的地图, 只有一个png)
	*@param scene: 添加背景的background
	*@param ZOrder: 背景添加进scene与其他节点的遮挡关系
	*@param windowSize: 背景的大小, 直接决定生成图片的总大小
	*@param scale: 背景原地图的放大比例, 只能是宽高的公因数, 一般为4
	*@param mapHeight: 背景地图放大后的高度, 要与放大比例整除
	*@param moveSpeed: 地图在scene中每秒移动多少个像素, 像素是以scene为基础的
	*@param bias: 对地图进行随机化的标准 */
	static void setRepeatedPlainMap(bool reGenerate, int repeatedMapIndex,cocos2d::Scene* scene,int ZOrder, cocos2d::Size windowSize, int scale, int mapHeight, float moveSpeed, int bias);
	/*以下部分是工具类*/
	bool generatePlainMap(std::string name,int mapCount, int scale,int bias); //生成mapCount张以name_N.png命名的平面图, 像素放大用scale控制, bias控制随机性
	bool generateRepeatedPlainMap(std::string name, int scale, int bias); //生成前后衔接的一张地图
	bool generateHeightMap(std::string name, int radius, int detail, int frameCount,int cloudFrameCount, int scale, int bias); //由给定参数生成地图，并进行球投影
	bool generateCloud(std::string name, int radius, int cloudHeight, int mapW, int mapH, cloudQuantity quantity, int frameCount, int scale, int bias);//根据指定参数生成云彩平面图
	float * mapProjection(float control, float * grey, int size, int noiseRadius); //control控制生成的密度，类地行星最佳为0.95,越高越接近海洋行星，越低越接近陆地行星
	//地图sprite的回调函数
	void backgroundCallback(cocos2d::Sprite * sprite, float time, int mapHeight);
	//内部的东西
	void setMapSize(int height, int width);//设置地图大小
	void setSeed(unsigned int);
	float getRandom(float start, float end) { //生成（start,end）开区间内的浮点数，如果想得到1-100值，将start设为1，将end设为100
		float i = start + (end - start)*rand() / (RAND_MAX + 1.0);
		return i;
	}
private:
	//地形平展图的大小
	unsigned int mapWidth, mapHeight;
	//地图噪声生成器
	SimplexNoise sn;
	//云彩噪声生成器
	PerlinNoise pn;
	//存储一种颜色风格
	tinter styleTinter;
	//用于存储云彩点阵
	std::vector<cloudPoint>* cloudVec;
};
#endif __PLANETGENERATOR_H__

