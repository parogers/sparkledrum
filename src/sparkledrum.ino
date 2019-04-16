/*     
 * sparkledrum - Flashes lights in time with a beating drum
 * Copyright (C) 2018  Peter Rogers (peter.rogers@gmail.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <Adafruit_NeoPixel.h>
#include "colour.h"

#define STRIP_PIN           2
#define SENSOR_PIN          A2
#define NUM_LEDS            15

/* Globals */

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, STRIP_PIN, NEO_GRB | NEO_KHZ800);

float amplitude = 0;
uint32_t cooldown = 0;
float pos = 0;
uint32_t last_time = 0;
int last_reading = 0;

/* Main */

void setup() 
{
    strip.begin();
    strip.show();
    last_time = millis();
}

void loop() 
{
    /* TODO - handle drum strikes */
    /* ... */

    amplitude = 1;
    
    uint32_t now = millis();

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

    for (int n = 0; n < NUM_LEDS; n++) 
    {
        // Have the strip peak in the middle, and fade out at the ends
        // (ie strip cell brightness is mapped onto a half wavelength sine curve)
        float off = 2*PI*n/(NUM_LEDS);
        float s = abs(sin(pos+off));

        // Raising the result to a power has the affect of smoothing out the half sine curve
        // when the value of 's' is closer to zero.
        s = s*s;

        byte red = (byte)(s*f_red);
        byte green = (byte)(s*f_green);
        byte blue = (byte)(s*f_blue);

        strip.setPixelColor(n, strip.Color(red+1, green+1, blue+1));
    }
    strip.show();

    // Keep the sine wave moving across the strip 
    // (somewhat slower when the light is dimmer)
    pos += 0.01*(0.5+2.5*amplitude);

    // Have the pulse fade out over time according to an exponential decay curve
    // 
    // y = y0*e^(Bx)
    // y' = y0*B*e^(Bx)
    // y' = B*y
    //
    // So: y(t+dt) = y(t) + y'(t)*dt = y(t) + B*y*dt
    //
    if (amplitude > 0) {
        float dt = (now - last_time)/1000.0;
        amplitude -= 4*amplitude*dt;
    }

    last_time = now;
}

