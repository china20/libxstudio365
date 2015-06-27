#include "NXTouchSpread.h"
#include "NXNodesUtility.h"

CCTouchSpread::CCTouchSpread(void)
{
	this->control = nullptr;
}

CCTouchSpread::~CCTouchSpread(void)
{
	CC_SAFE_RELEASE(this->control);
}

void CCTouchSpread::attach(Control* button)
{
	if(button) {
		button->retain();
		this->control = button;
	}
}

//CCRect CCTouchableSprite::rect()
//{
//    CCSize s = getTexture()->getContentSize();
//    return CCRectMake(-s.width / 2, -s.height / 2, s.width, s.height);
//}
/*
void CCTouchSpread::onEnter()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->addTargetedDelegate(this, 0, true);
    CCSprite::onEnter();
}

void CCTouchSpread::onExit()
{
    CCDirector* pDirector = CCDirector::sharedDirector();
    pDirector->getTouchDispatcher()->removeDelegate(this);
    CCSprite::onExit();
}   */ 

bool CCTouchSpread::containsTouchLocation(CCTouch* touch)
{
	CCNode* self = dynamic_cast<CCNode*>(this);
	CC_ASSERT(self);

	CCPoint touchPoint = self->getParent()->convertTouchToNodeSpace(touch);
	return self->boundingBox().containsPoint(touchPoint);
  // return rect().containsPoint(convertTouchToNodeSpaceAR(touch));
}

bool CCTouchSpread::onTouchBegan(CCTouch* touch, CCEvent* event)
{
	if ( !nodes_utility::checkVisibility(dynamic_cast<CCNode*>(this) ) || !containsTouchLocation(touch) ) 
		return false;
    
	if(this->control) {
		this->control->setHighlighted(true);
		this->control->sendActionsForControlEvents(Control::EventType::TOUCH_DOWN);
	}
    return true;
}

void CCTouchSpread::onTouchMoved(CCTouch* touch, CCEvent* event)
{
    // If it weren't for the TouchDispatcher, you would need to keep a reference
    // to the touch from touchBegan and check that the current touch is the same
    // as that one.
    // Actually, it would be even more complicated since in the Cocos dispatcher
    // you get CCSets instead of 1 UITouch, so you'd need to loop through the set
    // in each touchXXX method.
    
   // CCAssert(m_state == kTouchStateGrabbed, "TouchableSprite - Unexpected state!");    
    
    //CCPoint touchPoint = touch->getLocation();
    
   // setPosition( touchPoint );

	/* if (!isEnabled() || !isPushed() || isSelected())
    {
        if (isHighlighted())
        {
            setHighlighted(false);
        }
        return;
    }*/
	if ( !nodes_utility::checkVisibility(dynamic_cast<CCNode*>(this) ) )
	{
		return;
	}

	if(!control)
		return;
    
	bool isTouchMoveInside = containsTouchLocation(touch);
    if (isTouchMoveInside && !control->isHighlighted())
    {
        control->setHighlighted(true);
        control->sendActionsForControlEvents(Control::EventType::DRAG_ENTER);
    }
    else if (isTouchMoveInside && control->isHighlighted())
    {
        control->sendActionsForControlEvents(Control::EventType::DRAG_INSIDE);
    }
    else if (!isTouchMoveInside && control->isHighlighted())
    {
        control->setHighlighted(false);
        
        control->sendActionsForControlEvents(Control::EventType::DRAG_EXIT);        
    }
    else if (!isTouchMoveInside && !control->isHighlighted())
    {
        control->sendActionsForControlEvents(Control::EventType::DRAG_OUTSIDE);        
    }
}


void CCTouchSpread::onTouchEnded(CCTouch* touch, CCEvent* event)
{
	if ( !nodes_utility::checkVisibility(dynamic_cast<CCNode*>(this) ) )
	{
		return;
	}

	if(!this->control)
		return;

	this->control->setHighlighted(false);

	if(containsTouchLocation(touch)) 
	{
		this->control->sendActionsForControlEvents(Control::EventType::TOUCH_UP_INSIDE);
	}
	else {
		this->control->sendActionsForControlEvents(Control::EventType::TOUCH_UP_OUTSIDE);    
	}
} 

void CCTouchSpread::onTouchCancelled(CCTouch* touch, CCEvent* event)
{
}

//void CCTouchableSprite::draw(void)
//{
//    // draw bounding box
//    CCSprite::draw();
//    CCPoint pos = this->convertToWorldSpace(getPosition());
//    CCSize size = this->getContentSize();
//    CCPoint vertices[4]={
//        ccp(pos.x+1, pos.y+1),
//        ccp(pos.x+size.width-1, pos.y+1),
//        ccp(pos.x+size.width-1, pos.y+size.height-1),
//        ccp(pos.x+1, pos.y+size.height-1),
//    };
//    ccDrawColor4B(0, 0, 255, 255);
//    ccDrawPoly(vertices, 4, true);
//}


