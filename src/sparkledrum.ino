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

#define STRIP_PIN           PB2
#define SENSOR_PIN          A2
#define POT_PIN             A3
#define NUM_LEDS            12
#define SENSOR_THRESHOLD    10
#define SENSOR_COOLDOWN     75

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
    //int threshold = analogRead(POT_PIN)/4;
    int threshold = SENSOR_THRESHOLD;
    int reading = analogRead(SENSOR_PIN);

    if (last_reading > threshold && reading > threshold && millis() > cooldown) {
        amplitude = 1;
        cooldown = millis() + SENSOR_COOLDOWN;
    }
    last_reading = reading;

    uint32_t now = millis();

    // Have the strip colour vary slightly over time
    byte peak_red, peak_green, peak_blue;
    hsv2rgb(
        150+50*sin(now/800.0), 200, 200, 
        &peak_red, &peak_green, &peak_blue);

    for (int n = 0; n < NUM_LEDS; n++) 
    {
        // Have the strip peak in the middle, and fade out at the ends
        // (ie strip cell brightness is mapped onto a half wavelength sine curve)
        float off = PI*n/(NUM_LEDS);
        float s = abs(sin(pos+off));

        // Cubing the result has the affect of smoothing out the half sine curve
        // when the value of 's' is closer to zero.
        s = s*s*s;

        byte red = (byte)(s*peak_red*amplitude);
        byte green = (byte)(s*peak_green*amplitude);
        byte blue = (byte)(s*peak_blue*amplitude);

        strip.setPixelColor(n, strip.Color(red, green, blue));
    }
    strip.show();

    // Keep the sine wave moving across the strip 
    // (somewhat slower when the light is dimmer)
    pos += 0.02*(1+amplitude);

    // Have the pulse fade out over time according to an exponential decay curve
    if (amplitude > 0) {
        float dt = (now - last_time)/1000.0;
        amplitude -= 3*amplitude*dt;
    }

    last_time = now;
}

