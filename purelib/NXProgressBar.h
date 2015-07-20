/*
 * Copyright (c) 2014 purelib. All rights reserved.
 */


#ifndef _NXPROGRESSBAR_H_
#define _NXPROGRESSBAR_H_

#include "NXGameDefs.h"

namespace purelib {

/**
 * @addtogroup GUI
 * @{
 * @original: addtogroup control_extension CCControlSlider
 * @{
 */

class ProgressBar : public cocos2d::Layer
{
    //maunally put in the setters
    /** Contains the receiver¡¯s current value. */
    CC_SYNTHESIZE_READONLY(float, m_value, Value);
    virtual void setValue(float val);
    /** Contains the minimum value of the receiver. 
    * The default value of this property is 0.0. */
    CC_SYNTHESIZE_READONLY(float, m_minimumValue, MinimumValue);
    virtual void setMinimumValue(float val);
    /** Contains the maximum value of the receiver. 
    * The default value of this property is 1.0. */
    CC_SYNTHESIZE_READONLY(float, m_maximumValue, MaximumValue);
    virtual void setMaximumValue(float val);

	// only slider need flow members
    /*CC_SYNTHESIZE(float, m_minimumAllowedValue, MinimumAllowedValue);
    CC_SYNTHESIZE(float, m_maximumAllowedValue, MaximumAllowedValue);*/

    // maybe this should be read-only
    /*CC_SYNTHESIZE_RETAIN(Sprite*, m_thumbSprite, ThumbSprite);*/
    CC_SYNTHESIZE_RETAIN(Sprite*, m_progressSprite, ProgressSprite);
    CC_SYNTHESIZE_RETAIN(Sprite*, m_backgroundSprite, BackgroundSprite);
	
	virtual void needsLayout();
public:
    ProgressBar();
    virtual ~ProgressBar();

    /** 
    * Initializes a slider with a background sprite, a progress bar and a thumb
    * item.
    *
    * @param backgroundSprite  Sprite, that is used as a background.
    * @param progressSprite    Sprite, that is used as a progress bar.
    * @param thumbItem         Sprite, that is used as a thumb.
    */
    virtual bool initWithSprites(Sprite * backgroundSprite, Sprite* progressSprite);

	virtual bool initWithFiles(const char * bgFile, const char* progressFile);

    /** 
    * Creates slider with a background filename, a progress filename and a 
    * thumb image filename.
    */
    static ProgressBar* create(const char* bgFile, const char* progressFile);

    /** 
    * Creates a progress bar with a given background sprite and a progress bar and a
    * thumb item.
    *
    * @see initWithBackgroundSprite:progressSprite:thumbMenuItem:
    */
    static ProgressBar* create(Sprite * backgroundSprite, Sprite* pogressSprite);

public:
    void updateTexture(void);

/** Returns the value for the given location. */
    /*float valueForLocation(CCPoint location);*/
};

// end of GUI group
/// @}
/// @}

};

#endif
