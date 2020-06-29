#pragma once
#include "cocos2d.h"
#include <string>
#include "Data.h"
#include <vector>
/*�������ͱ�ʯ*/
class gem :
	public cocos2d::Sprite
{
public:
	//��Ա
	std::string type;//�洢��ʯ����
	float value; //����, �����advanced���ͣ����������ֵ
	int price;
	//����
	static gem* getGem(game::Data* pData);//������������һ����ʯʵ��, ������create
	static float getGain(std::vector<gem>* vec); //���ݱ�ʯ�б��������
	static std::string getPicturePath(std::string type); //�ɵ�ǰtype������Ӧ�ı�ʯ����·��
private:
	inline float getAdvancedGain(float average);
	CREATE_FUNC(gem);
};

