#include "stationScene.h"
#ifndef __FITWEAPONWINDOW_H__
#define __FITWEAPONWINDOW_H__
#include "cocos2d.h"
#include "cocos/ui/CocosGUI.h" //ͼ�οؼ�
#include "DataLoader.h"
#include "Data.h"
#include "Weapon.h"
#include "gem.h"
#include <vector>
#include <string>
#include "util.h"

class stationScene;

/*����һ�����*/
namespace game {
	class FitWeaponWindow : public cocos2d::Sprite
	{
	private:
		class WeaponPanel : public cocos2d::ui::Button
		{
		public:
			//ͨ����
			std::string weaponPath;//�������ͼƬ
			std::string backgroundPath;//��ű���ͼƬ
			Data* pData; //�������ʵ���������Ϣ
			//��Data�д�ȡ��Ϣ�Ĺ���(����ת����)
			std::vector<gem*> gemInWeapon; //����Data�е���Ϣ��ʼ��gem
			void addGem(gem*g); //���ָ����gem
			bool deleteGem(gem*g); //ͨ�����icon�Ƴ�ָ����gem
			//gemIcon���õ�callback
			void gemIconCallback(Ref* sender);
			FitWeaponWindow* window;
			static WeaponPanel* create(FitWeaponWindow* window, Data* pData, std::string weaponPath, std::string backgroundPath);
			void chickPanel();
			static WeaponPanel* create(const std::string &normalImage, const std::string& selectedImage, const std::string& disableImage, TextureResType texType);
		};
		class GemPanel : public cocos2d::ui::Button
		{
		public:
			//ͨ��
			FitWeaponWindow *window;
			gem* pGem; //���ﲻ�Ǻ����������淶...���ع���ʱ���ٸĽ���
			//�������ɺ������ڲ�����
			static GemPanel* create(const std::string &normalImage, const std::string& selectedImage, const std::string& disableImage, TextureResType texType);
			//ֱ�����ɵ�ʱ�������
			static GemPanel* create(FitWeaponWindow *window, gem* g);
			//�ص�����
			void chickPanel(Ref* sender);
		};

	public:
		//��ȡ����
		game::DataLoader* pGameDataLoader;
		game::DataLoader* pUserDataLoader;
		static FitWeaponWindow* _window;
		CREATE_FUNC(FitWeaponWindow);
		stationScene* scene; //���һ������
		Data* storage; //��ȡ��Ҳֿ���Ķ���
		//�洢�����б��еĶ���
		std::vector<WeaponPanel*> weaponPanelVec;
		std::vector<GemPanel*> gemPanelVec; //ֻ�洢�ֿ��л�ûװ���ϵ�gem
		//��ʾ������ϵ�������Ϣ
		cocos2d::ui::Button* weaponImage;
		cocos2d::Label* weaponName, *gemGain, *performance, *story;//��������, ���������������������˵��
		WeaponPanel* showingWeaponPanel; //��ǰ������ʾ��weapon
		//��gem����ӡ��Ƴ�
		void addGem(gem*g);
		void deleteGemPanel(GemPanel *gp);
		//ˢ�������ң�ֻ�Ƕ�����child������λ, pos=left/right, ˢ���м�������chooseWeapon����
		void flush(std::string pos);
		//������
		cocos2d::ui::ScrollView* weaponScrollView;
		cocos2d::ui::ScrollView* gemScrollView;
		static FitWeaponWindow* create(stationScene* s); //��ʼ��������һ������
		void chooseWeapon(FitWeaponWindow::WeaponPanel* wp); //���û�����һ������ʱ����ʾ��ص���Ϣ, Ҳ��������ˢ��
	};
}

#endif __FITWEAPONWINDOW_H__ // __FITWEAPONWINDOW_H__
