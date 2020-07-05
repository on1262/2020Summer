#ifndef __FITSHIPWINDOW_H__
#define __FITSHIPWINDOW_H__

#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h"
#include <vector>
#include "DataLoader.h"
#include "Data.h"
#include "Fighter.h"

namespace game {
	/*��ʾ������һ������*/
	class FitShipPanel :public cocos2d::Sprite
	{
	public:
		bool isEquipped; //�Ƿ��Ѿ�װ�����ɴ�����
		Data* weaponData; //�洢��Ӧ��Weapon
		cocos2d::Label* status; //�洢��װ����δװ���ı�ǩ
		cocos2d::ui::Button* button;
		static FitShipPanel* create(bool isEquipped, Data* weaponData);
		void buttonClickCallback(); //��ɴ���ӻ��Ƴ�����
	};
	class FitShipWindow : public cocos2d::Sprite
	{
	public:

		//��Ա
		static FitShipWindow* _window; //һ��ʱ��ֻ����һ��window
		cocos2d::ui::ScrollView* weaponView; //������
		Data* shipData; //�����ж�������
		Fighter* pFighter; //����չʾ���,�����Ȳ�����
		cocos2d::Label* shipLabel; //������ʾ�ɴ���Ϣ
		/*ʹ��ȫ���Ե�DataLoader, ��Ҫ����ship��ָ��*/
		static FitShipWindow* create(Data* shipData);
		/*����ʱ���������*/
		static FitShipWindow* getInstance();
		/*���Ӻ�ɾ��weapon*/
		//storage��ɾ�����ʵ��, �����ɾ��panel,�ɴ�����п���port�����,���û�о���Ϊ��Ч,����panel����
		bool equipWeapon(FitShipPanel* panel, Data* wData); 
		//�ɸ�����ʵ��������Data
		bool unEquipWeapon(FitShipPanel * panel, Data * wData);
		//����panel,װ��������Ҫ������ǰ
		void sortPanel();
		//ˢ�������ͷɴ����(TODO:�ɴ���۵�ʵʱ�ı��Ժ�����)
		void flush();
	};
}
#endif __FITSHIPWINDOW_H__

