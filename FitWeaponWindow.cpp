#include "FitWeaponWindow.h"
#include "LanguageManager.h"
#include <sstream>
game::FitWeaponWindow *game::FitWeaponWindow::_window = nullptr;

void game::FitWeaponWindow::WeaponPanel::addGem(gem *g)
{
	//添加到data中
	this->pData->addPair("gem", g->instanceName);
	//添加到vec中
	this->gemInWeapon.push_back(g);
	//刷新中部
	_window->chooseWeapon(this);
}

bool game::FitWeaponWindow::WeaponPanel::deleteGem(gem *g)
{
	//先删除icon
	auto icon = dynamic_cast<cocos2d::ui::Button*> (g->iconRef);
	window->removeChild(icon, false);
	//从weapon的data中删除这一个实例
	/*只删除一个实例，所以用false,因为data不会实时影响gem,而下次打开面板时会自动根据data更新，所以无所谓删除哪个*/
	pData->deletePair(g->instanceName, false);
	//删除vec中的gem
	auto iter = gemInWeapon.begin();
	for (auto i = gemInWeapon.begin(); i != gemInWeapon.end(); i++) {
		if ((*i) == g) {
			//如果相同，则标记
			iter = i;
			cocos2d::log("INFO: deleting iter in gemInWeapon");
			break;
		}
	}
	gemInWeapon.erase(iter);
	return false;
}

void game::FitWeaponWindow::WeaponPanel::gemIconCallback(Ref* sender) //点击图标，将gem转移到storage
{
	gem* g;
	//首先对sender进行解析，确定是哪个gem的image
	for (auto i = gemInWeapon.begin(); i != gemInWeapon.end(); i++) {
		if ((*i)->iconRef == sender) {
			g = *i;
			cocos2d::log("INFO: Find sender in gemIconCallback()");
			break;
		}
	}
	//注意：这些操作不能放在for循环里，因为deleteGem已经将*i删除了，不可以用*i添加到window
	cocos2d::log("INFO: deleting gem in WeaponPanel");
	this->deleteGem(g); //从武器中删除这个gem
	cocos2d::log("INFO: adding gem to window");
	window->addGem(g);//像storage添加gem
}

game::FitWeaponWindow::WeaponPanel * game::FitWeaponWindow::WeaponPanel::create(FitWeaponWindow* window, Data * pData, std::string weaponPath, std::string backgroundPath) {
	WeaponPanel* wp;
	wp = WeaponPanel::create("UI//UI-weaponPanel-white.png", "", "", TextureResType::LOCAL);
	wp->addClickEventListener(CC_CALLBACK_0(WeaponPanel::chickPanel, wp));
	wp->setAnchorPoint(cocos2d::Vec2::ZERO);
	wp->weaponPath = weaponPath;
	wp->pData = pData;
	wp->window = window;
	//加载武器图片
	Sprite* weaponIcon = Sprite::create(weaponPath);
	weaponIcon->setAnchorPoint(cocos2d::Vec2(0.5, 0.5)); //图标居中显示
	weaponIcon->setNormalizedPosition(cocos2d::Vec2(0.2, 0.5));
	wp->addChild(weaponIcon);
	//加载武器拥有的gem
	auto gemIcons = pData->findPairsByKey("gem"); //查找武器拥有的所有gem
	for (auto i = gemIcons.begin(); i != gemIcons.end(); i++) {
		gem* g = gem::getGem(window->pUserDataLoader->findDataByLabel(*i)); //查找实例并产生一个gem
		wp->gemInWeapon.push_back(g);
	}
	return wp;
}

void game::FitWeaponWindow::WeaponPanel::chickPanel()
{
	//传入一个this指针代表自己的身份，让window显示武器信息
	cocos2d::log("chick panel!");
	window->chooseWeapon(this);
}

/*create函数是可以直接重载的，不需要用create_func，这是因为init与类型无关*/
game::FitWeaponWindow::WeaponPanel * game::FitWeaponWindow::WeaponPanel::create(const std::string & normalImage, const std::string & selectedImage, const std::string & disableImage, TextureResType texType)
{
	WeaponPanel *btn = new (std::nothrow) WeaponPanel;
	if (btn && btn->init(normalImage, selectedImage, disableImage, texType))
	{
		btn->autorelease();
		return btn;
	}
	CC_SAFE_DELETE(btn);
	return nullptr;
}

void game::FitWeaponWindow::addGem(gem * g)
{
	//写入到storage中
	_window->storage->addPair("gem", g->instanceName);
	//添加panel
	auto panel = GemPanel::create(_window, g);
	panel->setAnchorPoint(cocos2d::Vec2::ZERO);
	//panel->setPosition(cocos2d::Vec2(8, _window->gemScrollView->getInnerContainerSize().height - 80 * _window->gemScrollView->getChildrenCount());
	//添加到vec中
	_window->gemPanelVec.push_back(panel);
	//添加到scroll中
	_window->gemScrollView->addChild(panel);
	//调用刷新函数（只刷新列表排序）
	_window->chooseWeapon(_window->showingWeaponPanel);
	_window->flush("left");
}

void game::FitWeaponWindow::deleteGemPanel(game::FitWeaponWindow::GemPanel * gp)
{
	//从data的storage中删除
	_window->storage->deletePair("gem", gp->pGem->instanceName, false);
	//删除panel(child和vec)
	_window->gemScrollView->removeChild(gp, false);
	for (auto i = _window->gemPanelVec.begin(); i != _window->gemPanelVec.end(); i++) {
		if ((*i) == gp) {
			_window->gemPanelVec.erase(i);
			cocos2d::log("INFO: removed GemPanel from gemPanelVec");
			break;
		}
	}
	//刷新
	_window->flush("left");
}

void game::FitWeaponWindow::flush(std::string pos)
{
	int count = 1; //刷新计数器
	if (pos == "left") {
		auto vec = _window->gemScrollView->getChildren();
		float maxHeight = _window->gemScrollView->getInnerContainerSize().height;
		for (auto i = vec.begin(); i != vec.end(); i++) {
			(*i)->setPosition(0, maxHeight - count * 80);
			count++;
		}
		return;
	}
	if (pos == "right") {
		auto vec = _window->weaponScrollView->getChildren();
		float maxHeight = _window->weaponScrollView->getInnerContainerSize().height;
		for (auto i = vec.begin(); i != vec.end(); i++) {
			(*i)->setPosition(0, maxHeight - count * 80);
			count++;
		}
		return;
	}
}

std::string game::FitWeaponWindow::getPercent(float x)
{
	return std::to_string((int)(x * 100)) + "%";
}

std::string game::FitWeaponWindow::shortenNum(float x)
{
	std::string s;
	std::stringstream ss;
	ss << ss.precision(2) << x;
	ss >> s;
	return s;
}

game::FitWeaponWindow * game::FitWeaponWindow::create(stationScene * s)
{
	//无论是否存在都要重新创建，因为其他操作可能改变数据
	if (_window != nullptr) _window->release();
	_window = FitWeaponWindow::create();
	_window->setSpriteFrame(Sprite::create("UI//UI-station-weapon.png")->getSpriteFrame()); //这个地方可以更改成用缓存加载
	_window->scene = s; //初始化
	_window->pGameDataLoader = s->pGameDataLoader;
	_window->pUserDataLoader = s->pUserDataLoader;
	//获取玩家拥有的武器实例
	_window->storage = s->pUserDataLoader->findDataByLabel("storage");
	std::vector<std::string> weaponStrVec = _window->storage->findPairsByKey("weapon");
	//获取玩家拥有的宝石实例
	std::vector<std::string> gemStrVec = _window->storage->findPairsByKey("gem");
	//对武器依次进行初始化
	for (auto i = weaponStrVec.begin(); i != weaponStrVec.end(); i++) {
		//这里应当是浅色和深色固定交替的，暂时不做
		_window->weaponPanelVec.push_back(WeaponPanel::create(_window,
			_window->pUserDataLoader->findDataByLabel(*i),
			_window->pGameDataLoader->findDataByLabel(_window->pUserDataLoader->findDataByLabel(*i)->findPairByKey("weaponType")->value)->findPairByKey("texturePath")->value,
			std::string("UI//UI-weaponPanel-white.png"))
		);
	}
	//对宝石依次进行初始化
	for (auto i = gemStrVec.begin(); i != gemStrVec.end(); i++) {
		_window->gemPanelVec.push_back(GemPanel::create(_window, gem::getGem(
			_window->pUserDataLoader->findDataByLabel(*i)
		)));
	}
	//调整自身位置
	_window->setAnchorPoint(cocos2d::Vec2::ZERO);
	_window->setPosition(228, 768 - 642);
	//加载左侧的显示条
	_window->weaponScrollView = cocos2d::ui::ScrollView::create();
	_window->weaponScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	_window->weaponScrollView->setSize(cocos2d::Size(200, 475));//这是根据UI界面制定的，
	_window->weaponScrollView->setInnerContainerSize(cocos2d::Size(200, 2000)); //这个size的作用还不清楚，目前认为是实际能存放的大小
	_window->weaponScrollView->setAnchorPoint(cocos2d::Vec2::ZERO);
	_window->weaponScrollView->setPosition(cocos2d::Vec2(538, 768 - 744)); //相对于panel来说的
	_window->addChild(_window->weaponScrollView);
	//向左侧显示条中加载panel
	int i = 1;
	for (auto j = _window->weaponPanelVec.begin(); j != _window->weaponPanelVec.end(); j++) {
		//TODO: 这里应该根据每一栏的大小动态调整
		(*j)->setPosition(cocos2d::Vec2(0, 2000 - i * 84)); //按照顺序进行排列
		_window->weaponScrollView->addChild((*j));
		i++;
	}
	//加载右侧显示条
	_window->gemScrollView = cocos2d::ui::ScrollView::create();
	_window->gemScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	_window->gemScrollView->setSize(cocos2d::Size(412 - 236, 475));//这是根据UI界面制定的，
	_window->gemScrollView->setInnerContainerSize(cocos2d::Size(412 - 236, 2000)); //这个size的作用还不清楚，目前认为是实际能存放的大小
	_window->gemScrollView->setAnchorPoint(cocos2d::Vec2::ZERO);
	_window->gemScrollView->setPosition(cocos2d::Vec2(8, 768 - 744)); //相对于panel来说的
	_window->addChild(_window->gemScrollView);
	//向右侧显示条添加panel
	i = 1;
	for (auto j = _window->gemPanelVec.begin(); j != _window->gemPanelVec.end(); j++) {
		(*j)->setAnchorPoint(cocos2d::Vec2::ZERO);
		(*j)->setPosition(cocos2d::Vec2(8, 2000 - 80.0*i));
		_window->gemScrollView->addChild(*j);
		i++;
	}
	//刚开始不显示文字信息
	_window->weaponName = nullptr;
	_window->story = nullptr;
	_window->performance = nullptr;
	_window->gemGain = nullptr;
	_window->weaponImage = nullptr;
	//初始化其他参数
	_window->showingWeaponPanel = nullptr;
	return _window;
}

void game::FitWeaponWindow::chooseWeapon(FitWeaponWindow::WeaponPanel * wp)
{
	cocos2d::log("INFO: *******chooseWeapon()*******");
	//设置flag
	cocos2d::log("INFO: setting flag");
	_window->showingWeaponPanel = wp;
	cocos2d::log("INFO: showing weapon picture");
	//添加放大后的武器图片到展示栏中
	if (_window->weaponImage != nullptr) _window->removeChild(_window->weaponImage, false);
	_window->weaponImage = cocos2d::ui::Button::create(wp->weaponPath, "", "", cocos2d::ui::Widget::TextureResType::LOCAL);
	_window->weaponImage->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	_window->weaponImage->setPosition(cocos2d::Vec2(365, 768 - 342));
	_window->weaponImage->setScale9Enabled(false);
	_window->weaponImage->setScale(3);
	_window->addChild(_window->weaponImage);
	//显示镶嵌上的gem
	cocos2d::log("INFO: showing gemIcon");
	int gemPositionCount = 0;
	//清空之前的icon
	auto childrenVec = _window->getChildren();
	for (auto i = childrenVec.begin(); i != childrenVec.end(); i++) {
		if ((*i)->getName() == "gemIcon") _window->removeChild(*i,false);
	}
	//_window->removeChildByName("gemIcon",false); //先清空之前的icon
	for (auto i = wp->gemInWeapon.begin(); i != wp->gemInWeapon.end(); i++) {
		auto icon = (*i)->getGemIcon();
		icon->setName("gemIcon"); //便于批量删除
		icon->addClickEventListener(CC_CALLBACK_1(WeaponPanel::gemIconCallback, wp));//设置图标被点击时的删除的回调函数
		//这里加上根据count排列坐标
		icon->setAnchorPoint(cocos2d::Vec2::ZERO);
		icon->setPosition(cocos2d::Vec2(322 + gemPositionCount * 43, 768 - 462));
		gemPositionCount++;
		_window->addChild(icon);
	}

	//刷新中间panel的文字说明，并计算gain
	cocos2d::log("INFO: removing old text");
	if (gemGain != nullptr) _window->removeChild(gemGain, false);
	if (weaponName != nullptr) _window->removeChild(weaponName, false);
	if (story != nullptr) _window->removeChild(story, false);
	if (performance != nullptr)_window->removeChild(performance, false);
	//更新总增益
	cocos2d::log("INFO: calculating gain");
	float gain = gem::getGain(&wp->gemInWeapon);
	std::string perfStr;
	cocos2d::log("INFO: setting attribute text");
	auto perfVec = wp->pData->findAttrs(); //属性vec
	for (auto j = perfVec.begin(); j != perfVec.end(); j++) {
		float fixedAttrValue = j->attrValue + gain * j->attrCoef; //计算增益后的数值
		if (perfStr.empty() == false) perfStr += "\n";//第二行开始换行
		perfStr += j->attrStr + ": " + shortenNum(j->attrValue) + "(" + shortenNum(j->attrCoef) + ")" + "->" + shortenNum(fixedAttrValue);
	}
	//添加到performance上
	cocos2d::log("INFO: setting performance text");
	performance = cocos2d::Label::create(perfStr, "fonts//msyh.ttf", 24);
	performance->setAnchorPoint(cocos2d::Vec2(0, 1)); //悬挂左上角
	performance->setPosition(187, 768 - 466);
	this->addChild(performance);
	//添加gemGain
	cocos2d::log("INFO: setting gainLabel");
	std::string gainStr = LanguageManager::getInstance()->getStringForKey("gainLabel") + getPercent(gain);
	gemGain = cocos2d::Label::create(gainStr, "fonts//msyh.ttf", 24);
	gemGain->setColor(cocos2d::Color3B(14, 184, 255));
	gemGain->setAnchorPoint(cocos2d::Vec2(0, 0.5));
	gemGain->setPosition(189, 768 - 442);
	this->addChild(gemGain);
	//TODO: 添加story和weaponName相关的东西
}

game::FitWeaponWindow::GemPanel * game::FitWeaponWindow::GemPanel::create(const std::string & normalImage, const std::string & selectedImage, const std::string & disableImage, TextureResType texType)
{
	GemPanel *btn = new (std::nothrow) GemPanel;
	if (btn && btn->init(normalImage, selectedImage, disableImage, texType))
	{
		btn->autorelease();
		return btn;
	}
	CC_SAFE_DELETE(btn);
	return nullptr;
}

game::FitWeaponWindow::GemPanel * game::FitWeaponWindow::GemPanel::create(FitWeaponWindow * window, gem * g)
{
	GemPanel *gp;
	//获取宝石类型
	gp = GemPanel::create("UI//UI-gemPanel-white.png", "", "", TextureResType::LOCAL);
	gp->window = window;
	gp->pGem = g;
	//根据宝石类型添加一个icon, gem中的指针会自动更新
	auto icon = g->getGemIcon();
	icon->setNormalizedPosition(cocos2d::Vec2(0.2, 0.5));
	gp->addClickEventListener(CC_CALLBACK_1(GemPanel::chickPanel, gp));
	gp->addChild(icon);
	return gp;
}

void game::FitWeaponWindow::GemPanel::chickPanel(Ref * sender)
{
	cocos2d::log("chickGemPanel!");
	//如果没有选中weapon,那么什么都不做
	if (window->showingWeaponPanel == nullptr) return;
	//如果宝石槽已经满了，也不添加
	if (window->showingWeaponPanel != nullptr && window->showingWeaponPanel->gemInWeapon.size() == 5) return;
	//根据icon寻找对应的gem
	auto gp = dynamic_cast<GemPanel*>(sender);
	gem* g = gp->pGem;
	//从storage中删除这个gem(包括panel和data中的数据)
	window->deleteGemPanel(gp);
	//将这个gem添加到当前选择的weapon中
	window->showingWeaponPanel->addGem(g);
}