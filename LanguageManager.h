#ifndef LanguageManager_h
#define LanguageManager_h

#include <string>
using std::string;

#include "json/rapidjson.h"
#include "json/document.h"
using namespace rapidjson;

class LanguageManager
{
    Document _document;
    LanguageManager();
    static LanguageManager* _instance;
public:
    static LanguageManager* getInstance();
    string getStringForKey(string key) const;
};

#endif
