#ifndef _NX_CLIPPING_NODE_H_
#define _NX_CLIPPING_NODE_H_

#include <cocos2d.h>
#include <algorithm>
#include "deprecated/CCDeprecated.h"

USING_NS_CC;

namespace purelib
{
	/*@auto leo.lei
	@function add clipper gui
	@datatime:2015.3.13
	*/
	enum dir_type
	{
		dir_left = 0,
		dir_right = 1,
		dir_up = 2,
		dir_down = 3,
		dir_mid_left = 4,
		dir_mid_right = 5,
		dir_mid_up = 6,
		dir_mid_down = 7,
	};
	class ClipperNode :public ClippingNode
	{

		//get duration
		CC_SYNTHESIZE_READONLY(float, m_nDuration, Duration);
		void setDuration(float dt);

		//get clipper node direct
		CC_SYNTHESIZE_READONLY(dir_type, m_nDirect, Direct);
		void setDirect(dir_type direct);

		//get clpper rect
		CC_SYNTHESIZE_READONLY(Vec2*, m_nVec, Rectage);
		void setRectage(Vec2* vec);

		void update(float dt);

		void Begin_action();

	public:
		ClipperNode(void);
		virtual ~ClipperNode();

		static ClipperNode* create();
	private:
		float	m_addTime;
	};
}

#endif