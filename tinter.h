#pragma once
#include <map>
#include <string>
/*���е�ͼ��ɫ���Ļ���*/
class tinter 
{
public:
	std::string style; //�����л����
	struct color {
		int R, G, B, A;
	};
	tinter(); //Ĭ�ϳ�ʼ����basic
	tinter(std::string colorStyle);//���ݷ���ʼ��
	//���ڴ洢��Ҫ����ɫ
	std::map<std::string, color> colorMap;
	color getColor(float grey); //�߳���ɫ��
};

