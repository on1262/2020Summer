#ifndef __GEM_H__
#define __GEM_H__
#include "cocos2d.h"
#include <string>
#include "Data.h"
#include <vector>
#include "cocos/ui/CocosGUI.h"

namespace game {
	/*�������ͱ�ʯ*/
	class gem
	{
	public:
		//��Ա
		std::string instanceName; //�洢��ʯʵ������
		std::string type;//�洢��ʯ����
		float value; //����, �����advanced���ͣ����������ֵ
		int price;
		cocos2d::Ref* iconRef;
		//��̬����
		static gem* getGem(game::Data* pData);//������������һ����ʯʵ��, ������create
		static float getGain(std::vector<gem*>* vec); //���ݱ�ʯ�б��������
		static std::string getPicturePath(std::string type); //�ɵ�ǰtype������Ӧ�ı�ʯ����·��
		//�Ǿ�̬����
		gem();
		cocos2d::ui::Button* getGemIcon(); //������ʾ�������в���

	private:
		inline float getAdvancedGain(float average);
	};
}

#endif __GEM_H__

