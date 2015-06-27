/****************************************************************************
Copyright (c) 2014 purelib, All Rights Reserved.
****************************************************************************/
#include "NXNodesLayout.h"
#include "NXProgressLayer.h"
#include "NXActivityIndicator.h"

using namespace purelib;

#define kProgressLayerBackgroundShowOpacity 0

enum {
    kProgressLayerPriority = INT_MIN
};

bool ProgressLayer::init()
{
    if (!CCLayerColor::initWithColor(ccc4(0, 0, 0, 0))) return false;
    
	this->setTag(128);
	// this->setTouchPriority(kProgressLayerPriority/*equal to CCControl*/);
    Director::getInstance()->getEventDispatcher()->setPriority(this->_touchListener, kProgressLayerPriority);
    this->setTouchMode(kCCTouchesOneByOne); // only work when touchEnabled
    this->setTouchEnabled(true); // only work when running (after worker)
    this->setZOrder((std::numeric_limits<int>::max)() - 1);

    showing = false;
    
    activityIndicator = ActivityIndicator::create();
    activityIndicator->setHidesWhenStopped(true);
    addChild(activityIndicator);
    
    return true;
}

ProgressLayer::~ProgressLayer()
{
    
}

void ProgressLayer::show()
{
    if (showing) return;
    showing = true;
    
    //display background with less opacity than the indicator
    setOpacity(kProgressLayerBackgroundShowOpacity);
    
    //indicator shows itself when it starts animating
    activityIndicator->startAnimating();
}

void ProgressLayer::hide()
{
    if (!showing) return;
    showing = false;
    
    //hide everything
    setOpacity(0);
    activityIndicator->stopAnimating();
}


void ProgressLayer::attach(cocos2d::Node* node)
{
	if(node->getChildByTag(128) == nullptr) {
		this->setContentSize(node->getContentSize());
		node->addChild(this);
		nodes_layout::centerNode(this);
		nodes_layout::centerNode(activityIndicator);
		this->show();
	}
}

void ProgressLayer::detach(cocos2d::Node* node)
{
	if(node->getChildByTag(128) == this)
	{
		this->removeFromParent();
	}
}

bool ProgressLayer::onTouchBegan(CCTouch*, CCEvent*)
{
    return true;
}
