#include "fsine.h"

#define APPROX_PI      3.142
#define APPROX_2_PI    6.283

float fast_sin(float angle)
{
    if (angle > APPROX_2_PI) angle = angle - APPROX_2_PI*(int)(angle/APPROX_2_PI);

    if (angle > APPROX_PI) angle -= APPROX_PI;
  
    float z = (angle-1.570);
    return -0.405*z*z + 1;
}

int fast_isin(int angle)
{

}

