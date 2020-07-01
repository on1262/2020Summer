#include "FitWeaponWindow.h"
#include "LanguageManager.h"
#include <sstream>
game::FitWeaponWindow *game::FitWeaponWindow::_window = nullptr;

void game::FitWeaponWindow::WeaponPanel::addGem(gem *g)
{
	//��ӵ�data��
	this->pData->addPair("gem", g->instanceName);
	//��ӵ�vec��
	this->gemInWeapon.push_back(g);
	//ˢ���в�
	_window->chooseWeapon(this);
}

bool game::FitWeaponWindow::WeaponPanel::deleteGem(gem *g)
{
	//��ɾ��icon
	auto icon = dynamic_cast<cocos2d::ui::Button*> (g->iconRef);
	window->removeChild(icon, false);
	//��weapon��data��ɾ����һ��ʵ��
	/*ֻɾ��һ��ʵ����������false,��Ϊdata����ʵʱӰ��gem,���´δ����ʱ���Զ�����data���£���������νɾ���ĸ�*/
	pData->deletePair(g->instanceName, false);
	//ɾ��vec�е�gem
	auto iter = gemInWeapon.begin();
	for (auto i = gemInWeapon.begin(); i != gemInWeapon.end(); i++) {
		if ((*i) == g) {
			//�����ͬ������
			iter = i;
			cocos2d::log("INFO: deleting iter in gemInWeapon");
			break;
		}
	}
	gemInWeapon.erase(iter);
	return false;
}

void game::FitWeaponWindow::WeaponPanel::gemIconCallback(Ref* sender) //���ͼ�꣬��gemת�Ƶ�storage
{
	gem* g;
	//���ȶ�sender���н�����ȷ�����ĸ�gem��image
	for (auto i = gemInWeapon.begin(); i != gemInWeapon.end(); i++) {
		if ((*i)->iconRef == sender) {
			g = *i;
			cocos2d::log("INFO: Find sender in gemIconCallback()");
			break;
		}
	}
	//ע�⣺��Щ�������ܷ���forѭ�����ΪdeleteGem�Ѿ���*iɾ���ˣ���������*i��ӵ�window
	cocos2d::log("INFO: deleting gem in WeaponPanel");
	this->deleteGem(g); //��������ɾ�����gem
	cocos2d::log("INFO: adding gem to window");
	window->addGem(g);//��storage���gem
}

game::FitWeaponWindow::WeaponPanel * game::FitWeaponWindow::WeaponPanel::create(FitWeaponWindow* window, Data * pData, std::string weaponPath, std::string backgroundPath) {
	WeaponPanel* wp;
	wp = WeaponPanel::create("UI//UI-weaponPanel-white.png", "", "", TextureResType::LOCAL);
	wp->addClickEventListener(CC_CALLBACK_0(WeaponPanel::chickPanel, wp));
	wp->setAnchorPoint(cocos2d::Vec2::ZERO);
	wp->weaponPath = weaponPath;
	wp->pData = pData;
	wp->window = window;
	//��������ͼƬ
	Sprite* weaponIcon = Sprite::create(weaponPath);
	weaponIcon->setAnchorPoint(cocos2d::Vec2(0.5, 0.5)); //ͼ�������ʾ
	weaponIcon->setNormalizedPosition(cocos2d::Vec2(0.2, 0.5));
	wp->addChild(weaponIcon);
	//��������ӵ�е�gem
	auto gemIcons = pData->findPairsByKey("gem"); //��������ӵ�е�����gem
	for (auto i = gemIcons.begin(); i != gemIcons.end(); i++) {
		gem* g = gem::getGem(window->pUserDataLoader->findDataByLabel(*i)); //����ʵ��������һ��gem
		wp->gemInWeapon.push_back(g);
	}
	return wp;
}

void game::FitWeaponWindow::WeaponPanel::chickPanel()
{
	//����һ��thisָ������Լ�����ݣ���window��ʾ������Ϣ
	cocos2d::log("chick panel!");
	window->chooseWeapon(this);
}

/*create�����ǿ���ֱ�����صģ�����Ҫ��create_func��������Ϊinit�������޹�*/
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
	//д�뵽storage��
	_window->storage->addPair("gem", g->instanceName);
	//���panel
	auto panel = GemPanel::create(_window, g);
	panel->setAnchorPoint(cocos2d::Vec2::ZERO);
	//panel->setPosition(cocos2d::Vec2(8, _window->gemScrollView->getInnerContainerSize().height - 80 * _window->gemScrollView->getChildrenCount());
	//��ӵ�vec��
	_window->gemPanelVec.push_back(panel);
	//��ӵ�scroll��
	_window->gemScrollView->addChild(panel);
	//����ˢ�º�����ֻˢ���б�����
	_window->chooseWeapon(_window->showingWeaponPanel);
	_window->flush("left");
}

void game::FitWeaponWindow::deleteGemPanel(game::FitWeaponWindow::GemPanel * gp)
{
	//��data��storage��ɾ��
	_window->storage->deletePair("gem", gp->pGem->instanceName, false);
	//ɾ��panel(child��vec)
	_window->gemScrollView->removeChild(gp, false);
	for (auto i = _window->gemPanelVec.begin(); i != _window->gemPanelVec.end(); i++) {
		if ((*i) == gp) {
			_window->gemPanelVec.erase(i);
			cocos2d::log("INFO: removed GemPanel from gemPanelVec");
			break;
		}
	}
	//ˢ��
	_window->flush("left");
}

void game::FitWeaponWindow::flush(std::string pos)
{
	int count = 1; //ˢ�¼�����
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
	//�����Ƿ���ڶ�Ҫ���´�������Ϊ�����������ܸı�����
	if (_window != nullptr) _window->release();
	_window = FitWeaponWindow::create();
	_window->setSpriteFrame(Sprite::create("UI//UI-station-weapon.png")->getSpriteFrame()); //����ط����Ը��ĳ��û������
	_window->scene = s; //��ʼ��
	_window->pGameDataLoader = s->pGameDataLoader;
	_window->pUserDataLoader = s->pUserDataLoader;
	//��ȡ���ӵ�е�����ʵ��
	_window->storage = s->pUserDataLoader->findDataByLabel("storage");
	std::vector<std::string> weaponStrVec = _window->storage->findPairsByKey("weapon");
	//��ȡ���ӵ�еı�ʯʵ��
	std::vector<std::string> gemStrVec = _window->storage->findPairsByKey("gem");
	//���������ν��г�ʼ��
	for (auto i = weaponStrVec.begin(); i != weaponStrVec.end(); i++) {
		//����Ӧ����ǳɫ����ɫ�̶�����ģ���ʱ����
		_window->weaponPanelVec.push_back(WeaponPanel::create(_window,
			_window->pUserDataLoader->findDataByLabel(*i),
			_window->pGameDataLoader->findDataByLabel(_window->pUserDataLoader->findDataByLabel(*i)->findPairByKey("weaponType")->value)->findPairByKey("texturePath")->value,
			std::string("UI//UI-weaponPanel-white.png"))
		);
	}
	//�Ա�ʯ���ν��г�ʼ��
	for (auto i = gemStrVec.begin(); i != gemStrVec.end(); i++) {
		_window->gemPanelVec.push_back(GemPanel::create(_window, gem::getGem(
			_window->pUserDataLoader->findDataByLabel(*i)
		)));
	}
	//��������λ��
	_window->setAnchorPoint(cocos2d::Vec2::ZERO);
	_window->setPosition(228, 768 - 642);
	//����������ʾ��
	_window->weaponScrollView = cocos2d::ui::ScrollView::create();
	_window->weaponScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	_window->weaponScrollView->setSize(cocos2d::Size(200, 475));//���Ǹ���UI�����ƶ��ģ�
	_window->weaponScrollView->setInnerContainerSize(cocos2d::Size(200, 2000)); //���size�����û��������Ŀǰ��Ϊ��ʵ���ܴ�ŵĴ�С
	_window->weaponScrollView->setAnchorPoint(cocos2d::Vec2::ZERO);
	_window->weaponScrollView->setPosition(cocos2d::Vec2(538, 768 - 744)); //�����panel��˵��
	_window->addChild(_window->weaponScrollView);
	//�������ʾ���м���panel
	int i = 1;
	for (auto j = _window->weaponPanelVec.begin(); j != _window->weaponPanelVec.end(); j++) {
		//TODO: ����Ӧ�ø���ÿһ���Ĵ�С��̬����
		(*j)->setPosition(cocos2d::Vec2(0, 2000 - i * 84)); //����˳���������
		_window->weaponScrollView->addChild((*j));
		i++;
	}
	//�����Ҳ���ʾ��
	_window->gemScrollView = cocos2d::ui::ScrollView::create();
	_window->gemScrollView->setDirection(cocos2d::ui::ScrollView::Direction::VERTICAL);
	_window->gemScrollView->setSize(cocos2d::Size(412 - 236, 475));//���Ǹ���UI�����ƶ��ģ�
	_window->gemScrollView->setInnerContainerSize(cocos2d::Size(412 - 236, 2000)); //���size�����û��������Ŀǰ��Ϊ��ʵ���ܴ�ŵĴ�С
	_window->gemScrollView->setAnchorPoint(cocos2d::Vec2::ZERO);
	_window->gemScrollView->setPosition(cocos2d::Vec2(8, 768 - 744)); //�����panel��˵��
	_window->addChild(_window->gemScrollView);
	//���Ҳ���ʾ�����panel
	i = 1;
	for (auto j = _window->gemPanelVec.begin(); j != _window->gemPanelVec.end(); j++) {
		(*j)->setAnchorPoint(cocos2d::Vec2::ZERO);
		(*j)->setPosition(cocos2d::Vec2(8, 2000 - 80.0*i));
		_window->gemScrollView->addChild(*j);
		i++;
	}
	//�տ�ʼ����ʾ������Ϣ
	_window->weaponName = nullptr;
	_window->story = nullptr;
	_window->performance = nullptr;
	_window->gemGain = nullptr;
	_window->weaponImage = nullptr;
	//��ʼ����������
	_window->showingWeaponPanel = nullptr;
	return _window;
}

void game::FitWeaponWindow::chooseWeapon(FitWeaponWindow::WeaponPanel * wp)
{
	cocos2d::log("INFO: *******chooseWeapon()*******");
	//����flag
	cocos2d::log("INFO: setting flag");
	_window->showingWeaponPanel = wp;
	cocos2d::log("INFO: showing weapon picture");
	//��ӷŴ�������ͼƬ��չʾ����
	if (_window->weaponImage != nullptr) _window->removeChild(_window->weaponImage, false);
	_window->weaponImage = cocos2d::ui::Button::create(wp->weaponPath, "", "", cocos2d::ui::Widget::TextureResType::LOCAL);
	_window->weaponImage->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	_window->weaponImage->setPosition(cocos2d::Vec2(365, 768 - 342));
	_window->weaponImage->setScale9Enabled(false);
	_window->weaponImage->setScale(3);
	_window->addChild(_window->weaponImage);
	//��ʾ��Ƕ�ϵ�gem
	cocos2d::log("INFO: showing gemIcon");
	int gemPositionCount = 0;
	//���֮ǰ��icon
	auto childrenVec = _window->getChildren();
	for (auto i = childrenVec.begin(); i != childrenVec.end(); i++) {
		if ((*i)->getName() == "gemIcon") _window->removeChild(*i,false);
	}
	//_window->removeChildByName("gemIcon",false); //�����֮ǰ��icon
	for (auto i = wp->gemInWeapon.begin(); i != wp->gemInWeapon.end(); i++) {
		auto icon = (*i)->getGemIcon();
		icon->setName("gemIcon"); //��������ɾ��
		icon->addClickEventListener(CC_CALLBACK_1(WeaponPanel::gemIconCallback, wp));//����ͼ�걻���ʱ��ɾ���Ļص�����
		//������ϸ���count��������
		icon->setAnchorPoint(cocos2d::Vec2::ZERO);
		icon->setPosition(cocos2d::Vec2(322 + gemPositionCount * 43, 768 - 462));
		gemPositionCount++;
		_window->addChild(icon);
	}

	//ˢ���м�panel������˵����������gain
	cocos2d::log("INFO: removing old text");
	if (gemGain != nullptr) _window->removeChild(gemGain, false);
	if (weaponName != nullptr) _window->removeChild(weaponName, false);
	if (story != nullptr) _window->removeChild(story, false);
	if (performance != nullptr)_window->removeChild(performance, false);
	//����������
	cocos2d::log("INFO: calculating gain");
	float gain = gem::getGain(&wp->gemInWeapon);
	std::string perfStr;
	cocos2d::log("INFO: setting attribute text");
	auto perfVec = wp->pData->findAttrs(); //����vec
	for (auto j = perfVec.begin(); j != perfVec.end(); j++) {
		float fixedAttrValue = j->attrValue + gain * j->attrCoef; //������������ֵ
		if (perfStr.empty() == false) perfStr += "\n";//�ڶ��п�ʼ����
		perfStr += j->attrStr + ": " + shortenNum(j->attrValue) + "(" + shortenNum(j->attrCoef) + ")" + "->" + shortenNum(fixedAttrValue);
	}
	//��ӵ�performance��
	cocos2d::log("INFO: setting performance text");
	performance = cocos2d::Label::create(perfStr, "fonts//msyh.ttf", 24);
	performance->setAnchorPoint(cocos2d::Vec2(0, 1)); //�������Ͻ�
	performance->setPosition(187, 768 - 466);
	this->addChild(performance);
	//���gemGain
	cocos2d::log("INFO: setting gainLabel");
	std::string gainStr = LanguageManager::getInstance()->getStringForKey("gainLabel") + getPercent(gain);
	gemGain = cocos2d::Label::create(gainStr, "fonts//msyh.ttf", 24);
	gemGain->setColor(cocos2d::Color3B(14, 184, 255));
	gemGain->setAnchorPoint(cocos2d::Vec2(0, 0.5));
	gemGain->setPosition(189, 768 - 442);
	this->addChild(gemGain);
	//TODO: ���story��weaponName��صĶ���
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
	//��ȡ��ʯ����
	gp = GemPanel::create("UI//UI-gemPanel-white.png", "", "", TextureResType::LOCAL);
	gp->window = window;
	gp->pGem = g;
	//���ݱ�ʯ�������һ��icon, gem�е�ָ����Զ�����
	auto icon = g->getGemIcon();
	icon->setNormalizedPosition(cocos2d::Vec2(0.2, 0.5));
	gp->addClickEventListener(CC_CALLBACK_1(GemPanel::chickPanel, gp));
	gp->addChild(icon);
	return gp;
}

void game::FitWeaponWindow::GemPanel::chickPanel(Ref * sender)
{
	cocos2d::log("chickGemPanel!");
	//���û��ѡ��weapon,��ôʲô������
	if (window->showingWeaponPanel == nullptr) return;
	//�����ʯ���Ѿ����ˣ�Ҳ�����
	if (window->showingWeaponPanel != nullptr && window->showingWeaponPanel->gemInWeapon.size() == 5) return;
	//����iconѰ�Ҷ�Ӧ��gem
	auto gp = dynamic_cast<GemPanel*>(sender);
	gem* g = gp->pGem;
	//��storage��ɾ�����gem(����panel��data�е�����)
	window->deleteGemPanel(gp);
	//�����gem��ӵ���ǰѡ���weapon��
	window->showingWeaponPanel->addGem(g);
}