#ifndef __PLANETGENERATOR_H__
#define __PLANETGENERATOR_H__
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
//����
#include "cocos2d.h"
/*�������������ѭ��ƽ�汳��ͼ, ��ѭ��ƽ�汳��ͼ, ָ����С�����涯ͼ�ȵ� */

class PlanetGenerator
{
public:
	enum cloudQuantity { none, little, medium, large, veryLarge };
	struct cloudPoint { //x,y,size,renderSize
		int x, y, size, renderSize; //size��Ϊ1,2,3���֣�renderSize����Ⱦ�Ĵ�С
	};
	PlanetGenerator();
	static PlanetGenerator* _generator;
	/*������õĽӿ�*/
	static PlanetGenerator* getInstance(); //��ȡʵ��
	/*@param repeatedMapIndex: �������ֲ�ͬ�ؿ��ĵ�ͼ���(���������Եĵ�ͼ, ֻ��һ��png)
	*@param scene: ��ӱ�����background
	*@param ZOrder: ������ӽ�scene�������ڵ���ڵ���ϵ
	*@param windowSize: �����Ĵ�С, ֱ�Ӿ�������ͼƬ���ܴ�С
	*@param scale: ����ԭ��ͼ�ķŴ����, ֻ���ǿ�ߵĹ�����, һ��Ϊ4
	*@param mapHeight: ������ͼ�Ŵ��ĸ߶�, Ҫ��Ŵ��������
	*@param moveSpeed: ��ͼ��scene��ÿ���ƶ����ٸ�����, ��������sceneΪ������
	*@param bias: �Ե�ͼ����������ı�׼ */
	static void setRepeatedPlainMap(bool reGenerate, int repeatedMapIndex,cocos2d::Scene* scene,int ZOrder, cocos2d::Size windowSize, int scale, int mapHeight, float moveSpeed, int bias);
	/*���²����ǹ�����*/
	bool generatePlainMap(std::string name,int mapCount, int scale,int bias); //����mapCount����name_N.png������ƽ��ͼ, ���طŴ���scale����, bias���������
	bool generateRepeatedPlainMap(std::string name, int scale, int bias); //����ǰ���νӵ�һ�ŵ�ͼ
	bool generateHeightMap(std::string name, int radius, int detail, int frameCount,int cloudFrameCount, int scale, int bias); //�ɸ����������ɵ�ͼ����������ͶӰ
	bool generateCloud(std::string name, int radius, int cloudHeight, int mapW, int mapH, cloudQuantity quantity, int frameCount, int scale, int bias);//����ָ�����������Ʋ�ƽ��ͼ
	float * mapProjection(float control, float * grey, int size, int noiseRadius); //control�������ɵ��ܶȣ�����������Ϊ0.95,Խ��Խ�ӽ��������ǣ�Խ��Խ�ӽ�½������
	//��ͼsprite�Ļص�����
	void backgroundCallback(cocos2d::Sprite * sprite, float time, int mapHeight);
	//�ڲ��Ķ���
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
#endif __PLANETGENERATOR_H__

