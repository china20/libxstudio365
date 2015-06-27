#include "NXParticleFactory.h"
namespace purelib {
cocos2d::CCParticleSystem* ParticleFactory::spawn(ParticleType type, const char* image)
{
    CCParticleSystem* pParticle = nullptr;
    switch(type) 
    {
    case ParticleType_Fire:
        pParticle = CCParticleFire::create();
        break;
    case ParticleType_Fireworks:
        pParticle = CCParticleFireworks::create();
        break;
    case ParticleType_Sun:
        pParticle = CCParticleSun::create();
        break;
    case ParticleType_Galaxy:
        pParticle = CCParticleGalaxy::create();
        break;
    case ParticleType_Flower:
        pParticle = CCParticleFlower::create();
        break;
    case ParticleType_Meteor:
        pParticle = CCParticleMeteor::create();
        break;
    case ParticleType_Spiral:
        pParticle = CCParticleSpiral::create();
        break;
    case ParticleType_Explosion:
        pParticle = CCParticleExplosion::create();
        break;
    case ParticleType_Smoke:
        pParticle = CCParticleSmoke::create();
        break;
    case ParticleType_Snow:
        pParticle = CCParticleSnow::create();
        break;
    case ParticleType_Rain:
        pParticle = CCParticleRain::create();
        break;
    default:
        CCLOGWARN("ParticleFactory: Unknow particle type");
    };

    if(pParticle != nullptr)
    {
        pParticle->retain();
        pParticle->setTexture( CCTextureCache::sharedTextureCache()->addImage(image) );
        //pParticle->setAutoRemoveOnFinish(true);
    }
    return pParticle;
}

cocos2d::CCParticleSystem* ParticleFactory::spawnFromFile(const char* plist)
{
    CCParticleSystem* emitter = CCParticleSystemQuad::create(plist);
    
    return emitter;
}

cocos2d::CCParticleSystem* ParticleFactory::spawnFromFile(const char* plist, float duration, bool autoRemoveFromParent, bool blendAdditive)
{
    CCParticleSystem* emitter = CCParticleSystemQuad::create(plist);
    emitter->setDuration(duration);
    emitter->setAutoRemoveOnFinish(autoRemoveFromParent);
    emitter->setBlendAdditive(blendAdditive);
    return emitter;
}

};



