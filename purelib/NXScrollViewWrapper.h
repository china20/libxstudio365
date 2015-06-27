// Copyright (c) 2014 purelib Corporation. All rights reserved.
#ifndef _INS_SCROLLVIEWWRAPPER_H_
#define _INS_SCROLLVIEWWRAPPER_H_
#include "extensions/cocos-ext.h"
using namespace cocos2d;
using namespace cocos2d::extension;

namespace purelib {
class ScrollViewWrapper : public ScrollView
{
public:
    CREATE_FUNC(ScrollViewWrapper);
    static ScrollViewWrapper* create(CCSize size, cocos2d::Node* container = NULL)
    {
        ScrollViewWrapper* pRet = new ScrollViewWrapper();
        if (pRet && pRet->initWithViewSize(size, container))
        {
            pRet->autorelease();
        }
        else
        {
            CC_SAFE_DELETE(pRet);
        }
        return pRet;
    }
    virtual void setContentSize(const CCSize & size) { ScrollView::setViewSize(size); }
    virtual const CCSize& getContentSize() const { return getViewSize(); }
    void setContainerSize(const CCSize& size) {ScrollView::setContentSize(size); }
    const CCSize& getContainerSize(void) const { return ScrollView::getContentSize(); }

    void adjustDistance(const CCPoint& distance) // very important
    {
        this->_scrollDistance = distance;
    }
};

class CCTableViewWrapper : public TableView
{
public:
    static CCTableViewWrapper* create(TableViewDataSource* dataSource, CCSize size, cocos2d::Node *container = nullptr)
    {
        CCTableViewWrapper *table = new CCTableViewWrapper();
        table->initWithViewSize(size, container);
        table->autorelease();
        table->setDataSource(dataSource);
        table->_updateCellPositions();
        table->_updateContentSize();

        return table;
    }
    virtual void setContentSize(const CCSize & size) { ScrollView::setViewSize(size); }
    virtual const CCSize& getContentSize() const { return ScrollView::getViewSize(); }
    void setContainerSize(const CCSize& size) {ScrollView::setContentSize(size); }
    const CCSize& getContainerSize(void) const { return ScrollView::getContentSize(); }
};

};

#endif
