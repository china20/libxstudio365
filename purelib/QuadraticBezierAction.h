#ifndef _QUADRATICBEZIERACTION_H_
#define _QUADRATICBEZIERACTION_H_

#include <cocos2d.h>

NS_CC_BEGIN

/** QuadraticBezier configuration structure
*/
typedef struct _ccQuadraticBezierConfig {
    //! end position of the bezier
    Vec2 endPosition;
    //! Bezier control point 1
    Vec2 controlPoint;
} ccQuadraticBezierConfig;

/** @brief An action that moves the target with a cubic Bezier curve by a certain distance.
*/
class CC_DLL QuadraticBezierBy : public ActionInterval
{
public:
    /** creates the action with a duration and a bezier configuration
    * @code
    * when this function bound to js or lua,the input params are changed
    * in js: var create(var t,var table)
    * in lua: lcaol create(local t, local table)
    * @endcode
    */
    static QuadraticBezierBy* create(float t, const ccQuadraticBezierConfig& c);

    //
    // Overrides
    //
    virtual QuadraticBezierBy* clone() const override;
    virtual QuadraticBezierBy* reverse(void) const override;
    virtual void startWithTarget(Node *target) override;
    virtual void update(float time) override;

CC_CONSTRUCTOR_ACCESS:
    QuadraticBezierBy() {}
    virtual ~QuadraticBezierBy() {}

    /** initializes the action with a duration and a bezier configuration */
    bool initWithDuration(float t, const ccQuadraticBezierConfig& c);

protected:
    ccQuadraticBezierConfig _config;
    Vec2 _startPosition;
    Vec2 _previousPosition;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(QuadraticBezierBy);
};

/** @brief An action that moves the target with a cubic Bezier curve to a destination point.
@since v0.8.2
*/
class CC_DLL QuadraticBezierTo : public QuadraticBezierBy
{
public:
    /** creates the action with a duration and a bezier configuration
    * @code
    * when this function bound to js or lua,the input params are changed
    * in js: var create(var t,var table)
    * in lua: lcaol create(local t, local table)
    * @endcode
    */
    static QuadraticBezierTo* create(float t, const ccQuadraticBezierConfig& c);

    //
    // Overrides
    //
    virtual void startWithTarget(Node *target) override;
    virtual QuadraticBezierTo* clone() const override;
    virtual QuadraticBezierTo* reverse(void) const override;

CC_CONSTRUCTOR_ACCESS:
    QuadraticBezierTo() {}
    virtual ~QuadraticBezierTo() {}

    bool initWithDuration(float t, const ccQuadraticBezierConfig &c);

protected:
    ccQuadraticBezierConfig _toConfig;

private:
    CC_DISALLOW_COPY_AND_ASSIGN(QuadraticBezierTo);
};

NS_CC_END;

#endif
