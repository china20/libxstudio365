// Copyright (c) 2014 purelib. All rights reserved.
#ifndef _NXLAYERPOPUP_H_
#define _NXLAYERPOPUP_H_
#include <cocos2d.h>
#include "extensions/cocos-ext.h"
#include "NXMacroDefs.h"

USING_NS_CC;
USING_NS_CC_EXT;

namespace purelib {

enum {
    kPopupResultNone,
    kPopupResultOK,
    kPopupResultCancel,
    kPopupResultAbort,
    kPopupResultRetry,
    kPopupResultIgnore,
    kPopupResultYes,
    kPopupResultNo,
    kPopupResultRequestComplete, // 完成挑战对象选择返回值, 挑战时, 需要继续选择助战.
    kPopupResultLogoff,
    kPopupResultLoginOK,
};

enum {
    kPopupTagRoamMenu,
};

class LayerPopup : public CCLayer
{
public:
    CR_FUNC_NTR(LayerPopup);

    LayerPopup(void);
    virtual ~LayerPopup(void);

    virtual bool init(void);

    void pushNodeToHidden(cocos2d::Node* otherToHidden);

    void pushNodesToHidden(const std::vector<cocos2d::Node*>& othersToHidden);

    template<typename _Ty>
    static _Ty* popupWithoutListener(cocos2d::Node* parent, int tag = kCCNodeTagInvalid)
    {
        assert(parent);

        if(tag == kCCNodeTagInvalid || _IsNull(parent->getChildByTag(tag)))
        {
            _Ty* layer = _Ty::create();
            layer->popup(parent, tag);
            return layer;
        }

        return nullptr;
    }

    template<typename _Ty, typename _Fty>
    static _Ty* popupWithPoppedListener(cocos2d::Node* parent, int tag, const _Fty& poppedListener)
    {
        assert(parent);

        if(tag == kCCNodeTagInvalid || _IsNull(parent->getChildByTag(tag)))
        {
            _Ty* layer = _Ty::create();
            layer->setPoppedCallback(poppedListener);
            layer->popup(parent, tag);
            return layer;
        }

        return nullptr;
    }

    template<typename _Ty, typename _Fty>
    static _Ty* popupWithClosingListener(cocos2d::Node* parent, int tag, const _Fty& closedListener)
    {
        assert(parent);

        if(tag == kCCNodeTagInvalid || _IsNull(parent->getChildByTag(tag)))
        {
            _Ty* layer = _Ty::create();
            layer->setClosingCallback(closedListener);
            layer->popup(parent, tag);
            return layer;
        }

        return nullptr;
    }

    template<typename _Ty, typename _Fty>
    static _Ty* popupWithClosedListener(cocos2d::Node* parent, int tag, const _Fty& closedListener)
    {
        assert(parent);

        if(tag == kCCNodeTagInvalid || _IsNull(parent->getChildByTag(tag)))
        {
            _Ty* layer = _Ty::create();
            layer->setClosedCallback(closedListener);
            layer->popup(parent, tag);
            return layer;
        }

        return nullptr;
    }

    void popup(cocos2d::Node* parent, int tag = kCCNodeTagInvalid, cocos2d::Node* otherToHidden = nullptr);

    // post close request
    virtual void close(bool force = false, float closeDelayTime = 0.0f);

    void setPoppedCallback(const std::function<void(void)>& callback = [](void){});
    void setClosingCallback(const std::function<void(void)>& callback = [](void){});
    void setClosedCallback(const std::function<void(int result)>& callback = [](int){});

    void closeButton_TouchUpinside(CCObject*, Control::EventType);

    void callbackShowNodeWhenDidAction(void);

    void callbackRemoveNodeWhenDidAction(void);

    virtual bool onTouchBegan(CCTouch *pTouch, CCEvent *pEvent) override;
    virtual void onTouchEnded(CCTouch *pTouch, CCEvent *pEvent) override;

    CC_SYNTHESIZE(bool, m_closeWhenTouchEnd, CloseWhenTouchEnd);
    CC_SYNTHESIZE(bool, m_bPopped, Popped);

    /// 
    void suspendLayout(cocos2d::Node* ui);
    void resumeLayout(cocos2d::Node* ui);

    void setCloseable(bool bCloseable);
protected:
    //void setBackground(CCScale9Sprite* background);
    void showCloseButton(float scale = 1.0f);
    void hideCloseButton(void);
    void enableCloseButton(void);
    void disableCloseButton(void);
    void setCloseButtonEnabled(bool enabled);
	virtual void runPopupAction();



public:
    ControlButton*                  closeButton;
    std::function<void(void)>       closingCallback;
    std::function<void(int result)> closedCallback;
    std::function<void(void)>       popupedCallback;
    float                           closeDelayTime;
    std::vector<cocos2d::Node*>            othersNeedsHidden; // 其他需要隐藏的节点

    // for loading
    CCSprite*                       loadingBackground;

    bool                            closeable;
    bool                            closed;
    int                             result;
};

};


#endif

