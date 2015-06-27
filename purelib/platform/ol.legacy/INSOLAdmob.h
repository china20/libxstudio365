// Copyright (c) 2014 inewsoft Corporation. All rights reserved.
#ifndef _INS_OL_ADMOB_H_
#define _INS_OL_ADMOB_H_

namespace inewsoft {
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

};

#endif


