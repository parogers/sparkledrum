/*
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

#define TRIGGER_THRESHOLD 10
#define BUFFER_LEN 1400
#define MIC_PIN A0

byte buffer[BUFFER_LEN];

void setup() 
{
    pinMode(13, OUTPUT);
    digitalWrite(2, HIGH);
    Serial.begin(9600);
}

void loop()
{
    Serial.println("waiting for trigger...");
    while(true) {
        int raw = analogRead(MIC_PIN);
        if (raw > TRIGGER_THRESHOLD) break;
    }

    unsigned int start = millis();
    for (int n = 0; n < BUFFER_LEN; n++) 
    {
        int raw = analogRead(MIC_PIN);
        buffer[n] = (raw >> 2);
    }
    unsigned int finish = millis();
    float sampleLen = (finish-start)/(float)BUFFER_LEN;

    float tm = 0;
    for (int n = 0; n < BUFFER_LEN; n++) {
        Serial.print(tm);
        Serial.print(" ");
        Serial.println(buffer[n]);
        tm += sampleLen;
    }

    Serial.println("done");
    Serial.println(finish - start);

    while(true) ;
}
