// Copyright (c) 2014 inewsoft Corporation. All rights reserved.
#ifndef _NXALERTVIEW_DELEGATE_H_
#define _NXALERTVIEW_DELEGATE_H_

namespace nx {

class AlertViewDelegate
{
public:
    virtual void alertViewClickedButtonAtIndex(int buttonIndex) = 0;
};

};

#endif // _INS_ALERTVIEW_DELEGATE_H_
