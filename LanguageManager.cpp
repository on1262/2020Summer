#include "LanguageManager.h"

#include "cocos2d.h"
using namespace cocos2d;

LanguageManager* LanguageManager::_instance = 0;

LanguageManager::LanguageManager()
{
    string fileName = "language//zh.json"; //Ä¬ÈÏÖÐÎÄ

    ssize_t size;
    const char* buf = (const char*)FileUtils::getInstance()->getFileData(fileName.c_str(), "r", &size);
    string content(buf);
    string clearContent = content.substr(0, content.rfind('}') + 1);

    _document.Parse<0>(clearContent.c_str());
    if(_document.HasParseError())
    {
        CCLOG("Language file parsing errors");
        return;
    }
}

LanguageManager* LanguageManager::getInstance()
{
    if(!_instance)
        _instance = new LanguageManager();
    return _instance;
}

string LanguageManager::getStringForKey(string key) const
{
    return _document[key.c_str()].GetString();
}
