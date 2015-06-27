// Copyright (c) 2014 purelib.
#ifndef _NX_OL_ADMOB_H_
#define _NX_OL_ADMOB_H_

namespace ol {
class Admob
{
public:
    virtual ~Admob(void);
    virtual bool init(void){return false;};
    virtual void showAdView(void){};
    virtual void hideAdView(void){};
    virtual void cleanup(void);
};

};


#endif


