
#ifndef  _INS_IAD_H_
#define  _INS_IAD_H_

#include "cocos-ext.h"
#include "../../../INSOLAdmob.h"

class INSIAd : public inewsoft::ol::Admob
{
public:
    
    virtual bool init(void);
    
    virtual void showAdView(void);
    virtual void hideAdView(void);
    
    virtual void cleanup(void);
    
};

#endif // _INS_IAD_H_

