#ifndef __STRIP_ANIMATION_H__
#define __STRIP_ANATIMION_H__

#include "Arduino.h"

class StripAnimation 
{
public:
    uint32_t last_time = 0;
    float amplitude;
    float pos = 0;
    float speed;
    float dt;

    StripAnimation();

    void begin();
    void update();
};

extern StripAnimation animation;

#endif

