#include "language.h"
#include "cocos2d.h"

void game::language::init(std::string path)
{
	game::language::dic = cocos2d::Dictionary::createWithContentsOfFile(path.c_str());

}

const char* game::language::get(std::string key)
{
	const char* str = ((cocos2d::String*)dic->objectForKey(key))->getCString(); //×Ö·û´®
	return str;
}
