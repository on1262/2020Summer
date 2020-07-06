#include "PlanetGenerator.h"
#include <iostream>
#include "cocos2d.h"
#include <cmath>
#include <random>

#define _USE_MATH_DEFINES

PlanetGenerator* PlanetGenerator::_generator = nullptr;

PlanetGenerator::PlanetGenerator()
{
	//��ʼ������
	mapHeight = 128;
	mapWidth = 256;
	//���ر�Ҫ����ɫ����
	styleTinter = tinter(std::string("earth-like"));
	//��ʼ��
	sn = SimplexNoise(); //ʹ��Ĭ�ϲ�����ʼ��pn
	pn = PerlinNoise();
	//��ʼ�������������(����Ҫ��
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
	float detail = 0.01; //�������ɵ��εĺ�۳̶ȣ�Խ��������Խ��Ծ��Խ��ۣ�
	float fBiasX = (float)bias * mapWidth * detail; //����ƫ��ֵ
	float fBiasY = (float)bias * mapHeight * detail;
	//����mapHeight��mapWidth����ָ����С�ĵ�ͼ��Ȼ��Ŵ�scale��
	cocos2d::log("generating plain map: initing...");
	//���ļ�
	FILE* fp = fopen(("plainMap\\" + name + ".png").c_str(), "wb");
	//����ͼ��
	unsigned char* pScaledMap = new unsigned char[4 * scale * scale * mapHeight * mapWidth];
	//����Сͼ
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
	//д���ļ�
	svpng(fp, mapWidth * scale, mapHeight * scale, pScaledMap, 1);
	//�����ڴ�
	delete pUnScaledMap;
	delete pScaledMap;
	fclose(fp);
	return true;
}

void PlanetGenerator::setRepeatedPlainMap(bool reGenerate, int repeatedMapIndex, cocos2d::Scene * scene, int ZOrder, cocos2d::Size windowSize, int scale, int mapHeight, float moveSpeed, int bias)
{
	//���ø߿�, ��ͼ����Դ����趨���,����Ϊ�˱�����ַ�϶
	_generator->setMapSize(((mapHeight+8) / scale), windowSize.width / scale);
	//���ɵ�ͼ
	cocos2d::log("INFO: generating repeated map...");
	if (reGenerate == true) {
		_generator->generateRepeatedPlainMap("repeatedPlainMap_" + std::to_string(repeatedMapIndex), 4, bias);
	}
	//��ȡ��ͼ,���������������ŵ�sprite
	cocos2d::log("INFO: creating background sprites");
	auto bg1 = cocos2d::Sprite::create("plainMap//repeatedPlainMap_" + std::to_string(repeatedMapIndex) + ".png");
	auto bg2 = cocos2d::Sprite::create("plainMap//repeatedPlainMap_" + std::to_string(repeatedMapIndex) + ".png");
	float time = mapHeight / moveSpeed; //һ����������һȦ��ʱ��
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
	float detail = 0.01; //�������ɵ��εĺ�۳̶ȣ�Խ��������Խ��Ծ��Խ��ۣ�
	float fBiasX = (float)bias * mapWidth * detail; //����ƫ��ֵ
	//����mapHeight��mapWidth����ָ����С�ĵ�ͼ��Ȼ��Ŵ�scale��
	cocos2d::log("generating repeated plain map: initing...");
	//���ļ�
	FILE* fp = fopen(("plainMap\\" + name + ".png").c_str(), "wb");
	//����ͼ��
	unsigned char* pScaledMap = new unsigned char[4 * scale * scale * mapHeight * mapWidth];
	//����Сͼ
	float* pUnScaledMap = new float[mapHeight * mapWidth];
	unsigned char* p = &pScaledMap[0];
	float* up = &pUnScaledMap[0];
	//Բ���İ뾶
	float radius = mapHeight * detail / (2 * M_PI);
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			//ͶӰ��Բ��,�Դ˻��������ƽ��
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
	//д���ļ�
	svpng(fp, mapWidth * scale, mapHeight * scale, pScaledMap, 1);
	//�����ڴ�
	delete pUnScaledMap;
	delete pScaledMap;
	fclose(fp);
	return true;
}

bool PlanetGenerator::generateHeightMap(std::string name, int radius,int detail, int frameCount, int cloudFrameCount,int scale, int bias) //����bias������ÿ��ͼ�Ĺ���
{
	//�ı�mapHeight��mapWidth�Ļ���setMapSize
	int greySize = mapWidth * mapHeight;
	float *grey = new float[greySize];//�洢�Ҷ�ֵ
	cocos2d::log("Planet generation: generating terrain...");
	//����������ָ�����������ͼ��������ͶӰ��
	mapProjection((float)detail * 0.05 + 0.9, grey, greySize,1); //������������Ч����detail����ƽ�����θ߶ȣ�detail=1ʱЧ�������
	cocos2d::log("Planet generation: projecting terrain...");
	/*����ͶӰ*/
	float R = radius * scale;
	//���������
	float theta = 0.0;
	float phi = 0.0;
	int scaledRadius = scale * radius;
	unsigned char* pFrame = new unsigned char[4 * (2 * scaledRadius) * (2 * scaledRadius)]; //����һ�����Σ����ڴ�Ų�ɫͼ
	//�ⲿ��������ƿ��
	for (int k = 1; k <= frameCount; k++) {
		//���ļ�
		FILE* fp = fopen(("planetGenerator//" + std::to_string(k) + name + ".png").c_str(), "wb");
		//����ÿ�ε�ƫת�Ƕ�
		float viewBias = 2 * M_PI * ((float)k / frameCount);
		//����pFrame������
		unsigned char *pf = &pFrame[0];
		for (int i = 0; i < 2 * scaledRadius; i++) { //y��
			for (int j = 0; j < 2 * scaledRadius; j++) { //x��
				//�任����������ϵ
				int x = j - scaledRadius;
				int y = scaledRadius - i;
				if ((x*x + y * y <= R * R)) { //������ʱ����ͶӰ
					//��ͶӰ���������������
					phi = acos(y / R);
					theta = viewBias + asin(x / (R * sin(phi))); //������ǶȲ�����ƫת��
					//��theta������������ֵXscale
					float Xscale = theta / (2 * M_PI);
					if (theta <= 0) Xscale += 1.0;
					if (theta >= 2 * M_PI) Xscale -= 1.0;
					//���������������Ӧ�ĵ�
					int mapX = (int)(mapWidth * Xscale);
					int mapY = (int)(mapHeight * (phi / (M_PI)));
					//ȷ����Խ��
					if (mapX < 0) mapX = 0;
					if (mapY < 0) mapY = 0;
					if (mapX > mapWidth - 1) mapX = mapWidth - 1;
					if (mapY > mapWidth - 1) mapY = mapWidth - 1;
					//��ɫ
					tinter::color c = styleTinter.getColor(grey[mapWidth * mapY + mapX]);
					*pf++ = c.R;
					*pf++ = c.G;
					*pf++ = c.B;
					*pf++ = c.A;
				}
				else { //�������������͸��
					*pf++ = 0;
					*pf++ = 0;
					*pf++ = 0;
					*pf++ = 0;
				}
			}
		}
		pf = &pFrame[0]; //ÿһ֡������pf
		svpng(fp, 2 * scaledRadius, 2 * scaledRadius, pFrame, 1);
		fclose(fp);
	}
	delete pFrame; //�ͷ��ڴ�
	delete grey;
	//������ͼ
	this->generateCloud(name + "_cloud" ,radius - 2, 8, mapWidth, mapHeight, cloudQuantity::medium, cloudFrameCount,scale, bias);
	return false;
}

bool PlanetGenerator::generateCloud(std::string name, int radius, int cloudHeight, int mapW, int mapH, cloudQuantity quantity, int frameCount, int Rscale, int bias)
{
	//mapW��mapH��ʵ������Ҫ���Ʋ�ͼ��С������������Ϊ�˱�������Խ�磬������border
	//radius����Ҫ�ڱε�����뾶�������ɶ�ͼ��ʱ��Ὣ�뾶�����ȥ��
	//cloudHeight�Ǵ�����߶ȣ�Ҳ���Ʋ���ظ߶�.
	int border = 15; //���ڲ���
	//���ļ�
	FILE* fp = fopen("cloudmap.png", "wb");
	//����radius + border��ȵ��Ʋ�
	float scale = 0.08;
	//���õ�ͼ��С
	int width = mapW + 2 * border;
	int height = mapH + 2 * border;
	unsigned char* clouds = new unsigned char[width * height * 4]{ 0 }; //���λ�ô洢�Ʋ�λ�ã����ֵ�洢�Ʋʴ�С
	unsigned char* renderMap = new unsigned char[mapW * mapH * 4 * 4];
	cocos2d::log("Cloud texture: initializing...");
	for (int i = border; i < mapH + 2 * border; i++) { //y��
		for (int j = border; j < mapW + 2 * border; j++) { //x��
			float n = this->pn.noise(scale * i, scale * j, 0.5);
			unsigned char nGrey = (unsigned char)(n * 255.0);
			if (nGrey >= 160 && nGrey < 160 + (int)quantity) { //�õ���������Ʋ�
				//��������������ɫ��
				for (int m = -3; m <= 3; m++) {
					for (int n = -3; n <= 3; n++) {
						clouds[(i + m)*width + j + n] ++; //��������1
					}
				}
				clouds[i*width + j] += 100; //���������Ҫ���ɵĵ�
			}
		}
	}
	//��ʼ��
	cloudVec = new std::vector<cloudPoint>;
	cocos2d::log("Cloud texture: generating clouds...");
	//���ݾۼ�����ȷ���ƶ��Ĵ�С
	for (int i = border; i < mapH + 2 * border; i++) { //y��
		for (int j = border; j < mapW + 2 * border; j++) { //x��
			int sizePointer = clouds[i * width + j];
			sizePointer = sizePointer > 100 ? sizePointer - 100 : 0;
			if (sizePointer == 2) { //�������㸲�ǵ�������7ֱ������
				cloudVec->push_back(cloudPoint{ j,i,2,3 });
			}
			if (sizePointer >= 3) {
				cloudVec->push_back(cloudPoint{ j,i,3,4 });
			}
		}
	}
	cocos2d::log("Cloud texture: generating details...");
	//�Ʋ�ϸ��
	std::vector<cloudPoint> tmp;
	for (auto i = cloudVec->begin(); i != cloudVec->end(); i++) {
		if (i->size == 2) {
			//�������2��С��
			tmp.push_back(cloudPoint{ i->x + (int)getRandom(-6,6),i->y + (int)getRandom(-6,6),1,1 });
		}
		if (i->size == 3) {
			//�������2��С��
			tmp.push_back(cloudPoint{ i->x + (int)getRandom(-5,5),i->y + (int)getRandom(-5,5),2,2 });
			tmp.push_back(cloudPoint{ i->x + (int)getRandom(-5,5),i->y + (int)getRandom(-5,5),2,2 });
		}
	}
	for (auto j = tmp.begin(); j != tmp.end(); j++) {
		cloudVec->push_back(*j);
	}
	cocos2d::log(("Cloud texture: CloudPoint=" + std::to_string(cloudVec->size()) + " Rendering...").c_str());
	//�Ʋ���Ⱦ
	for (auto i = cloudVec->begin(); i != cloudVec->end(); i++) {
		int rs = i->renderSize;
		if (rs == 1) {
			//�����1���أ����㷨
			clouds[i->y * width + i->x] = 200;
			continue;
		}
		for (int m = -rs; m < rs; m++) {
			for (int n = -rs; n < rs; n++) {
				if (m*m + n * n <= rs) {
					//�����Բ��
					clouds[(i->y + m)*width + i->x + n] = 200;
				}
			}
		}
	}
	cocos2d::log("Cloud texture: writing file...");
	//�����ļ�, ����ĺ���borderֱ�ӱ��е�
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

	//�洢�ļ����ջ��ڴ�
	svpng(fp, mapW, mapH, renderMap, 1); //����ʾ��ͼ
	fclose(fp);
	delete renderMap;
	delete clouds;
	//��ʼͶӰ������
	cocos2d::log("Cloud global projection...");
	//�õ���ֵ
	int cloudRadius = radius + cloudHeight;
	int frameWidth = 2*(radius + 2 * cloudHeight);
	int frameHeight = 2*(radius + 2 * cloudHeight);
	/*����ͶӰ������Ǹ�����ͶӰ�����ϵ�*/
	int frameR = radius + 2 * cloudHeight;
	//���������
	float theta = 0.0;
	float phi = 0.0;
	unsigned char* pRender = new unsigned char[frameWidth * frameHeight]{ 0 }; //��Ⱦ����
	unsigned char* pFrame = new unsigned char[4 * (frameWidth*Rscale) * (frameHeight*Rscale)]; //������ɵ�ͼ
	//trace on!
	for (int k = 1; k <= frameCount; k++) {
		//���ļ�
		FILE* fp = fopen(("planetGenerator//" + std::to_string(k) + name + ".png").c_str(), "wb");
		//����ÿ�ε�ƫת�Ƕ�,�Ӵ˿�ʼ�İ��Բ�ܱ�����Ϊ����
		float viewBias = 2 * M_PI * ((float)k / frameCount);
		//��ÿ����ӳ�䵽����Ӧ����ʾ�ĵ�
		std::vector<cloudPoint> globalPoints; //���ӳ����vector
		for (auto point = cloudVec->begin(); point != cloudVec->end(); point++) {
			if (point->x >= mapW || point->x <= 0 || point->y >= mapH || point->y <= 0) {
				//������������鲻�ᱻ��������
				continue;
			}
			phi = ((float)point->y / mapH) * M_PI; //��ȡ�õ��Ӧ���������
			theta = ((float)point->x / mapW) * 2 * M_PI - viewBias; //�ӽ�����
			//����theta����ʱtheta��-pi��pi֮�䣬0�������Թ۲�����
			if (theta < -M_PI) theta += 2 * M_PI;
			if (theta > M_PI) theta -= 2 * M_PI;
			//�����Ӧ�������,ע���Ʋ���ͶӰ���ر����ϸ߶ȵ������, �����������������Ϊԭ�������
			int y = (int)((float)cloudRadius * cos(phi));
			int x = (int)((float)cloudRadius * sin(phi) * sin(theta));
			//д�뵽����ͳ������
			int face;
			if (theta > -M_PI_2 && theta < M_PI_2) { //���������
				face = 1;
			}
			else {
				face = -1;
			}
			//���ƻ��浽������
			for (int i = -point->renderSize; i < point->renderSize; i++) { //y
				for (int j = -point->renderSize; j < point->renderSize; j++) { //x
					int axisX = j + x;
					int axisY = i + y;
					if (i*i + j * j <= point->renderSize * point->renderSize) { //��������ڲ�
						if (face == -1 && axisX * axisX + axisY * axisY <= radius * radius) {//������ڱ��沢����������Ѿ�����ס
							continue;
						}
						int projX = axisX + frameR;
						int projY = frameR - axisY;
						//��Խ��
						if (projX < 0) projX = 0;
						if (projX >= frameWidth) projX = frameWidth - 1;
						if (projY < 0) projY = 0;
						if (projY >= frameHeight) projY = frameWidth - 1;
						//ͶӰ
						pRender[projY * frameWidth + projX] = 1;
					}
				}
			}
		}
		//��������Ƶ�����ͼ��
		unsigned char* p = &pFrame[0];
		for (int i = 0; i < frameHeight * Rscale; i++) {
			for (int j = 0; j < frameWidth * Rscale; j++) {
				int fixed_i = i / Rscale;
				int fixed_j = j / Rscale;
				if (pRender[fixed_i * frameWidth + fixed_j] == 1) { //�����һ��ȷ��Ҫ���
					*p++ = c.R;
					*p++ = c.G;
					*p++ = c.B;
					*p++ = c.A;
				}
				else {//͸��
					*p++ = 0;
					*p++ = 0;
					*p++ = 0;
					*p++ = 0;
				}
			}
		}
		svpng(fp, frameWidth * Rscale, frameHeight * Rscale, pFrame, 1);
		fclose(fp);
		//���pRender
		for (int i = 0; i < frameWidth * frameHeight; i++) {
			pRender[i] = 0;
		}
	}
	delete cloudVec;
	cocos2d::log("Cloud rendering: Done!");
	return true;
}

float * PlanetGenerator::mapProjection(float control, float * grey, int size, int noiseRadius) //�������еõ���ά���棬������չ���ɵ�ͼ
{
	if (size != mapHeight * mapWidth) return nullptr; //����Ƿ�ƥ��
	float * g0 = grey; //�ɱ�ָ��
	float theta, phi;
	float x, y, z;
	float g;
	for (int i = 0; i < mapHeight; i++) {
		for (int j = 0; j < mapWidth; j++) {
			//��ÿ�������ӳ��,���ȹ����������
			theta = 2 * M_PI * ((float)j / mapWidth);
			phi = M_PI * ((float)i / mapHeight);
			//�����������������ά����
			z = cos(phi) * noiseRadius;
			x = sin(phi) * noiseRadius * cos(theta);
			y = sin(phi) * noiseRadius * sin(theta);
			//��ȡ�Ҷ�
			g = 0.95 - sn.fractal(8,x,y,z); 
			//�洢����ͼ��
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
	//��������
}

void PlanetGenerator::setMapSize(int height, int width)
{
	this->mapHeight = height;
	this->mapWidth = width;
}