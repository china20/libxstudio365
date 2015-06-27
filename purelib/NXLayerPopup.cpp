#include "utils/super_cast.h"
#include "NXNodesLayout.h"
#include "NXNodesUtility.h"
#include "NXLayerPopup.h"

namespace purelib {

enum {
    //* priority used by the LayerPopup for the event handler
    kLayerPopupPriority = 1,
};

// std::stack<cocos2d::Node*> CCExtLayerPopup::displayStack;

LayerPopup::LayerPopup(void) : m_closeWhenTouchEnd(false)
{
    this->loadingBackground = nullptr;
    this->closeButton = nullptr;
    this->closed = false;
    this->closeable = true;
    this->m_bPopped = false;
    this->setPoppedCallback();
    this->setClosingCallback();
    this->setClosedCallback();
    this->result = kPopupResultNone;
}

LayerPopup::~LayerPopup(void)
{
}

bool LayerPopup::init(void)
{
	if(!CCLayer::init())
    {
        return false;
    }
    
     
    // this->setTouchPriority(kLayerPopupPriority/*equal to CCControl*/);
    this->setTouchMode(kCCTouchesOneByOne); // only work when touchEnabled
    this->setTouchEnabled(true); // only work when running (after worker)


    this->setAnchorPoint(CCPointMake(0.5f, 0.5f));
    this->ignoreAnchorPointForPosition(false);
    this->setPosition(CCPointZero);

    this->setCloseWhenTouchEnd(false);

    return true;
}

void LayerPopup::pushNodeToHidden(cocos2d::Node* toHidden)
{
    toHidden->retain();
    this->othersNeedsHidden.push_back(toHidden);
}

void LayerPopup::pushNodesToHidden(const std::vector<cocos2d::Node*>& othersToHidden)
{
    std::for_each(othersToHidden.begin(), othersToHidden.end(), [](cocos2d::Node* toHidden)->void {
        toHidden->retain();
    });
    this->othersNeedsHidden.insert(this->othersNeedsHidden.end(), othersToHidden.begin(), othersToHidden.end());
}

void LayerPopup::setPoppedCallback(const std::function<void(void)>& callback)
{
    this->popupedCallback = callback;
}

void LayerPopup::setClosingCallback(const std::function<void(void)>& callback)
{
    this->closingCallback = callback;
}

void LayerPopup::setClosedCallback(const std::function<void(int)>& callback)
{
    this->closedCallback = callback;
}

void LayerPopup::popup(cocos2d::Node* parent, int tag, cocos2d::Node* otherToHidden)
{
    CC_ASSERT(parent);

    if(tag != kCCNodeTagInvalid && parent->getChildByTag(tag) != nullptr)
    {
        this->release();
        return;
    }

    this->closed = false;

    if (otherToHidden != nullptr)
    {
        this->pushNodeToHidden(otherToHidden);
    }

    parent->addChild(this, 1, tag);
	cocos2d::Size ps = this->getParent()->getContentSize();
	nodes_layout::vscr::setNodeNormalizedPosition(this, ccp(0.5f, 0.5f));
    this->runPopupAction();
}

void LayerPopup::showCloseButton(float scale)
{
    if(this->closeButton == nullptr)
    {
        this->closeButton = nodes_utility::a_button("images/common/9.png", nullptr, nullptr, nullptr);
        this->addChild(this->closeButton, 1);
        //this->closeButton->release();
        nodes_layout::setNodeLB(this->closeButton, CCPointMake(20, 20));
        this->closeButton->addTargetWithActionForControlEvents(this, cccontrol_selector(LayerPopup::closeButton_TouchUpinside), Control::EventType::TOUCH_UP_INSIDE);
        this->closeButton->setPreferredSize(this->closeButton->getContentSize() * scale);
    }
    else {
        this->closeButton->setVisible(true);
    }
}

void LayerPopup::hideCloseButton(void)
{
    if(this->closeButton != nullptr)
    {
        this->closeButton->setVisible(false);
    }
}

void LayerPopup::setCloseable(bool bCloseable)
{
    this->closeable = bCloseable;
    this->setCloseButtonEnabled(bCloseable);
}

void LayerPopup::enableCloseButton(void)
{
    this->setCloseButtonEnabled(true);
}

void LayerPopup::disableCloseButton(void)
{
    this->setCloseButtonEnabled(false);
}

void LayerPopup::setCloseButtonEnabled(bool enabled)
{
    if(this->closeButton != nullptr)
    {
        this->closeButton->setEnabled(enabled);
    }
}

void LayerPopup::closeButton_TouchUpinside(CCObject*, Control::EventType)
{ 
    CCLOG("CCExtLayerPopup::closeButton_TouchUpinside");
    this->result = kPopupResultCancel;
    this->close();
}

void LayerPopup::runPopupAction(void)
{
    this->setScale(0.0f);
    nodes_layout::centerNode(this);

    //const CCSize& size = this->getContentSize();
    CCAction* popupAction = CCSequence::create(
        CCSpawn::create(CCScaleTo::create(0.15f, 1.0f, 1.0f), 
        CCFadeIn::create(0.15f), nullptr), 
        CCCallFunc::create(this, callfunc_selector(LayerPopup::callbackShowNodeWhenDidAction)),
        nullptr);

    this->runAction(popupAction);
}

void LayerPopup::close(bool force, float closeDelayTime)
{
    CCLOG("CCExtLayerPopup::close");
    if( (!this->closeable && !force) || this->closed) {
        return;
    }
    this->closed = true;
    this->setTouchEnabled(false);
    this->closeDelayTime = closeDelayTime;
    this->closingCallback();
    //const CCSize& size = this->getContentSize();
    CCAction* popinAction = CCSequence::create(
        CCSpawn::create(
        CCScaleTo::create(0.15f, 0.0f, 0.0f),
        CCFadeOut::create(0.15f), 
        nullptr ), 
        CCDelayTime::create(this->closeDelayTime),
        CCCallFunc::create(this, callfunc_selector(LayerPopup::callbackRemoveNodeWhenDidAction)), 
        nullptr);
    this->runAction(popinAction);
}

void LayerPopup::callbackShowNodeWhenDidAction(void)
{
    this->m_bPopped = true;

    this->popupedCallback();

    std::for_each(this->othersNeedsHidden.begin(), this->othersNeedsHidden.end(), [](cocos2d::Node* toHidden){
        toHidden->setVisible(false);
    });


    /*if (!CCExtLayerPopup::displayStack.empty())
    {
    CCExtLayerPopup::displayStack.top()->setVisible(false);
    }
    CCExtLayerPopup::displayStack.push(this);*/
}

void LayerPopup::callbackRemoveNodeWhenDidAction(void)
{
    this->closedCallback(this->result);

    std::for_each(this->othersNeedsHidden.begin(), this->othersNeedsHidden.end(), [](cocos2d::Node* toHidden){
        toHidden->setVisible(true);
        toHidden->release();
    });

    // this->othersNeedsHidden.clear();
    // this->pauseSchedulerAndActions();
    this->removeFromParent();

    /*CCExtLayerPopup::displayStack.pop();
    if (!CCExtLayerPopup::displayStack.empty())
    {
    CCExtLayerPopup::displayStack.top()->setVisible(true);
    }*/
}

bool LayerPopup::onTouchBegan(CCTouch*, CCEvent*)
{
    return true;
}

void LayerPopup::onTouchEnded(CCTouch *pTouch, CCEvent *pEvent)
{
    if(this->m_closeWhenTouchEnd && !(this->boundingBox().containsPoint(this->getParent()->convertTouchToNodeSpace(pTouch)) ) )
    {
        this->close();
    }
}

void LayerPopup::suspendLayout(cocos2d::Node* ui)
{
    if(!ui->isVisible()) return;

    this->disableCloseButton();
    ui->setVisible(false);

    if(this->loadingBackground == nullptr) {
        // create background
        this->loadingBackground = CCSprite::create();
        // ScrollView* scrollView = dynamic_cast<ScrollView*>(ui);
        /*if(scrollView != nullptr) {
        this->loadingBackground->setTextureRect(CCRectMake(0, 0, scrollView->getViewSize().width, scrollView->getViewSize().height));
        }
        else {*/
        this->loadingBackground->setTextureRect(CCRectMake(0, 0, ui->getContentSize().width, ui->getContentSize().height));
        //}
        this->loadingBackground->setColor(ccc3(0, 0, 0));
        this->loadingBackground->setOpacity(75);
        this->addChild(this->loadingBackground);
        /*this->loadingBackground->setPosition(ui->getPosition());*/
        nodes_layout::setNodePosition(this->loadingBackground, nodes_layout::ALIGN_LT, nodes_layout::getNodeLeft(ui), nodes_layout::getNodeTop(ui));

        // create prompt
        //Label* prompt = Label::create(nodes_layout::::getStringByKey("loading"), FONT_MINIREALWIDESTD, 36);
        //this->loadingBackground->addChild(prompt);
        //prompt->setDimensions(CCSizeMake(386.0, 82.0));
        //prompt->setColor(ccc3(0, 192, 192));
        //prompt->setHorizontalAlignment(kCCTextAlignmentLeft);
        //prompt->setVerticalAlignment(kCCVerticalTextAlignmentCenter);
        //nodes_layout::centerNode(prompt/*, xg::consts::ALIGN_LT, 126, 526*/);

        // this->loadingBackground->retain();
    }
    else {
        this->loadingBackground->setVisible(true);
    }
}

void LayerPopup::resumeLayout(cocos2d::Node* ui)
{
    if(ui->isVisible()) return;

    this->setPoppedCallback([](void){});
    this->loadingBackground->setVisible(false);
    ui->setVisible(true);
    this->enableCloseButton();
}

};
