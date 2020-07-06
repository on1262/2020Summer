#include "PlanetGenerator.h"
#include <iostream>
#include "cocos2d.h"
#include <cmath>
#include <random>

#define _USE_MATH_DEFINES

PlanetGenerator* PlanetGenerator::_generator = nullptr;

PlanetGenerator::PlanetGenerator()
{
	//初始化参数
	mapHeight = 128;
	mapWidth = 256;
	//加载必要的颜色参数
	styleTinter = tinter(std::string("earth-like"));
	//初始化
	sn = SimplexNoise(); //使用默认参数初始化pn
	pn = PerlinNoise();
	//初始化随机数生成器(这行要改
	srand(123);
}

PlanetGenerator * PlanetGenerator::getInstance()
{
	if (_generator == nullptr) {
		_generator = new PlanetGenerator();
	}
	return _generator;
}

bool PlanetGenerator::generatePlainMap(std::string name,int mapCount,int scale, int bias)
{
	float detail = 0.01; //控制生成地形的宏观程度（越大则生成越跳跃，越宏观）
	float fBiasX = (float)bias * mapWidth * detail; //设置偏移值
	float fBiasY = (float)bias * mapHeight * detail;
	//根据mapHeight和mapWidth生成指定大小的地图，然后放大scale倍
	cocos2d::log("generating plain map: initing...");
	//打开文件
	FILE* fp = fopen(("plainMap\\" + name + ".png").c_str(), "wb");
	//生成图像
	unsigned char* pScaledMap = new unsigned char[4 * scale * scale * mapHeight * mapWidth];
	//生成小图
	float* pUnScaledMap = new float[mapHeight * mapWidth];
	unsigned char* p = &pScaledMap[0];
	float* up = &pUnScaledMap[0];
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			*up++ = 0.8 - 0.5 * sn.fractal(8, (float)i * detail + fBiasY, (float)j * detail + fBiasX);
		}
	}
	for (int i = 0; i < mapHeight * scale; i++) {
		for (int j = 0; j < mapWidth * scale; j++) {
			tinter::color c = styleTinter.getColor(pUnScaledMap[(i / scale) * mapWidth + j / scale]);
			*p++ = c.R;
			*p++ = c.G;
			*p++ = c.B;
			*p++ = c.A;
		}
	}
	//写入文件
	svpng(fp, mapWidth * scale, mapHeight * scale, pScaledMap, 1);
	//返回内存
	delete pUnScaledMap;
	delete pScaledMap;
	fclose(fp);
	return true;
}

void PlanetGenerator::setRepeatedPlainMap(bool reGenerate, int repeatedMapIndex, cocos2d::Scene * scene, int ZOrder, cocos2d::Size windowSize, int scale, int mapHeight, float moveSpeed, int bias)
{
	//设置高宽, 地图宽度略大于设定宽度,这是为了避免出现缝隙
	_generator->setMapSize(((mapHeight+8) / scale), windowSize.width / scale);
	//生成地图
	cocos2d::log("INFO: generating repeated map...");
	if (reGenerate == true) {
		_generator->generateRepeatedPlainMap("repeatedPlainMap_" + std::to_string(repeatedMapIndex), 4, bias);
	}
	//读取地图,生成两个连续播放的sprite
	cocos2d::log("INFO: creating background sprites");
	auto bg1 = cocos2d::Sprite::create("plainMap//repeatedPlainMap_" + std::to_string(repeatedMapIndex) + ".png");
	auto bg2 = cocos2d::Sprite::create("plainMap//repeatedPlainMap_" + std::to_string(repeatedMapIndex) + ".png");
	float time = mapHeight / moveSpeed; //一个背景走完一圈的时间
	bg1->setAnchorPoint(cocos2d::Vec2::ZERO);
	bg1->setPosition(0, 0);
	bg1->setZOrder(ZOrder);
	bg1->runAction(
		cocos2d::Sequence::create(
			cocos2d::MoveBy::create(time, cocos2d::Vec3(0, -mapHeight, 0)),
			cocos2d::CallFunc::create(std::bind(&PlanetGenerator::backgroundCallback,_generator, bg1, time, mapHeight)),
			0
		)
	);
	bg2->setAnchorPoint(cocos2d::Vec2::ZERO);
	bg2->setPosition(0, mapHeight);
	bg2->setZOrder(ZOrder);
	bg2->runAction(
		cocos2d::Sequence::create(
			cocos2d::MoveBy::create(2.0 * time, cocos2d::Vec3(0, -2 * mapHeight, 0)),
			cocos2d::CallFunc::create(std::bind(&PlanetGenerator::backgroundCallback, _generator, bg2, time, mapHeight)),
			0
		)
	);
	scene->addChild(bg1);
	scene->addChild(bg2);
	
}

bool PlanetGenerator::generateRepeatedPlainMap(std::string name, int scale, int bias)
{
	float detail = 0.01; //控制生成地形的宏观程度（越大则生成越跳跃，越宏观）
	float fBiasX = (float)bias * mapWidth * detail; //设置偏移值
	//根据mapHeight和mapWidth生成指定大小的地图，然后放大scale倍
	cocos2d::log("generating repeated plain map: initing...");
	//打开文件
	FILE* fp = fopen(("plainMap\\" + name + ".png").c_str(), "wb");
	//生成图像
	unsigned char* pScaledMap = new unsigned char[4 * scale * scale * mapHeight * mapWidth];
	//生成小图
	float* pUnScaledMap = new float[mapHeight * mapWidth];
	unsigned char* p = &pScaledMap[0];
	float* up = &pUnScaledMap[0];
	//圆柱的半径
	float radius = mapHeight * detail / (2 * M_PI);
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			//投影成圆柱,以此获得连续的平面
			float theta = ((float)i / (float)mapHeight) * 2 * M_PI;
			*up++ = 0.8 - 0.5 * sn.fractal(8, radius * cos(theta), (float)j * detail + fBiasX, radius * sin(theta));
		}
	}
	for (int i = 0; i < mapHeight * scale; i++) {
		for (int j = 0; j < mapWidth * scale; j++) {
			tinter::color c = styleTinter.getColor(pUnScaledMap[(i / scale) * mapWidth + j / scale]);
			*p++ = c.R;
			*p++ = c.G;
			*p++ = c.B;
			*p++ = c.A;
		}
	}
	//写入文件
	svpng(fp, mapWidth * scale, mapHeight * scale, pScaledMap, 1);
	//返回内存
	delete pUnScaledMap;
	delete pScaledMap;
	fclose(fp);
	return true;
}

bool PlanetGenerator::generateHeightMap(std::string name, int radius,int detail, int frameCount, int cloudFrameCount,int scale, int bias) //设置bias来区分每张图的构成
{
	//改变mapHeight和mapWidth的话用setMapSize
	int greySize = mapWidth * mapHeight;
	float *grey = new float[greySize];//存储灰度值
	cocos2d::log("Planet generation: generating terrain...");
	//连续化生成指定长宽的像素图（反球面投影）
	mapProjection((float)detail * 0.05 + 0.9, grey, greySize,1); //参数控制生成效果，detail控制平均海拔高度，detail=1时效果最合适
	cocos2d::log("Planet generation: projecting terrain...");
	/*球面投影*/
	float R = radius * scale;
	//球坐标参数
	float theta = 0.0;
	float phi = 0.0;
	int scaledRadius = scale * radius;
	unsigned char* pFrame = new unsigned char[4 * (2 * scaledRadius) * (2 * scaledRadius)]; //建立一个矩形，用于存放彩色图
	//这部分是性能瓶颈
	for (int k = 1; k <= frameCount; k++) {
		//打开文件
		FILE* fp = fopen(("planetGenerator//" + std::to_string(k) + name + ".png").c_str(), "wb");
		//设置每次的偏转角度
		float viewBias = 2 * M_PI * ((float)k / frameCount);
		//建立pFrame的索引
		unsigned char *pf = &pFrame[0];
		for (int i = 0; i < 2 * scaledRadius; i++) { //y轴
			for (int j = 0; j < 2 * scaledRadius; j++) { //x轴
				//变换到球心坐标系
				int x = j - scaledRadius;
				int y = scaledRadius - i;
				if ((x*x + y * y <= R * R)) { //在球内时才能投影
					//从投影坐标解出球坐标参数
					phi = acos(y / R);
					theta = viewBias + asin(x / (R * sin(phi))); //加上随角度产生的偏转量
					//由theta计算坐标伸缩值Xscale
					float Xscale = theta / (2 * M_PI);
					if (theta <= 0) Xscale += 1.0;
					if (theta >= 2 * M_PI) Xscale -= 1.0;
					//计算球坐标参数对应的点
					int mapX = (int)(mapWidth * Xscale);
					int mapY = (int)(mapHeight * (phi / (M_PI)));
					//确保不越界
					if (mapX < 0) mapX = 0;
					if (mapY < 0) mapY = 0;
					if (mapX > mapWidth - 1) mapX = mapWidth - 1;
					if (mapY > mapWidth - 1) mapY = mapWidth - 1;
					//着色
					tinter::color c = styleTinter.getColor(grey[mapWidth * mapY + mapX]);
					*pf++ = c.R;
					*pf++ = c.G;
					*pf++ = c.B;
					*pf++ = c.A;
				}
				else { //在球面外则填充透明
					*pf++ = 0;
					*pf++ = 0;
					*pf++ = 0;
					*pf++ = 0;
				}
			}
		}
		pf = &pFrame[0]; //每一帧后重置pf
		svpng(fp, 2 * scaledRadius, 2 * scaledRadius, pFrame, 1);
		fclose(fp);
	}
	delete pFrame; //释放内存
	delete grey;
	//生成云图
	this->generateCloud(name + "_cloud" ,radius - 2, 8, mapWidth, mapHeight, cloudQuantity::medium, cloudFrameCount,scale, bias);
	return false;
}

bool PlanetGenerator::generateCloud(std::string name, int radius, int cloudHeight, int mapW, int mapH, cloudQuantity quantity, int frameCount, int Rscale, int bias)
{
	//mapW和mapH是实际所需要的云层图大小，而在生成中为了避免数组越界，增加了border
	//radius是需要遮蔽的球体半径，在生成动图的时候会将半径后的云去掉
	//cloudHeight是大气层高度，也是云层离地高度.
	int border = 15; //用于裁切
	//打开文件
	FILE* fp = fopen("cloudmap.png", "wb");
	//生成radius + border厚度的云层
	float scale = 0.08;
	//设置地图大小
	int width = mapW + 2 * border;
	int height = mapH + 2 * border;
	unsigned char* clouds = new unsigned char[width * height * 4]{ 0 }; //点的位置存储云彩位置，点的值存储云彩大小
	unsigned char* renderMap = new unsigned char[mapW * mapH * 4 * 4];
	cocos2d::log("Cloud texture: initializing...");
	for (int i = border; i < mapH + 2 * border; i++) { //y轴
		for (int j = border; j < mapW + 2 * border; j++) { //x轴
			float n = this->pn.noise(scale * i, scale * j, 0.5);
			unsigned char nGrey = (unsigned char)(n * 255.0);
			if (nGrey >= 160 && nGrey < 160 + (int)quantity) { //用点的量控制云彩
				//将附近的区域“上色”
				for (int m = -3; m <= 3; m++) {
					for (int n = -3; n <= 3; n++) {
						clouds[(i + m)*width + j + n] ++; //区域自增1
					}
				}
				clouds[i*width + j] += 100; //这个点是需要生成的点
			}
		}
	}
	//初始化
	cloudVec = new std::vector<cloudPoint>;
	cocos2d::log("Cloud texture: generating clouds...");
	//根据聚集区域确定云朵块的大小
	for (int i = border; i < mapH + 2 * border; i++) { //y轴
		for (int j = border; j < mapW + 2 * border; j++) { //x轴
			int sizePointer = clouds[i * width + j];
			sizePointer = sizePointer > 100 ? sizePointer - 100 : 0;
			if (sizePointer == 2) { //有两个点覆盖到，生成7直径的球
				cloudVec->push_back(cloudPoint{ j,i,2,3 });
			}
			if (sizePointer >= 3) {
				cloudVec->push_back(cloudPoint{ j,i,3,4 });
			}
		}
	}
	cocos2d::log("Cloud texture: generating details...");
	//云彩细化
	std::vector<cloudPoint> tmp;
	for (auto i = cloudVec->begin(); i != cloudVec->end(); i++) {
		if (i->size == 2) {
			//随机生成2个小点
			tmp.push_back(cloudPoint{ i->x + (int)getRandom(-6,6),i->y + (int)getRandom(-6,6),1,1 });
		}
		if (i->size == 3) {
			//随机生成2个小点
			tmp.push_back(cloudPoint{ i->x + (int)getRandom(-5,5),i->y + (int)getRandom(-5,5),2,2 });
			tmp.push_back(cloudPoint{ i->x + (int)getRandom(-5,5),i->y + (int)getRandom(-5,5),2,2 });
		}
	}
	for (auto j = tmp.begin(); j != tmp.end(); j++) {
		cloudVec->push_back(*j);
	}
	cocos2d::log(("Cloud texture: CloudPoint=" + std::to_string(cloudVec->size()) + " Rendering...").c_str());
	//云彩渲染
	for (auto i = cloudVec->begin(); i != cloudVec->end(); i++) {
		int rs = i->renderSize;
		if (rs == 1) {
			//如果是1像素，简化算法
			clouds[i->y * width + i->x] = 200;
			continue;
		}
		for (int m = -rs; m < rs; m++) {
			for (int n = -rs; n < rs; n++) {
				if (m*m + n * n <= rs) {
					//如果在圆内
					clouds[(i->y + m)*width + i->x + n] = 200;
				}
			}
		}
	}
	cocos2d::log("Cloud texture: writing file...");
	//填入文件, 这里的横向border直接被切掉
	unsigned char* pMap = &renderMap[0];
	tinter::color c = styleTinter.colorMap["Cloud"];
	for (int i = 0; i < mapH; i++) {
		for (int j = 0; j < mapW; j++) {
			if (clouds[i * width + j] == 200) {
				*pMap++ = c.R;
				*pMap++ = c.G;
				*pMap++ = c.B;
				*pMap++ = c.A;
			}
			else {
				*pMap++ = 0;
				*pMap++ = 0;
				*pMap++ = 0;
				*pMap++ = 0;
			}
		}
	}

	//存储文件，收回内存
	svpng(fp, mapW, mapH, renderMap, 1); //制作示意图
	fclose(fp);
	delete renderMap;
	delete clouds;
	//开始投影到球面
	cocos2d::log("Cloud global projection...");
	//用到的值
	int cloudRadius = radius + cloudHeight;
	int frameWidth = 2*(radius + 2 * cloudHeight);
	int frameHeight = 2*(radius + 2 * cloudHeight);
	/*球面投影，这个是给出点投影到面上的*/
	int frameR = radius + 2 * cloudHeight;
	//球坐标参数
	float theta = 0.0;
	float phi = 0.0;
	unsigned char* pRender = new unsigned char[frameWidth * frameHeight]{ 0 }; //渲染缓存
	unsigned char* pFrame = new unsigned char[4 * (frameWidth*Rscale) * (frameHeight*Rscale)]; //存放生成的图
	//trace on!
	for (int k = 1; k <= frameCount; k++) {
		//打开文件
		FILE* fp = fopen(("planetGenerator//" + std::to_string(k) + name + ".png").c_str(), "wb");
		//设置每次的偏转角度,从此开始的半个圆周被定义为正面
		float viewBias = 2 * M_PI * ((float)k / frameCount);
		//将每个点映射到球面应该显示的点
		std::vector<cloudPoint> globalPoints; //存放映射后的vector
		for (auto point = cloudVec->begin(); point != cloudVec->end(); point++) {
			if (point->x >= mapW || point->x <= 0 || point->y >= mapH || point->y <= 0) {
				//超出区域的数组不会被考虑在内
				continue;
			}
			phi = ((float)point->y / mapH) * M_PI; //获取该点对应的球面参数
			theta = ((float)point->x / mapW) * 2 * M_PI - viewBias; //视角修正
			//修正theta，此时theta在-pi到pi之间，0代表正对观测中心
			if (theta < -M_PI) theta += 2 * M_PI;
			if (theta > M_PI) theta -= 2 * M_PI;
			//计算对应的坐标点,注意云层是投影到地表以上高度的球面的, 这个坐标是以球中心为原点的坐标
			int y = (int)((float)cloudRadius * cos(phi));
			int x = (int)((float)cloudRadius * sin(phi) * sin(theta));
			//写入到球面统计区中
			int face;
			if (theta > -M_PI_2 && theta < M_PI_2) { //如果在正面
				face = 1;
			}
			else {
				face = -1;
			}
			//绘制画面到缓存中
			for (int i = -point->renderSize; i < point->renderSize; i++) { //y
				for (int j = -point->renderSize; j < point->renderSize; j++) { //x
					int axisX = j + x;
					int axisY = i + y;
					if (i*i + j * j <= point->renderSize * point->renderSize) { //如果在云内部
						if (face == -1 && axisX * axisX + axisY * axisY <= radius * radius) {//如果点在背面并且这个像素已经被遮住
							continue;
						}
						int projX = axisX + frameR;
						int projY = frameR - axisY;
						//防越界
						if (projX < 0) projX = 0;
						if (projX >= frameWidth) projX = frameWidth - 1;
						if (projY < 0) projY = 0;
						if (projY >= frameHeight) projY = frameWidth - 1;
						//投影
						pRender[projY * frameWidth + projX] = 1;
					}
				}
			}
		}
		//将画面绘制到缓存图中
		unsigned char* p = &pFrame[0];
		for (int i = 0; i < frameHeight * Rscale; i++) {
			for (int j = 0; j < frameWidth * Rscale; j++) {
				int fixed_i = i / Rscale;
				int fixed_j = j / Rscale;
				if (pRender[fixed_i * frameWidth + fixed_j] == 1) { //如果这一点确定要描绘
					*p++ = c.R;
					*p++ = c.G;
					*p++ = c.B;
					*p++ = c.A;
				}
				else {//透明
					*p++ = 0;
					*p++ = 0;
					*p++ = 0;
					*p++ = 0;
				}
			}
		}
		svpng(fp, frameWidth * Rscale, frameHeight * Rscale, pFrame, 1);
		fclose(fp);
		//清除pRender
		for (int i = 0; i < frameWidth * frameHeight; i++) {
			pRender[i] = 0;
		}
	}
	delete cloudVec;
	cocos2d::log("Cloud rendering: Done!");
	return true;
}

float * PlanetGenerator::mapProjection(float control, float * grey, int size, int noiseRadius) //从噪声中得到二维球面，并将其展开成地图
{
	if (size != mapHeight * mapWidth) return nullptr; //检查是否匹配
	float * g0 = grey; //可变指针
	float theta, phi;
	float x, y, z;
	float g;
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			//对每个点进行映射,首先构造球面参数
			theta = 2 * M_PI * ((float)j / mapWidth);
			phi = M_PI * ((float)i / mapHeight);
			//根据球面参数计算三维坐标
			z = cos(phi) * noiseRadius;
			x = sin(phi) * noiseRadius * cos(theta);
			y = sin(phi) * noiseRadius * sin(theta);
			//获取灰度
			g = 0.95 - sn.fractal(8,x,y,z); 
			//存储到地图中
			*g0 = g;
			g0++;
		}
	}
	return grey;
}

void PlanetGenerator::backgroundCallback(cocos2d::Sprite * sprite, float time, int mapHeight)
{
	sprite->setPosition(0, mapHeight);
	sprite->runAction(
		cocos2d::Sequence::create(
			cocos2d::MoveBy::create(2.0 * time, cocos2d::Vec3(0, -2 * mapHeight, 0)),
			cocos2d::CallFunc::create(std::bind(&PlanetGenerator::backgroundCallback, _generator, sprite, time, mapHeight)),
			0
		)
	);
}



void PlanetGenerator::setSeed(unsigned int seed)
{
	srand(seed);
	//重置种子
}

void PlanetGenerator::setMapSize(int height, int width)
{
	this->mapHeight = height;
	this->mapWidth = width;
}