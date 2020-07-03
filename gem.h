#ifndef __GEM_H__
#define __GEM_H__
#include "cocos2d.h"
#include <string>
#include "Data.h"
#include <vector>
#include "cocos/ui/CocosGUI.h"

namespace game {
	/*所有类型宝石*/
	class gem
	{
	public:
		//成员
		std::string instanceName; //存储宝石实例类型
		std::string type;//存储宝石类型
		float value; //增益, 如果是advanced类型，则代表最适值
		int price;
		cocos2d::Ref* iconRef;
		//静态方法
		static gem* getGem(game::Data* pData);//根据配置生成一个宝石实例, 不调用create
		static float getGain(std::vector<gem*>* vec); //根据宝石列表计算增益
		static std::string getPicturePath(std::string type); //由当前type返回相应的宝石类型路径
		//非静态方法
		gem();
		cocos2d::ui::Button* getGemIcon(); //用于显示在武器中侧栏

	private:
		inline float getAdvancedGain(float average);
	};
}

#endif __GEM_H__

