#ifndef _CCQUADBEZIERACTION_H_
#define _CCQUADBEZIERACTION_H_

#include <cocos2d.h>

NS_CC_BEGIN

/** Bezier configuration structure
*/
typedef struct _ccQuadBezierConfig {
    //! end position of the bezier
    Vec2 endPosition;
    //! Bezier control point 1
    Vec2 controlPoint;
} ccQuadBezierConfig;

/** @brief An action that moves the target with a cubic Bezier curve by a certain distance.
*/
class CC_DLL QuadBezierBy : public ActionInterval
{
public:
    /** creates the action with a duration and a bezier configuration
    * @code
    * when this function bound to js or lua,the input params are changed
    * in js: var create(var t,var table)
    * in lua: lcaol create(local t, local table)
    * @endcode
    */
	static QuadBezierBy* create(float t, const ccQuadBezierConfig& c);

    //
    // Overrides
    //
    virtual QuadBezierBy* clone() const override;
    virtual QuadBezierBy* reverse(void) const override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;

CC_CONSTRUCTOR_ACCESS:
    QuadBezierBy() {}
    virtual ~QuadBezierBy() {}

    /** initializes the action with a duration and a bezier configuration */
	bool initWithDuration(float t, const ccQuadBezierConfig& c);

protected:
	ccQuadBezierConfig _config;
    Vec2 _startPosition;
    Vec2 _previousPosition;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(QuadBezierBy);
};

/** @brief An action that moves the target with a cubic Bezier curve to a destination point.
@since v0.8.2
*/
class CC_DLL QuadBezierTo : public QuadBezierBy
{
public:
    /** creates the action with a duration and a bezier configuration
    * @code
    * when this function bound to js or lua,the input params are changed
    * in js: var create(var t,var table)
    * in lua: lcaol create(local t, local table)
    * @endcode
    */
	static QuadBezierTo* create(float t, const ccQuadBezierConfig& c);

    //
    // Overrides
    //
    virtual void startWithTarget(Node *target) override;
    virtual QuadBezierTo* clone() const override;
    virtual QuadBezierTo* reverse(void) const override;

CC_CONSTRUCTOR_ACCESS:
    QuadBezierTo() {}
    virtual ~QuadBezierTo() {}

	bool initWithDuration(float t, const ccQuadBezierConfig &c);

protected:
	ccQuadBezierConfig _toConfig;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(QuadBezierTo);
};

NS_CC_END;

#endif
