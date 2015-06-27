#include "QuadraticBezierAction.h"

NS_CC_BEGIN

static inline float bezierat(float a, float b, float d, float t)
{
    return (powf(1 - t, 2) * a +
        2 * t * (powf(1 - t, 2)) * b +
        powf(t, 2) * d);
}

//
// QuadraticBezierBy
//

QuadraticBezierBy* QuadraticBezierBy::create(float t, const ccQuadraticBezierConfig& c)
{
    auto bezier2By = new QuadraticBezierBy();
    bezier2By->initWithDuration(t, c);
    bezier2By->autorelease();

    return bezier2By;
}

bool QuadraticBezierBy::initWithDuration(float t, const ccQuadraticBezierConfig& c)
{
    if (ActionInterval::initWithDuration(t))
    {
        _config = c;
        return true;
    }
    return false;
}

void QuadraticBezierBy::startWithTarget(Node *target)
{
    ActionInterval::startWithTarget(target);
    _previousPosition = _startPosition = target->getPosition();
}

QuadraticBezierBy* QuadraticBezierBy::clone() const
{
    // no copy constructor
    auto a = new QuadraticBezierBy();
    a->initWithDuration(_duration, _config);
    a->autorelease();
    return a;
}

void QuadraticBezierBy::update(float time)
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

QuadraticBezierBy* QuadraticBezierBy::reverse() const
{
    ccQuadraticBezierConfig r;

    r.endPosition = -_config.endPosition;
    r.controlPoint = _config.controlPoint + (-_config.endPosition);

    QuadraticBezierBy *action = QuadraticBezierBy::create(_duration, r);
    return action;
}

//
// QuadraticBezierTo
//

QuadraticBezierTo* QuadraticBezierTo::create(float t, const ccQuadraticBezierConfig& c)
{
    auto bezier2To = new QuadraticBezierTo();
    bezier2To->initWithDuration(t, c);
    bezier2To->autorelease();

    return bezier2To;
}

bool QuadraticBezierTo::initWithDuration(float t, const ccQuadraticBezierConfig &c)
{
    if (ActionInterval::initWithDuration(t))
    {
        _toConfig = c;
        return true;
    }

    return false;
}

QuadraticBezierTo* QuadraticBezierTo::clone() const
{
    // no copy constructor
    auto a = new QuadraticBezierTo();
    a->initWithDuration(_duration, _toConfig);
    a->autorelease();
    return a;
}

void QuadraticBezierTo::startWithTarget(Node *target)
{
    QuadraticBezierBy::startWithTarget(target);
    _config.controlPoint = _toConfig.controlPoint - _startPosition;
    _config.endPosition = _toConfig.endPosition - _startPosition;
}

QuadraticBezierTo* QuadraticBezierTo::reverse() const
{
    CCASSERT(false, "CCBezier2To doesn't support the 'reverse' method");
    return nullptr;
}

NS_CC_END;
