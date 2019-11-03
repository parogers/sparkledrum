#include <Adafruit_NeoPixel.h>
#include "animation.h"
#include "colour.h"

#define STRIP_PIN           2
#define NUM_LEDS            15

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, STRIP_PIN, NEO_GRB | NEO_KHZ800);

static StripAnimation animation;

StripAnimation::StripAnimation() 
{
    last_time = millis();
}

void StripAnimation::begin()
{
    strip.begin();
    strip.show();
}

void StripAnimation::update()
{
    uint32_t now = millis();
    dt = (now - last_time)/1000.0;

    // Have the strip colour vary slightly over time
    byte peak_red, peak_green, peak_blue;
    hsv2rgb(
        210+70*sin(now/800.0), 200, 200, 
        &peak_red, &peak_green, &peak_blue);

    float f_red = peak_red * amplitude;
    float f_green = peak_green * amplitude;
    float f_blue = peak_blue * amplitude;

    f_red += (1-amplitude)*8;
    f_green += (1-amplitude)*4*(1+sin(now/500.0));
    f_blue += (1-amplitude)*8;
    
    float step = 2*PI/NUM_LEDS;
    float off = 0;
    
    for (int n = 0; n < NUM_LEDS; n++) 
    {
        // Have the strip peak in the middle, and fade out at the ends
        // (ie strip cell brightness is mapped onto a half wavelength sine curve)
        off += step;
        float s = sin(pos+off);

        if (s < 0) s = -s;

        // Raising the result to a power has the affect of smoothing out the half sine curve
        // when the value of 's' is closer to zero.
        s = s*s;

        byte red = (byte)(s*f_red);
        byte green = (byte)(s*f_green);
        byte blue = (byte)(s*f_blue);

        strip.setPixelColor(n, red+1, green+1, blue+1);
    }
    strip.show();

    // Keep the sine wave moving across the strip 
    // (somewhat slower when the light is dimmer)
    //pos += 0.01*(0.5+2.5*amplitude);
    pos += speed*dt;

    last_time = now;
}

