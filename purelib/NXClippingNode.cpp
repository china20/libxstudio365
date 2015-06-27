#include "NXClippingNode.h"


purelib::ClipperNode::ClipperNode(void)
{
	m_nDuration = 0.0f;
	m_nDirect = dir_left;
	m_nVec = new Vec2[32];
	m_addTime = 0.0f;
}

purelib::ClipperNode::~ClipperNode()
{
	m_nDuration = 0.0f;
	m_nDirect = dir_left;
	delete[] m_nVec;
	m_nVec = nullptr;
	m_addTime = 0.0f;
}

void purelib::ClipperNode::setDuration(float dt)
{
	m_nDuration = dt;
}

void purelib::ClipperNode::setDirect(dir_type direct)
{
	m_nDirect = direct;
}

void purelib::ClipperNode::setRectage(Vec2* vec)
{
	for (int i = 0; i < sizeof(vec); i++)
	{
		m_nVec[i] = vec[i];
	}
	DrawNode* node = DrawNode::create();
	ccColor4F white = { 1, 1, 1, 1 };
	node->drawPolygon(vec, 4, white, 1, white);
	node->setOpacity(0);
	//switch (m_nDirect)
	//{
	//case purelib::dir_left:
	//	node->setAnchorPoint(Vec2(-1, 0));
	//	break;
	//case purelib::dir_right:
	//	node->setAnchorPoint(Vec2(1, 0));
	//	break;
	//case purelib::dir_up:
	//	node->setAnchorPoint(Vec2(0, 1));
	//	break;
	//case purelib::dir_down:
	//	node->setAnchorPoint(Vec2(0, -1));
	//	break;
	//case purelib::dir_mid_left:
	//case purelib::dir_mid_right:
	//case purelib::dir_mid_up:
	//case purelib::dir_mid_down:
	//	node->setAnchorPoint(Vec2(0, 0));
	//	break;
	//default:
	//	break;
	//}
	this->setStencil(node);
}

void purelib::ClipperNode::Begin_action()
{
	this->scheduleUpdate();
}

void purelib::ClipperNode::update(float dt)
{
	m_addTime += dt;
	if (m_nDuration > m_addTime)
	{
		if (this->getStencil())
		{
			this->getStencil()->setScaleX(m_addTime / m_nDuration);
		}
	}
	else
	{
		this->unscheduleUpdate();
		if (this->getParent() != nullptr)
		{
			this->removeFromParent();
		}
	}
}

purelib::ClipperNode* purelib::ClipperNode::create()
{
	purelib::ClipperNode *ret = new purelib::ClipperNode();
	if (ret && ret->init())
	{
		ret->autorelease();
	}
	else
	{
		CC_SAFE_DELETE(ret);
	}

	return ret;
}

