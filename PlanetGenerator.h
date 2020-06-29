#pragma once

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
/*这个类用来生成一系列png格式的随机行星帧动画，然后提供一个接口实时加载/批量加载给sprite
可以生成球状天体，也可以生成平面地图
 */



class PlanetGenerator
{
public:
	enum cloudQuantity { none, little, medium, large, veryLarge };
	struct cloudPoint { //x,y,size,renderSize
		int x, y, size, renderSize; //size分为1,2,3三种，renderSize是渲染的大小
	};
	PlanetGenerator();
	/*以下部分是工具类*/
	bool generatePlainMap(std::string name, int scale,int bias);
	bool generateHeightMap(std::string name, int radius, int detail, int frameCount,int cloudFrameCount, int scale, int bias); //由给定参数生成地图，并进行球投影
	bool generateCloud(std::string name, int radius, int cloudHeight, int mapW, int mapH, cloudQuantity quantity, int frameCount, int scale, int bias);//根据指定参数生成云彩平面图
	float * mapProjection(float control, float * grey, int size, int noiseRadius); //control控制生成的密度，类地行星最佳为0.95,越高越接近海洋行星，越低越接近陆地行星
	
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

