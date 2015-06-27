#include "CCQuadBezierAction.h"

NS_CC_BEGIN

static inline float bezierat(float a, float b, float d, float t)
{
    return (powf(1 - t, 2) * a +
        2 * t * (1 - t) * b +
        powf(t, 2) * d);
}

//
// QuadBezierBy
//

QuadBezierBy* QuadBezierBy::create(float t, const ccQuadBezierConfig& c)
{
    auto bezier2By = new QuadBezierBy();
    bezier2By->initWithDuration(t, c);
    bezier2By->autorelease();

    return bezier2By;
}

bool QuadBezierBy::initWithDuration(float t, const ccQuadBezierConfig& c)
{
    if (ActionInterval::initWithDuration(t))
    {
        _config = c;
        return true;
    }

    return false;
}

void QuadBezierBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _previousPosition = _startPosition = target->getPosition();
}

QuadBezierBy* QuadBezierBy::clone() const
{
    // no copy constructor
    auto a = new QuadBezierBy();
    a->initWithDuration(_duration, _config);
    a->autorelease();
    return a;
}

void QuadBezierBy::update(float time)
{
    if (_target)
    {
        float xa = 0;
        float xb = _config.controlPoint.x;
        float xd = _config.endPosition.x;

        float ya = 0;
        float yb = _config.controlPoint.y;
        float yd = _config.endPosition.y;

        float x = bezierat(xa, xb, xd, time);
        float y = bezierat(ya, yb, yd, time);
		
#if CC_ENABLE_STACKABLE_ACTIONS
        Vec2 currentPos = _target->getPosition();
        Vec2 diff = currentPos - _previousPosition;
        _startPosition = _startPosition + diff;

        Vec2 newPos = _startPosition + Vec2(x, y);
        _target->setPosition(newPos);

        _previousPosition = newPos;
#else
        _target->setPosition(_startPosition + Vec2(x, y));
#endif // !CC_ENABLE_STACKABLE_ACTIONS
    }
}

QuadBezierBy* QuadBezierBy::reverse() const
{
    ccQuadBezierConfig r;

    r.endPosition = -_config.endPosition;
    r.controlPoint = _config.controlPoint + (-_config.endPosition);

    QuadBezierBy *action = QuadBezierBy::create(_duration, r);
    return action;
}

//
// QuadBezierTo
//

QuadBezierTo* QuadBezierTo::create(float t, const ccQuadBezierConfig& c)
{
    auto bezier2To = new QuadBezierTo();
    bezier2To->initWithDuration(t, c);
    bezier2To->autorelease();

    return bezier2To;
}

bool QuadBezierTo::initWithDuration(float t, const ccQuadBezierConfig &c)
{
    if (ActionInterval::initWithDuration(t))
    {
        _toConfig = c;
        return true;
    }

    return false;
}

QuadBezierTo* QuadBezierTo::clone() const
{
    // no copy constructor
    auto a = new QuadBezierTo();
    a->initWithDuration(_duration, _toConfig);
    a->autorelease();
    return a;
}

void QuadBezierTo::startWithTarget(Node *target)
{
    QuadBezierBy::startWithTarget(target);
    _config.controlPoint = _toConfig.controlPoint - _startPosition;
    _config.endPosition = _toConfig.endPosition - _startPosition;
}

QuadBezierTo* QuadBezierTo::reverse() const
{
    CCASSERT(false, "CCBezier2To doesn't support the 'reverse' method");
    return nullptr;
}

NS_CC_END;
