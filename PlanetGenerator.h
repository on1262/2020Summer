#pragma once

//png�����������svpng�ľ���ʹ�ü���https://github.com/miloyip/svpng
#include "svpng.h"
//perlin����
#include "PerlinNoise.h"
//Simplex����
#include "SimplexNoise.h"
#include <map>
#include <string>
//��ɫ���
#include "tinter.h"
/*�������������һϵ��png��ʽ���������֡������Ȼ���ṩһ���ӿ�ʵʱ����/�������ظ�sprite
����������״���壬Ҳ��������ƽ���ͼ
 */



class PlanetGenerator
{
public:
	enum cloudQuantity { none, little, medium, large, veryLarge };
	struct cloudPoint { //x,y,size,renderSize
		int x, y, size, renderSize; //size��Ϊ1,2,3���֣�renderSize����Ⱦ�Ĵ�С
	};
	PlanetGenerator();
	/*���²����ǹ�����*/
	bool generatePlainMap(std::string name, int scale,int bias);
	bool generateHeightMap(std::string name, int radius, int detail, int frameCount,int cloudFrameCount, int scale, int bias); //�ɸ����������ɵ�ͼ����������ͶӰ
	bool generateCloud(std::string name, int radius, int cloudHeight, int mapW, int mapH, cloudQuantity quantity, int frameCount, int scale, int bias);//����ָ�����������Ʋ�ƽ��ͼ
	float * mapProjection(float control, float * grey, int size, int noiseRadius); //control�������ɵ��ܶȣ�����������Ϊ0.95,Խ��Խ�ӽ��������ǣ�Խ��Խ�ӽ�½������
	
	void setMapSize(int height, int width);//���õ�ͼ��С
	void setSeed(unsigned int);
	float getRandom(float start, float end) { //���ɣ�start,end���������ڵĸ������������õ�1-100ֵ����start��Ϊ1����end��Ϊ100
		float i = start + (end - start)*rand() / (RAND_MAX + 1.0);
		return i;
	}
private:
	//����ƽչͼ�Ĵ�С
	unsigned int mapWidth, mapHeight;
	//��ͼ����������
	SimplexNoise sn;
	//�Ʋ�����������
	PerlinNoise pn;
	//�洢һ����ɫ���
	tinter styleTinter;
	//���ڴ洢�Ʋʵ���
	std::vector<cloudPoint>* cloudVec;
};

